/**
 * \defgroup execution Execution
 * \ingroup execution
 * @{
 * \page execution_description
 *
 * The execution module is responsible for executing instructions, task switching, and exception handling. The internal state of the execution module can be
 * accessed through methods.
 *
 * Before execution can begin the execution engine must be passed a dj_vm object. This object contains information about loaded infusions, running threads and
 * the like. Then one of the threads from the dj_vm object has to be enabled, after which execution can start.
 *
 * Methods that start with dj_exec are accessible from other modules and expose the execution module's internal state to the outside world. The dj_exec_stack
 * methods are used to control the state of the operand stack. Native methods use these to pop arguments off the stack, and optionally push a result
 * onto the stack.
 *
 * When the dj_exec_run(nrOpcodes) method is called the execution engine will execute at most [nrOpcodes] instructions. Execution can terminate prematurely
 * for instance when a thread terminates, when an uncaught exception is thrown, or when a thread is put to sleep with Thread.sleep().
 *
 * Because Darjeeling uses a double-ended stack, there are separate stack manipulation methods for shorts, integers and references. Note that integer arithmetic
 * using a 16-bit stack width is not fully implemented at this time.
 *
 */

#include "common/execution/execution.h"
#include "common/types.h"
#include "common/parse_infusion.h"
#include "common/infusion.h"
#include "common/object.h"
#include "common/array.h"
#include "common/heap/heap.h"
#include "common/vm.h"
#include "common/global_id.h"
#include "common/debug.h"
#include "common/panic.h"

// platform-specific configuration
#include "config.h"

// generated at infusion time
#include "base_definitions.h"

// platform specific
#include "pointerwidth.h"

#include "opcodes.c"

// currently selected Virtual Machine context
static dj_vm *vm;

// global variables for quick access
static dj_thread *currentThread;

// execution state
static uint16_t pc;
static dj_di_pointer code;

static int16_t *intStack;
static ref_t *refStack;

static ref_t *localReferenceVariables;
static int16_t *localIntegerVariables;

static ref_t *referenceParameters;
static int16_t *integerParameters;

static uint8_t nrReferenceParameters;
static uint8_t nrIntegerParameters;

static ref_t this;

static int nrOpcodesLeft;

#ifdef DARJEELING_DEBUG
static uint32_t totalNrOpcodes;
static uint16_t oldPc;
#endif
#ifdef DARJEELING_DEBUG_TRACE
static int callDepth = 0;
#endif

/**
 * Tells the execution engine which VM is currently running. In principle this should be called once in the main.
 * @param _vm the virtual machine to set as the executing VM
 */
void dj_exec_setVM(dj_vm *_vm)
{
#ifdef DARJEELING_DEBUG
		totalNrOpcodes=0;
#endif
	vm = _vm;
}

/**
 * Returns the VM that is currently executing.
 * @return the current VM
 */
inline dj_vm *dj_exec_getVM()
{
	return vm;
}


/**
 * Tells the execution loop to stop after it has finished interpreting the current instruction. The next instruction
 * will not be fetched.
 */
inline void dj_exec_breakExecution()
{
	nrOpcodesLeft = -1;
}

/**
 * Saves execution state (stack pointer, pc) in the given frame struct. This method
 * is used in context switching and method invocations/returns.
 * @param frame a dj_frame struct in which to save the current state
 */
static inline void dj_exec_saveLocalState(dj_frame *frame)
{
	frame->pc = pc;
	frame->nr_int_stack = ((char*)intStack - dj_frame_stackStartOffset(frame)) / sizeof(int16_t);
	frame->nr_ref_stack = (dj_frame_stackEndOffset(frame) - (char*)refStack) / sizeof(ref_t);
}

/**
 * Loads execution state (stack pointer, pc, code pointer, local variables) from a dj_frame struct. This method
 * is used in context switching and method invocations/returns.
 * @param frame the frame to load
 */
static inline void dj_exec_loadLocalState(dj_frame *frame)
{
	// get program counter, stack pointers, code
	dj_di_pointer methodImpl = dj_global_id_getMethodImplementation(frame->method);
	code = dj_di_methodImplementation_getData(methodImpl);
	pc = frame->pc;

	intStack = dj_frame_getIntegerStack(frame);
	refStack = dj_frame_getReferenceStack(frame);

	localReferenceVariables = dj_frame_getLocalReferenceVariables(frame);
	localIntegerVariables = dj_frame_getLocalIntegerVariables(frame);

	nrReferenceParameters = dj_di_methodImplementation_getReferenceArgumentCount(methodImpl) + ((dj_di_methodImplementation_getFlags(methodImpl)&FLAGS_STATIC)?0:1);
	nrIntegerParameters = dj_di_methodImplementation_getIntegerArgumentCount(methodImpl);

	if (frame->parent!=NULL)
	{
		referenceParameters = dj_frame_getReferenceStack(frame->parent) + nrReferenceParameters - 1;
		integerParameters = dj_frame_getIntegerStack(frame->parent) - nrIntegerParameters;
		this = nullref;
	} else
	{
		// Special corner case for the run() method in threads. In this case the method will need to access the implicit
		// 'this' parameter as run() is a virtual method. Usually parameters are accessed directly from the
		// caller stack, but since there is no caller frame we copy the 'this' reference from the thread object to a
		// global variable and wire the callerReferenceStack to point to it. Not very elegant, but it gets the job done.
		// this = VOIDP_TO_REF(currentThread->runnable);
		this = VOIDP_TO_REF(currentThread->runnable);
		referenceParameters = &this;
		integerParameters = NULL;

	}


}

/**
 * Deactivates a thread by saving the execution state (program counter, stack, etc) into the top execution frame.
 * This method is used in context switching.
 * @see dj_exec_saveLocalState()
 * @param thread the thread to deactivate
 */
void dj_exec_deactivateThread(dj_thread *thread)
{
	// store program counter and stack pointers
	if (thread->frameStack!=NULL)
		dj_exec_saveLocalState(thread->frameStack);
}

/**
 * Activates a thread by loading the execution state (program counter, stack, etc) from the top execution frame.
 * This method is used in context switching.
 * @see dj_exec_loadLocalState()
 * @param thread the thread to deactivate
 */
void dj_exec_activate_thread(dj_thread *thread)
{
	currentThread = thread;

	if (thread==NULL) return;

	if (thread->frameStack!=NULL)
		dj_exec_loadLocalState(thread->frameStack);
	else
        dj_panic(DJ_PANIC_ILLEGAL_INTERNAL_STATE);

}

void dj_exec_updatePointers()
{
	vm = dj_mem_getUpdatedPointer(vm);
	this = dj_mem_getUpdatedReference(this);
}

/**
 * Returns the current infusion which is the infusion containing the method that's currently executing. This infusion
 * serves as a context for instructions, as any local ID's contained in them are relative to it.
 * @return the infusion containing the method that's currently executing
 */
dj_infusion * dj_exec_getCurrentInfusion()
{
	return currentThread->frameStack->method.infusion;
}

/**
 * Returns the thread that the execution module is currently busy executing.
 */
dj_thread *dj_exec_getCurrentThread() { return currentThread; }

/**
 * Fetches a byte from the code pointer. Increases the PC by 1.
 * TODO make quicker using code++
 */
static inline uint8_t fetch()
{
	uint8_t bytecode = dj_di_getU8(code+pc);
	pc++;
	return bytecode;
}

/**
 * Peeks a 16-bit value from the code area, doesn't increase the PC.
 */
static inline int16_t peek16()
{
	return (dj_di_getU8(code+pc) << 8) | dj_di_getU8(code+pc+1);
}

/**
 * Peeks a 16-bit value from the code area at PC+n, doesn't increase the PC.
 * @param n the offset in bytes from the current PC at which to peek
 */
static inline int16_t peekn16(int n)
{
	return (dj_di_getU8(code+pc+n) << 8) | dj_di_getU8(code+pc+1+n);
}


/**
 * Fetches a 16-bit value from the code area and increases the PC by 2.
 */
static inline int16_t fetch16()
{
	// TODO speed up using getU16
	uint16_t ret = (dj_di_getU8(code+pc) << 8) | dj_di_getU8(code+pc+1);
	pc += 2;
	return ret;
}

/**
 * Fetches a 32-bit value from the code area and increases the PC by 4.
 */
static inline int32_t fetch32()
{
	int32_t ret = ((uint32_t)dj_di_getU8(code+pc  ) << 24) |
		((uint32_t)dj_di_getU8(code+pc+1) << 16) |
		((uint32_t)dj_di_getU8(code+pc+2) << 8) |
		(uint32_t)dj_di_getU8(code+pc+3);

	pc += 4;
	return ret;
}

/**
 * Peeks a 32-bit value from the code area at PC+n, doesn't increase the PC.
 * @param n the offset in bytes from the current PC at which to peek
 */
static inline uint32_t peekn32(int n)
{
	return ((uint32_t)dj_di_getU8(code+pc+n  ) << 24) |
		((uint32_t)dj_di_getU8(code+pc+n+1) << 16) |
		((uint32_t)dj_di_getU8(code+pc+n+2) << 8) |
		(uint32_t)dj_di_getU8(code+pc+n+3);
}

/**
 * Fetches a dj_local_id value from the code area and increases the PC by 2.
 */
static inline dj_local_id dj_fetchLocalId()
{
	dj_local_id ret;
	ret.infusion_id = fetch();
	ret.entity_id = fetch();
	return ret;
}

/**
 * Pushes an int (32 bit) onto the runtime stack
 */
static inline void pushInt(int32_t value)
{
	*((int32_t*)intStack) = value;
	intStack+=2;
}

/**
 * Pushes a short (16 bit) onto the runtime stack
 */
static inline void pushShort(int16_t value)
{
	*intStack = value;
	intStack++;
}

/**
 * Pushes a reference onto the runtime stack
 */
static inline void pushRef(ref_t value)
{
	refStack--;
	*refStack = value;
}

/**
 * Pops an int (32 bit) from the runtime stack
 */
static inline int32_t popInt()
{
	intStack-=2;
	return *(int32_t*)intStack;
}

/**
 * Pops a short (16 bit) from the runtime stack
 */
static inline int16_t popShort()
{
	intStack--;
	return *intStack;
}

/**
 * Pops a reference from the runtime stack
 */
static inline ref_t popRef()
{
	ref_t ret = *refStack;
	refStack++;
	return ret;
}

/**
 * Returns the topmost 32 bit integer element on the integer stack, does not change the stackpointer.
 */
static inline int32_t peekInt()
{
	return *(int32_t*)(intStack-2);
}

/**
 * Returns the topmost 16 bit short element on the integer stack, does not change the stackpointer.
 */
static inline int16_t peekShort()
{
	return *(intStack-1);
}

/**
 * Returns the topmost element on the reference stack, does not change the stackpointer.
 */
static inline ref_t peekRef()
{
	return *refStack;
}

/**
 * Peeks the reference stack at a specified depth. For example, if the stack equals
 * [a, b, c, d, e, f], peekDeepRef(0) would return 'a' (same as peek()), and peekDeepRef(3) would return 'c'.
 * Does not change the stackpointer
 */
static inline ref_t peekDeepRef(int depth)
{
	return *(refStack + depth);
}

/**
 * Pushes a short onto the runtime stack. Can be used by functions outside the execution module to interact with the
 * running program.
 * @param value the integer value to push.
 */
void dj_exec_stackPushShort(int16_t value) { pushShort(value); }

/**
 * Pushes an int onto the runtime stack. Can be used by functions outside the execution module to interact with the
 * running program.
 * @param value the integer value to push.
 */
void dj_exec_stackPushInt(int32_t value) { pushInt(value); }

/**
 * Pushes a reference onto the runtime stack. Can be used by functions outside the execution module to interact with the
 * running program. Corruping the stack can lead to crashes, use with care!
 * @param value the reference value to push.
 */
void dj_exec_stackPushRef(ref_t value) { pushRef(value); }

/**
 * Pops an int from the runtime stack. Can be used by functions outside the execution module to interact with the
 * running program. Corruping the stack can lead to crashes, use with care!
 * @return the top value of the runtime stack.
 */
int32_t dj_exec_stackPopInt() { return popInt(); }

/**
 * Pops a short from the runtime stack. Can be used by functions outside the execution module to interact with the
 * running program. Corruping the stack can lead to crashes, use with care!
 * @return the top value of the runtime stack.
 */
int16_t dj_exec_stackPopShort() { return popShort(); }

/**
 * Pops a reference from the runtime stack. Can be used by functions outside the execution module to interact with the
 * running program. Corruping the stack can lead to crashes, use with care!
 * @return the top value of the runtime stack.
 */
ref_t dj_exec_stackPopRef() { return popRef(); }

/**
 * Returns the top of the stack as a short value, but does not change the stackpointer. Can be used by functions outside
 * the execution module to interact with the running program.
 * @return the top value of the runtime stack.
 */
int16_t dj_exec_stackPeekShort() { return peekShort(); }

/**
 * Returns the top of the stack as an integer value, but does not change the stackpointer. Can be used by functions outside
 * the execution module to interact with the running program.
 * @return the top value of the runtime stack.
 */
int32_t dj_exec_stackPeekInt() { return peekInt(); }

/**
 * Returns the top of the stack as a reference value, but does not change the stackpointer. Can be used by functions outside
 * the execution module to interact with the running program.
 * @return the top value of the runtime stack.
 */
ref_t dj_exec_stackPeekRef() { return peekRef(); }

/**
 * Set local reference variable at index
 * @param index local variable slot number
 * @param value reference value
 */
static inline void setLocalRef(int index, ref_t value)
{
	if (index<nrReferenceParameters)
		referenceParameters[-index] = value;
	else
		localReferenceVariables[index-nrReferenceParameters] = value;
}

/**
 * Returns local reference variable at index
 * @param index local variable slot number
 */
static inline ref_t getLocalRef(int index)
{
	if (index<nrReferenceParameters)
		return referenceParameters[-index];
	else
		return localReferenceVariables[index-nrReferenceParameters];
}

/**
 * Set local reference variable at index
 * @param index local variable slot number
 * @param value 32 bit integer value
 */
static inline void setLocalInt(int index, int32_t value)
{
	if (index<nrIntegerParameters)
		*(int32_t*)(integerParameters+index) = value;
	else
		*(int32_t*)(localIntegerVariables+index-nrIntegerParameters) = value;
}
/**
 * Returns 32 bit integer local variable at index
 * @param index local variable slot number
 */
static inline int32_t getLocalInt(int index)
{
	if (index<nrIntegerParameters)
		return *(int32_t*)(integerParameters+index);
	else
		return *(int32_t*)(localIntegerVariables + index -nrIntegerParameters);
}

/**
 * Set local reference variable at index
 * @param index local variable slot number
 * @param value 16 bit short value
 */
static inline void setLocalShort(int index, int16_t value)
{
	if (index<nrIntegerParameters)
		integerParameters[index] = value;
	else
		localIntegerVariables[index-nrIntegerParameters] = value;
}

/**
 * Returns 16 bit short local variable at index
 * @param index local variable slot number
 */
static inline int16_t getLocalShort(int index)
{
	if (index<nrIntegerParameters)
		return integerParameters[index];
	else
		return localIntegerVariables[index-nrIntegerParameters];
}

/**
 * Branches to PC + offset.
 */
static inline void branch(int16_t offset)
{
	pc += offset;
}

/**
 * Enters a method. The method may be either Java or native. If the method is native, the native handler of the
 * method's containg infusion will be called. If the method is not native, a new frame is created and a context
 * switch is performed.
 * @param methodImplId a global id pointing to the method to be executed
 * @param virtualCall indicates if the call is a virtual or static call. In the case of a virtual call the object
 * the method belongs to is on the stack and should be handled as an additional parameter. Should be either 1 or 0.
 */
static inline void callMethod(dj_global_id methodImplId, int virtualCall)
{
	dj_frame *frame;
	dj_native_handler handler;

	// get a pointer in program space to the method implementation block from the method's global id
	dj_di_pointer methodImpl = dj_global_id_getMethodImplementation(methodImplId);

	// check if the method is a native methods
	if ((dj_di_methodImplementation_getFlags(methodImpl)&FLAGS_NATIVE)!=0)
	{
		// the method is native, check if we have a native handler for the infusion the method is in
		handler = methodImplId.infusion->native_handler;
		if (handler!=NULL)
			// we can execute the method by calling the infusion's native handler
			handler(methodImplId);
		else
			// there is no native handler for this method's infusion. Throw an exception
			dj_exec_createAndThrow(BASE_CDEF_javax_darjeeling_vm_NativeMethodNotImplementedError);

	} else
	{
		// create new frame for the function
		frame = dj_frame_create(methodImplId);

		// not enough space on the heap to allocate the frame
		if(frame == NULL)
        {
			dj_exec_createAndThrow(BASE_CDEF_java_lang_StackOverflowError);
			return;
        }

		// save current state of the frame
		dj_exec_saveLocalState(currentThread->frameStack);

		// push the new frame on the frame stack
		dj_thread_pushFrame(currentThread, frame);

		// switch in newly created frame
		dj_exec_loadLocalState(frame);

#ifdef DARJEELING_DEBUG_MEM_TRACE
		dj_mem_dumpMemUsage();
#endif

#ifdef DARJEELING_DEBUG_TRACE
		callDepth++;
#endif
	}

}

/**
 * Returns from a method. The current execution frame is popped off the thread's frame stack. If there are no other
 * frames to execute, the thread ends. Otherwise control is switched to the underlying caller frame.
 */
static inline void returnFromMethod()
{
	dj_di_pointer methodImpl;

	// get the method from the stack frame so we can calculate how many parameters to pop off the operand stack
	methodImpl = dj_global_id_getMethodImplementation(currentThread->frameStack->method);

	// pop frame from frame stack and dealloc it
	dj_frame_destroy(dj_thread_popFrame(currentThread));

	// check if there are elements on the call stack
	if (currentThread->frameStack==NULL)
	{
		// done executing (exited last element on the call stack)
		currentThread->status = THREADSTATUS_FINISHED;
		dj_exec_breakExecution();
	} else
	{
		// perform context switch.
		dj_exec_activate_thread(currentThread);

		// pop arguments off the stack
		refStack += dj_di_methodImplementation_getReferenceArgumentCount(methodImpl) + ((dj_di_methodImplementation_getFlags(methodImpl)&FLAGS_STATIC)?0:1);
		intStack -= dj_di_methodImplementation_getIntegerArgumentCount(methodImpl);
	}

#ifdef DARJEELING_DEBUG_TRACE
	callDepth--;
#endif

#ifdef DARJEELING_DEBUG_MEM_TRACE
		dj_mem_dumpMemUsage();
#endif

}

/**
 * Convenience method for throwing system library exceptions. It creates an exception object and throws it at the current PC.
 * For throwing exceptions that are not in the System class, create the object manually and throw it with dj_exec_throwHere.
 * @param exceptionId the class entity ID of the exception to throw. The exception class is assumed to be in the system library.
 */
void dj_exec_createAndThrow(int exceptionId)
{
	dj_object *obj = dj_vm_createSysLibObject(vm, exceptionId);

	// if we can't allocate the exception, we're really out of memory :(
	// throw the last resort panic exception object we pre-allocated
	if (obj==NULL)
		obj = dj_mem_getPanicExceptionObject();

	// in case we didn't preallocate an exception object for this corner case, simply panic
	if (obj==NULL)
		dj_panic(DJ_PANIC_OUT_OF_MEMORY);

	dj_exec_throwHere(obj);
}

/**
 * Throws an exception at the current PC. Ideally this function should be called only from outside the execution module.
 * @see dj_exe_throw()
 * @param obj the object to throw
 */
void dj_exec_throwHere(dj_object *obj)
{
	dj_exec_throw(obj, pc);
}

/**
 * Throws an exception at the given PC.
 * @see dj_exe_throw_here()
 * @param obj the object to throw
 * @param throw_pc the address at which the exception ocurred.
 */

//#include "nesc.h"

void dj_exec_throw(dj_object *obj, uint16_t throw_pc)
{
	uint8_t i;
	dj_di_pointer method;
	char caught = 0, type_applies;
	dj_local_id catch_type_local_id;
	dj_global_id catch_type;

	// get runtime class
	runtime_id_t classRuntimeId = dj_mem_getChunkId(obj);
	dj_global_id classGlobalId = dj_vm_getRuntimeClass(vm, classRuntimeId);

	DEBUG_LOG("Throwing exception at pc=%d, object entity id=%d\n", pc, dj_mem_getChunkId(obj));

//	char temp[32];
//	snprintf(temp, 32, "%d, %d\n", dj_vm_getInfusionId(dj_exec_getVM(), classGlobalId.infusion), classGlobalId.entity_id);
//	nesc_printf(temp);

	throw_pc = pc;
	while ( !caught && currentThread->frameStack!=NULL )
	{
		method = dj_global_id_getMethodImplementation(currentThread->frameStack->method);

		// loop through the exception handlers to try and find an appropriate one
		uint8_t nr_handlers = dj_di_methodImplementation_getNrExceptionHandlers(method);
		for (i=0; (i<nr_handlers)&&(caught==0); i++)
		{
			catch_type_local_id = dj_di_methodImplementation_getExceptionHandlerType(method, i);

			DEBUG_LOG("Testing handler pc=%d, start=%d, end=%d, object entity id=%d, handler type=%d\n",
					pc,
					dj_di_methodImplementation_getExceptionHandlerStartPC(method, i),
					dj_di_methodImplementation_getExceptionHandlerEndPC(method, i),
					dj_mem_getChunkId(obj),
					dj_global_id_resolve(dj_exec_getCurrentInfusion(), catch_type_local_id).entity_id
					);

			// If the infusion_id of the catch type equals 255, the catch block applies for every type.
			// This special case implements the finally block, as per JVM spec version 2, section 7.13.
			if (catch_type_local_id.infusion_id==255)
			{
				type_applies = 1;
			} else
			{
				catch_type = dj_global_id_resolve(dj_exec_getCurrentInfusion(), catch_type_local_id);
				type_applies = dj_global_id_testClassType(classGlobalId, catch_type);
			}

			// check if this handler applies
			if ( (type_applies) &&
                 (throw_pc>=dj_di_methodImplementation_getExceptionHandlerStartPC(method, i)) &&
                 (throw_pc<=dj_di_methodImplementation_getExceptionHandlerEndPC(method, i)) )
			{
				// handler found, jump to catch adress
				pc = dj_di_methodImplementation_getExceptionHandlerCatchPC(method, i);

				// TODO is this correct?
				// pop all operands from the integer and reference stacks
				intStack = (int16_t*) dj_frame_stackStartOffset(currentThread->frameStack);
				refStack = (ref_t*) dj_frame_stackEndOffset(currentThread->frameStack);

				pushRef(VOIDP_TO_REF(obj));
				caught=1;
			}
		}

		// if not caught, destroy the current frame and go into the next on the stack
		if (!caught) {

			dj_frame_destroy(dj_thread_popFrame(currentThread));

			if (currentThread->frameStack!=NULL)
				// perform context switch
				dj_exec_activate_thread(currentThread);

			throw_pc = pc;
		}

	}

	// if the exception was not caught, terminate the thread
	if (!caught)
	{
		currentThread->status=THREADSTATUS_FINISHED;
		dj_exec_breakExecution();

		// GS-07/10/2008-14:27(AEST)  while   I  agree  that  silently
        // terminating the thread is technically a correct beahaviour,
        // I  think that  at this  point, having  an  explicit failure
        // would be more useful
		// printf("Uncaught exception[%d]\n", class_global_id.entity_id);
		DEBUG_LOG("Uncaught exception[%d]\n", classGlobalId.entity_id);
        // dj_panic(DJ_PANIC_UNCAUGHT_EXCEPTION);
	}
}

#include "field_instructions.h"
#include "array_instructions.h"
#include "branch_instructions.h"
#include "invoke_instructions.h"
#include "misc_instructions.h"

#define INT_ARITHMETIC_OP(op) do { temp2 = popInt(); \
        temp1 = popInt();                        \
        pushInt(temp1 op temp2); } while(0)

#define SHORT_ARITHMETIC_OP(op) do { temp2 = popShort(); \
        temp1 = popShort();                        \
        pushShort((int16_t)(temp1 op temp2)); } while(0)

/**
 * The execution engine's main run function. Executes [nrOpcodes] instructions, or until execution is stopped explicitly.
 * @param nrOpcodes the amount of opcodes to execute in one 'run'.
 */
int dj_exec_run(int nrOpcodes)
{
#ifdef DARJEELING_DEBUG_TRACE
	int oldCallDepth = callDepth;
#endif

	uint8_t opcode, m, n;
	int i;
	nrOpcodesLeft = nrOpcodes;
	int32_t temp1, temp2, temp3;
	ref_t rtemp1, rtemp2, rtemp3;

	while (nrOpcodesLeft>0)
	{
		nrOpcodesLeft--;

		opcode=fetch();

#ifdef DARJEELING_DEBUG
		totalNrOpcodes++;
		oldPc = pc;
#endif

		switch(opcode)
		{
			// arithmetic
			case JVM_IADD: INT_ARITHMETIC_OP(+); break;
			case JVM_ISUB: INT_ARITHMETIC_OP(-); break;
			case JVM_IMUL: INT_ARITHMETIC_OP(*); break;
			case JVM_IDIV: INT_ARITHMETIC_OP(/); break;
			case JVM_INEG: pushInt(-popInt()); break;
			case JVM_ISHR: INT_ARITHMETIC_OP(>>); break;
			case JVM_IUSHR:
				temp2 = popInt();
				temp1 = popInt();
				pushInt( ((uint32_t)temp1) >> temp2 );
				break;
			case JVM_ISHL: INT_ARITHMETIC_OP(<<); break;
			case JVM_IREM: INT_ARITHMETIC_OP(%); break;
			case JVM_IAND: INT_ARITHMETIC_OP(&); break;
			case JVM_IOR: INT_ARITHMETIC_OP(|); break;
			case JVM_IXOR: INT_ARITHMETIC_OP(^); break;

			// arithmetic
			case JVM_SADD: SHORT_ARITHMETIC_OP(+); break;
			case JVM_SSUB: SHORT_ARITHMETIC_OP(-); break;
			case JVM_SMUL: SHORT_ARITHMETIC_OP(*); break;
			case JVM_SDIV: SHORT_ARITHMETIC_OP(/); break;
			case JVM_SNEG: pushShort(-popShort()); break;
			case JVM_SSHR: SHORT_ARITHMETIC_OP(>>); break;
			case JVM_SUSHR:
				temp2 = popShort();
				temp1 = popShort();
				pushShort( ((uint16_t)temp1) >> temp2 );
				break;
			case JVM_SSHL: SHORT_ARITHMETIC_OP(<<); break;
			case JVM_SREM: SHORT_ARITHMETIC_OP(%); break;
			case JVM_SAND: SHORT_ARITHMETIC_OP(&); break;
			case JVM_SOR: SHORT_ARITHMETIC_OP(|); break;
			case JVM_SXOR: SHORT_ARITHMETIC_OP(^); break;

			// TODO use peekInt/pokeInt
			case JVM_I2B: pushShort((int8_t)popInt()); break;
            case JVM_I2C: pushShort((int8_t)popInt()); break;
            case JVM_I2S: pushShort((int16_t)popInt()); break;

            case JVM_S2B: pushShort((int8_t)popShort()); break;
            case JVM_S2C: pushShort((int8_t)popShort()); break;
            case JVM_S2I: pushInt((int32_t)popShort()); break;

            case JVM_B2C: // TODO keep this opcode?
            	break;

            case JVM_IINC:
                temp1 = fetch();
                temp2 = (int8_t)fetch();
                setLocalInt(temp1, getLocalInt(temp1) + temp2);
                break;

            case JVM_IINC_W:
                temp1 = fetch();
                temp2 = (int16_t)fetch16();
                setLocalInt(temp1, getLocalInt(temp1) + temp2);
                break;

            case JVM_SINC:
                temp1 = fetch();
                temp2 = (int8_t)fetch();
                setLocalShort(temp1, getLocalShort(temp1) + temp2);
                break;

            case JVM_SINC_W:
                temp1 = fetch();
                temp2 = (int16_t)fetch16();
                setLocalShort(temp1, getLocalShort(temp1) + temp2);
                break;

                // stack and local variables
            case JVM_ICONST_M1: pushInt(-1); break;
            case JVM_ICONST_0: pushInt(0); break;
            case JVM_ICONST_1: pushInt(1); break;
            case JVM_ICONST_2: pushInt(2); break;
            case JVM_ICONST_3: pushInt(3); break;
            case JVM_ICONST_4: pushInt(4); break;
            case JVM_ICONST_5: pushInt(5); break;

            case JVM_SCONST_M1: pushShort(-1); break;
            case JVM_SCONST_0: pushShort(0); break;
            case JVM_SCONST_1: pushShort(1); break;
            case JVM_SCONST_2: pushShort(2); break;
            case JVM_SCONST_3: pushShort(3); break;
            case JVM_SCONST_4: pushShort(4); break;
            case JVM_SCONST_5: pushShort(5); break;

            case JVM_BIPUSH: pushInt((int8_t)fetch()); break;
            case JVM_BSPUSH: pushShort((int8_t)fetch()); break;
            case JVM_SIPUSH: pushInt((int16_t)fetch16()); break;
            case JVM_SSPUSH: pushShort((int16_t)fetch16()); break;
            case JVM_IIPUSH: pushInt((int32_t)fetch32()); break;

            case JVM_LDS: LDS(); break;

            case JVM_ISTORE: setLocalInt(fetch(), popInt()); break;
            case JVM_ISTORE_0: setLocalInt(0, popInt()); break;
            case JVM_ISTORE_1: setLocalInt(1, popInt()); break;
            case JVM_ISTORE_2: setLocalInt(2, popInt()); break;
            case JVM_ISTORE_3: setLocalInt(3, popInt()); break;

            case JVM_ILOAD: pushInt(getLocalInt(fetch())); break;
            case JVM_ILOAD_0: pushInt(getLocalInt(0)); break;
            case JVM_ILOAD_1: pushInt(getLocalInt(1)); break;
            case JVM_ILOAD_2: pushInt(getLocalInt(2)); break;
            case JVM_ILOAD_3: pushInt(getLocalInt(3)); break;

            case JVM_SSTORE: setLocalShort(fetch(), popShort()); break;
            case JVM_SSTORE_0: setLocalShort(0, popShort()); break;
            case JVM_SSTORE_1: setLocalShort(1, popShort()); break;
            case JVM_SSTORE_2: setLocalShort(2, popShort()); break;
            case JVM_SSTORE_3: setLocalShort(3, popShort()); break;

            case JVM_SLOAD: pushShort(getLocalShort(fetch())); break;
            case JVM_SLOAD_0: pushShort(getLocalShort(0)); break;
            case JVM_SLOAD_1: pushShort(getLocalShort(1)); break;
            case JVM_SLOAD_2: pushShort(getLocalShort(2)); break;
            case JVM_SLOAD_3: pushShort(getLocalShort(3)); break;

            case JVM_ACONST_NULL: pushRef(nullref); break;

            case JVM_ALOAD: pushRef(getLocalRef(fetch())); break;
            case JVM_ALOAD_0: pushRef(getLocalRef(0)); break;
            case JVM_ALOAD_1: pushRef(getLocalRef(1)); break;
            case JVM_ALOAD_2: pushRef(getLocalRef(2)); break;
            case JVM_ALOAD_3: pushRef(getLocalRef(3)); break;

            case JVM_ASTORE: setLocalRef(fetch(), popRef()); break;
            case JVM_ASTORE_0: setLocalRef(0, popRef()); break;
            case JVM_ASTORE_1: setLocalRef(1, popRef()); break;
            case JVM_ASTORE_2: setLocalRef(2, popRef()); break;
            case JVM_ASTORE_3: setLocalRef(3, popRef()); break;

            // Integer stack operations
            case JVM_IPOP:
                intStack--;
                break;

            case JVM_IPOP2:
                intStack-=2;
                break;

            case JVM_IDUP:
                *intStack=*(intStack-1);
                intStack++;
                break;

            case JVM_IDUP2:
                *(intStack+1)=*(intStack-1);
                *(intStack  )=*(intStack-2);
                intStack+=2;
                break;

            case JVM_IDUP_X:
            	m = fetch();
            	n = m&15;
            	m>>=4;

            	// reserve space on the stack for the duplicated data
            	intStack+=m;

            	if (n==0)
            	{
            		// perform normal dup
                	for (i=0; i<m; i++)
                		intStack[-i-m-1]=intStack[-i-1];

            	} else
            	{
                	// move existing stuff forwards
                	for (i=0; i<n; i++)
                		intStack[-i-1]=intStack[-i-m-1];

                	// copy duplicated value into place
                	for (i=0; i<m; i++)
                		intStack[-i-n-1]=intStack[-i-1];
            	}

                break;

            // TODO make faster
            case JVM_IDUP_X1:
                temp1 = popShort();
                temp2 = popShort();
                pushShort(temp1);
                pushShort(temp2);
                pushShort(temp1);
                break;

            // TODO make faster
            case JVM_IDUP_X2:
                temp1 = popShort();
                temp2 = popShort();
                temp3 = popShort();
                pushShort(temp1);
                pushShort(temp3);
                pushShort(temp2);
                pushShort(temp1);
                break;

			// Reference stack operations
			case JVM_APOP:
				refStack++;
				break;

			case JVM_APOP2:
				refStack+=2;
				break;

			case JVM_ADUP:
				refStack--;
				*refStack = *(refStack+1);
				break;

			case JVM_ADUP2:
				refStack-=2;
				*(refStack  ) = *(refStack+2);
				*(refStack+1) = *(refStack+3);
				break;

            // TODO make faster
            case JVM_ADUP_X1:
                rtemp1 = popRef();
                rtemp2 = popRef();
                pushRef(rtemp1);
                pushRef(rtemp2);
                pushRef(rtemp1);
                break;

            // TODO make faster
            case JVM_ADUP_X2:
                rtemp1 = popRef();
                rtemp2 = popRef();
                rtemp3 = popRef();
                pushRef(rtemp1);
                pushRef(rtemp3);
                pushRef(rtemp2);
                pushRef(rtemp1);
                break;

                // program flow
            case JVM_GOTO: GOTO(); break;

            case JVM_IF_ICMPEQ: IF_ICMPEQ(); break;
            case JVM_IF_ICMPNE: IF_ICMPNE(); break;
            case JVM_IF_ICMPLT: IF_ICMPLT(); break;
            case JVM_IF_ICMPGE: IF_ICMPGE(); break;
            case JVM_IF_ICMPGT: IF_ICMPGT(); break;
            case JVM_IF_ICMPLE: IF_ICMPLE(); break;

            case JVM_IF_SCMPEQ: IF_SCMPEQ(); break;
            case JVM_IF_SCMPNE: IF_SCMPNE(); break;
            case JVM_IF_SCMPLT: IF_SCMPLT(); break;
            case JVM_IF_SCMPGE: IF_SCMPGE(); break;
            case JVM_IF_SCMPGT: IF_SCMPGT(); break;
            case JVM_IF_SCMPLE: IF_SCMPLE(); break;

            case JVM_IIFEQ: IIFEQ(); break;
            case JVM_IIFNE: IIFNE(); break;
            case JVM_IIFLT: IIFLT(); break;
            case JVM_IIFGE: IIFGE(); break;
            case JVM_IIFGT: IIFGT(); break;
            case JVM_IIFLE: IIFLE(); break;

            case JVM_SIFEQ: SIFEQ(); break;
            case JVM_SIFNE: SIFNE(); break;
            case JVM_SIFLT: SIFLT(); break;
            case JVM_SIFGE: SIFGE(); break;
            case JVM_SIFGT: SIFGT(); break;
            case JVM_SIFLE: SIFLE(); break;

            case JVM_IF_ACMPEQ: IF_ACMPEQ(); break;
            case JVM_IF_ACMPNE: IF_ACMPNE(); break;
            case JVM_IFNULL: IFNULL(); break;
            case JVM_IFNONNULL: IFNONNULL(); break;

            case JVM_RETURN: RETURN(); break;
            case JVM_IRETURN: IRETURN(); break;
            case JVM_SRETURN: SRETURN(); break;
            case JVM_ARETURN: ARETURN(); break;
            case JVM_INVOKESTATIC: INVOKESTATIC(); break;
            case JVM_INVOKESPECIAL: INVOKESPECIAL(); break;
            case JVM_INVOKEVIRTUAL: INVOKEVIRTUAL(); break;
            case JVM_INVOKEINTERFACE: INVOKEINTERFACE(); break;

                // monitors
            case JVM_MONITORENTER: MONITORENTER(); break;
            case JVM_MONITOREXIT: MONITOREXIT(); break;

                // arrays and classes
            case JVM_NEW: NEW(); break;
            case JVM_INSTANCEOF: INSTANCEOF(); break;
            case JVM_CHECKCAST: CHECKCAST(); break;

            case JVM_NEWARRAY: NEWARRAY(); break;
            case JVM_ANEWARRAY: ANEWARRAY(); break;
            case JVM_ARRAYLENGTH: ARRAYLENGTH(); break;
            case JVM_BASTORE: BASTORE(); break;
            case JVM_CASTORE: CASTORE(); break;
            case JVM_SASTORE: SASTORE(); break;
            case JVM_IASTORE: IASTORE(); break;
            case JVM_AASTORE: AASTORE(); break;

            case JVM_BALOAD: BALOAD(); break;
            case JVM_CALOAD: CALOAD(); break;
            case JVM_SALOAD: SALOAD(); break;
            case JVM_IALOAD: IALOAD(); break;
            case JVM_AALOAD: AALOAD(); break;

            case JVM_GETSTATIC_I: GETSTATIC_I(); break;
            case JVM_GETSTATIC_A: GETSTATIC_A(); break;
            case JVM_GETSTATIC_B: GETSTATIC_B(); break;
            case JVM_GETSTATIC_C: GETSTATIC_C(); break;
            case JVM_GETSTATIC_S: GETSTATIC_S(); break;

            case JVM_PUTSTATIC_I: PUTSTATIC_I(); break;
            case JVM_PUTSTATIC_A: PUTSTATIC_A(); break;
            case JVM_PUTSTATIC_B: PUTSTATIC_B(); break;
            case JVM_PUTSTATIC_C: PUTSTATIC_C(); break;
            case JVM_PUTSTATIC_S: PUTSTATIC_S(); break;

            case JVM_GETFIELD_I: GETFIELD_I(); break;
            case JVM_GETFIELD_A: GETFIELD_A(); break;
            case JVM_GETFIELD_B: GETFIELD_B(); break;
            case JVM_GETFIELD_C: GETFIELD_C(); break;
            case JVM_GETFIELD_S: GETFIELD_S(); break;

            case JVM_PUTFIELD_I: PUTFIELD_I(); break;
            case JVM_PUTFIELD_A: PUTFIELD_A(); break;
            case JVM_PUTFIELD_B: PUTFIELD_B(); break;
            case JVM_PUTFIELD_C: PUTFIELD_C(); break;
			case JVM_PUTFIELD_S: PUTFIELD_S(); break;

			case JVM_TABLESWITCH: TABLESWITCH(); break;
			case JVM_LOOKUPSWITCH: LOOKUPSWITCH(); break;

            case JVM_ATHROW: ATHROW(); break;

            // misc
            case JVM_NOP: /* do nothing */ break;

            default:
                DEBUG_LOG("Unimplemented opcode %d at pc=%d: %s\n", opcode, oldPc, jvm_opcodes[opcode]);
            	dj_exec_createAndThrow(BASE_CDEF_java_lang_VirtualMachineError);
		}

#ifdef DARJEELING_DEBUG_TRACE


		dj_frame *current_frame = currentThread->frameStack;
		if (current_frame==NULL) continue;

		DEBUG_LOG("%*s%03d->%03d   %-15s",
               oldCallDepth*2,
               "",
               oldPc,
               pc,
               jvm_opcodes[opcode]);

		DEBUG_LOG("R<");

		dj_di_pointer method = dj_global_id_getMethodImplementation(current_frame->method);
		int len = dj_di_methodImplementation_getReferenceLocalVariableCount(method);
		for (i=0; i<len; i++)
			DEBUG_LOG((i==0)?" %-9d ":", %-9d ", localReferenceVariables[i]);

		DEBUG_LOG(">");

		DEBUG_LOG("\tI<");

		len = dj_di_methodImplementation_getIntegerLocalVariableCount(method);
		for (i=0; i<len; i++)
			DEBUG_LOG((i==0)?" %-9d ":", %-9d ", localIntegerVariables[i]);

		DEBUG_LOG(">");
		DEBUG_LOG("\tR(");

		// abuse this method to calculate nr_int_stack and nr_ref_stack for us
		dj_exec_saveLocalState(current_frame);

		ref_t *refStackStart = (ref_t*)((int)dj_frame_getStackEnd(current_frame) - current_frame->nr_ref_stack * sizeof(ref_t));
		for (i=0; i<current_frame->nr_ref_stack; i++)
			DEBUG_LOG("%-6d,", refStackStart[i]);

		DEBUG_LOG(")");
		DEBUG_LOG("\tI(");

		int16_t *intStackStart = dj_frame_getStackStart(current_frame);
		for (i=0; i<current_frame->nr_int_stack; i++)
			DEBUG_LOG("%-6d,", intStackStart[i]);

		DEBUG_LOG(")");

		DEBUG_LOG("\t(%d,%d)", current_frame->nr_ref_stack, current_frame->nr_int_stack);

		DEBUG_LOG("\n");

		oldCallDepth = callDepth;
#endif


	}

	return nrOpcodesLeft;

}
/**
 * @}
 */
