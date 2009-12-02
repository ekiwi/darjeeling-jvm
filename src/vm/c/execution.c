/*
 *	execution.c
 *
 *	Copyright (c) 2008 CSIRO, Delft University of Technology.
 *
 *	This file is part of Darjeeling.
 *
 *	Darjeeling is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Darjeeling is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */

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

#include "execution.h"
#include "types.h"
#include "parse_infusion.h"
#include "infusion.h"
#include "object.h"
#include "array.h"
#include "heap.h"
#include "vm.h"
#include "global_id.h"
#include "debug.h"
#include "panic.h"

// platform-specific configuration
#include "config.h"

// generated at infusion time
#include "jlib_base.h"

// platform specific
#include "pointerwidth.h"

#include "opcodes.c"

// currently selected Virtual Machine context
static dj_vm *vm;

// global variables for quick access
//static dj_thread *currentThread;

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
//if it is tossim we need a bunch of getter setters,
//because tossim considers global variables in all nodes to be shared
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
inline void dj_exec_breakExecution() {
	nrOpcodesLeft = -1;
}

/**
 * Saves execution state (stack pointer, pc) in the given frame struct. This method
 * is used in context switching and method invocations/returns.
 * @param frame a dj_frame struct in which to save the current state
 */
static inline void dj_exec_saveLocalState(dj_frame *frame) {
	frame->pc = pc;
	frame->nr_int_stack = ((char*) intStack - dj_frame_stackStartOffset(frame))
			/ sizeof(int16_t);
	frame->nr_ref_stack = (dj_frame_stackEndOffset(frame) - (char*) refStack)
			/ sizeof(ref_t);

#ifdef ASSEMBLY_DEBUGGING
	ASSEMBLY_DEBUG("\t----------------------------------------------\n");
	ASSEMBLY_DEBUG("\tLocal state saved\n\trefStack\t - \t%p\n\tintStack\t - \t%p\n\tnr_ref_stack\t - \t%d\n\tnr_int_stack\t - \t%d\n\tframe\t\t - \t%p\n", refStack, intStack, frame->nr_ref_stack, frame->nr_int_stack, frame);
	ASSEMBLY_DEBUG("\t----------------------------------------------\n");
#endif
}

/**
 * Loads execution state (stack pointer, pc, code pointer, local variables) from a dj_frame struct. This method
 * is used in context switching and method invocations/returns.
 * @param frame the frame to load
 */
static inline void dj_exec_loadLocalState(dj_frame *frame) {
	// get program counter, stack pointers, code
	dj_di_pointer methodImpl = dj_global_id_getMethodImplementation(
			frame->method);
	code = dj_di_methodImplementation_getData(methodImpl);
	DEBUG_LOG("\told pc %d to current pc %d\n", pc, frame->pc);
	pc = frame->pc;

	intStack = dj_frame_getIntegerStack(frame);
	refStack = dj_frame_getReferenceStack(frame);

	localReferenceVariables = dj_frame_getLocalReferenceVariables(frame);
	localIntegerVariables = dj_frame_getLocalIntegerVariables(frame);

	nrReferenceParameters
			= dj_di_methodImplementation_getReferenceArgumentCount(methodImpl)
					+ ((dj_di_methodImplementation_getFlags(methodImpl)
							& FLAGS_STATIC) ? 0 : 1);
	nrIntegerParameters
			= dj_di_methodImplementation_getIntegerArgumentCount(methodImpl);

	if (frame->parent != NULL) {
		referenceParameters = dj_frame_getReferenceStack(frame->parent)
				+ nrReferenceParameters - 1;
		integerParameters = dj_frame_getIntegerStack(frame->parent)
				- nrIntegerParameters;
		this = nullref;
	} else {
		// Special corner case for the run() method in threads. In this case the method will need to access the implicit
		// 'this' parameter as run() is a virtual method. Usually parameters are accessed directly from the
		// caller stack, but since there is no caller frame we copy the 'this' reference from the thread object to a
		// global variable and wire the callerReferenceStack to point to it. Not very elegant, but it gets the job done.
		// this = VOIDP_TO_REF(currentThread->runnable);
		this = VOIDP_TO_REF(dj_exec_getCurrentThread()->runnable);
		referenceParameters = &this;
		integerParameters = NULL;

	}
#if ASSEMBLY_DEBUGGING
	ASSEMBLY_DEBUG("\t----------------------------------------------\n");
	ASSEMBLY_DEBUG("\tLocal state loaded \n\tmaxStack\t - \t%d\n\trefStack\t - \t%p\n\tintStack\t - \t%p\n\tnr_ref_stack\t - \t%d\n\tnr_int_stack\t - \t%d\n\tframe\t\t - \t%p\n", dj_di_methodImplementation_getMaxStack(dj_global_id_getMethodImplementation(frame->method)), refStack, intStack, frame->nr_ref_stack, frame->nr_int_stack, frame);
	ASSEMBLY_DEBUG("\t----------------------------------------------\n");
#endif

}

/**
 * Deactivates a thread by saving the execution state (program counter, stack, etc) into the top execution frame.
 * This method is used in context switching.
 * @see dj_exec_saveLocalState()
 * @param thread the thread to deactivate
 */
void dj_exec_deactivateThread(dj_thread *thread) {
	// store program counter and stack pointers
	if (thread == NULL)
		return;
	if (thread->frameStack != NULL)
		dj_exec_saveLocalState(thread->frameStack);
}

/**
 * Activates a thread by loading the execution state (program counter, stack, etc) from the top execution frame.
 * This method is used in context switching.
 * @see dj_exec_loadLocalState()
 * @param thread the thread to deactivate
 */
void dj_exec_activate_thread(dj_thread *thread) {

	if (thread == NULL)
		return;
	vm->currentThread = thread;

	if (thread->frameStack != NULL)
		dj_exec_loadLocalState(thread->frameStack);
	else {
		DARJEELING_PRINTF("Thread frame is NULL, thread cannot be activated\n");
		dj_panic(DJ_PANIC_ILLEGAL_INTERNAL_STATE);
	}

}

void dj_exec_updatePointers() {
	vm = dj_mem_getUpdatedPointer(vm);
	this = dj_mem_getUpdatedReference(this);
}

/**
 * Returns the current infusion which is the infusion containing the method that's currently executing. This infusion
 * serves as a context for instructions, as any local ID's contained in them are relative to it.
 * @return the infusion containing the method that's currently executing
 */
dj_infusion * dj_exec_getCurrentInfusion() {
	DEBUG_LOG("\tCurrent infusion is being read from %p->%p\n", dj_exec_getCurrentThread(), dj_exec_getCurrentThread()->frameStack);
	return dj_exec_getCurrentThread()->frameStack->method.infusion;
}

/**
 * Returns the thread that the execution module is currently busy executing.
 */
dj_thread *dj_exec_getCurrentThread() {
	return vm->currentThread;
}

/**
 * Fetches a byte from the code pointer. Increases the PC by 1.
 * TODO make quicker using code++
 */
static inline uint8_t fetch() {
	uint8_t bytecode = dj_di_getU8(code + pc);
	pc++;
	return bytecode;
}

/**
 * Peeks a 16-bit value from the code area, doesn't increase the PC.
 */
static inline int16_t peek16() {
	return (dj_di_getU8(code + pc) << 8) | dj_di_getU8(code + pc + 1);
}

/**
 * Peeks a 16-bit value from the code area at PC+n, doesn't increase the PC.
 * @param n the offset in bytes from the current PC at which to peek
 */
static inline int16_t peekn16(int n) {
	return (dj_di_getU8(code + pc + n) << 8) | dj_di_getU8(code + pc + 1 + n);
}

/**
 * Fetches a 16-bit value from the code area and increases the PC by 2.
 */
static inline int16_t fetch16() {
	// TODO speed up using getU16
	uint16_t ret = (dj_di_getU8(code + pc) << 8) | dj_di_getU8(code + pc + 1);
	pc += 2;
	return ret;
}

/**
 * Fetches a 32-bit value from the code area and increases the PC by 4.
 */
static inline int32_t fetch32() {
	int32_t ret = ((uint32_t) dj_di_getU8(code + pc) << 24)
			| ((uint32_t) dj_di_getU8(code + pc + 1) << 16)
			| ((uint32_t) dj_di_getU8(code + pc + 2) << 8)
			| (uint32_t) dj_di_getU8(code + pc + 3);

	pc += 4;
	return ret;
}

/**
 * Peeks a 32-bit value from the code area at PC+n, doesn't increase the PC.
 * @param n the offset in bytes from the current PC at which to peek
 */
static inline uint32_t peekn32(int n) {
	return ((uint32_t) dj_di_getU8(code + pc + n) << 24)
			| ((uint32_t) dj_di_getU8(code + pc + n + 1) << 16)
			| ((uint32_t) dj_di_getU8(code + pc + n + 2) << 8)
			| (uint32_t) dj_di_getU8(code + pc + n + 3);
}

/**
 * Fetches a dj_local_id value from the code area and increases the PC by 2.
 */
static inline dj_local_id dj_fetchLocalId() {
	dj_local_id ret;
	ret.infusion_id = fetch();
	ret.entity_id = fetch();
	return ret;
}

/**
 * Pushes an int (32 bit) onto the runtime stack
 */
static inline void pushInt(int32_t value) {
	*((int32_t*) intStack) = value;
	intStack += 2;
}

/**
 * Pushes a short (16 bit) onto the runtime stack
 */
static inline void pushShort(int16_t value) {
	*intStack = value;
	intStack++;
}

/**
 * Pushes a reference onto the runtime stack
 */
static inline void pushRef(ref_t value) {
	refStack--;
	*refStack = value;
}

/**
 * Pops an int (32 bit) from the runtime stack
 */
static inline int32_t popInt() {
	intStack -= 2;
	return *(int32_t*) intStack;
}

/**
 * Pops a short (16 bit) from the runtime stack
 */
static inline int16_t popShort() {
	intStack--;
	return *intStack;
}

/**
 * Pops a reference from the runtime stack
 */
static inline ref_t popRef() {
	ref_t ret = *refStack;
	refStack++;
	return ret;
}

/**
 * Returns the topmost 32 bit integer element on the integer stack, does not change the stackpointer.
 */
static inline int32_t peekInt() {
	return *(int32_t*) (intStack - 2);
}

/**
 * Returns the topmost 16 bit short element on the integer stack, does not change the stackpointer.
 */
static inline int16_t peekShort() {
	return *(intStack - 1);
}

/**
 * Returns the topmost element on the reference stack, does not change the stackpointer.
 */
static inline ref_t peekRef() {
	return *refStack;
}

/**
 * Peeks the reference stack at a specified depth. For example, if the stack equals
 * [a, b, c, d, e, f], peekDeepRef(0) would return 'a' (same as peek()), and peekDeepRef(3) would return 'c'.
 * Does not change the stackpointer
 */
static inline ref_t peekDeepRef(int depth) {
	return *(refStack + depth);
}

/**
 * Pushes a short onto the runtime stack. Can be used by functions outside the execution module to interact with the
 * running program.
 * @param value the integer value to push.
 */
void dj_exec_stackPushShort(int16_t value) {
	pushShort(value);
}

/**
 * Pushes an int onto the runtime stack. Can be used by functions outside the execution module to interact with the
 * running program.
 * @param value the integer value to push.
 */
void dj_exec_stackPushInt(int32_t value) {
	pushInt(value);
}

/**
 * Pushes a reference onto the runtime stack. Can be used by functions outside the execution module to interact with the
 * running program. Corruping the stack can lead to crashes, use with care!
 * @param value the reference value to push.
 */
void dj_exec_stackPushRef(ref_t value) {
	pushRef(value);
}

/**
 * Pops an int from the runtime stack. Can be used by functions outside the execution module to interact with the
 * running program. Corruping the stack can lead to crashes, use with care!
 * @return the top value of the runtime stack.
 */
int32_t dj_exec_stackPopInt() {
	return popInt();
}

/**
 * Pops a short from the runtime stack. Can be used by functions outside the execution module to interact with the
 * running program. Corruping the stack can lead to crashes, use with care!
 * @return the top value of the runtime stack.
 */
int16_t dj_exec_stackPopShort() {
	return popShort();
}

/**
 * Pops a reference from the runtime stack. Can be used by functions outside the execution module to interact with the
 * running program. Corruping the stack can lead to crashes, use with care!
 * @return the top value of the runtime stack.
 */
ref_t dj_exec_stackPopRef() {
	return popRef();
}

/**
 * Returns the top of the stack as a short value, but does not change the stackpointer. Can be used by functions outside
 * the execution module to interact with the running program.
 * @return the top value of the runtime stack.
 */
int16_t dj_exec_stackPeekShort() {
	return peekShort();
}

/**
 * Returns the top of the stack as an integer value, but does not change the stackpointer. Can be used by functions outside
 * the execution module to interact with the running program.
 * @return the top value of the runtime stack.
 */
int32_t dj_exec_stackPeekInt() {
	return peekInt();
}

/**
 * Returns the top of the stack as a reference value, but does not change the stackpointer. Can be used by functions outside
 * the execution module to interact with the running program.
 * @return the top value of the runtime stack.
 */
ref_t dj_exec_stackPeekRef() {
	return peekRef();
}

/**
 * Set local reference variable at index
 * @param index local variable slot number
 * @param value reference value
 */
static inline void setLocalRef(int index, ref_t value) {
	if (index < nrReferenceParameters)
		referenceParameters[-index] = value;
	else
		localReferenceVariables[index - nrReferenceParameters] = value;
}

/**
 * Returns local reference variable at index
 * @param index local variable slot number
 */
static inline ref_t getLocalRef(int index) {
	if (index < nrReferenceParameters)
		return referenceParameters[-index];
	else
		return localReferenceVariables[index - nrReferenceParameters];
}

/**
 * Set local reference variable at index
 * @param index local variable slot number
 * @param value 32 bit integer value
 */
static inline void setLocalInt(int index, int32_t value) {
	if (index < nrIntegerParameters)
		*(int32_t*) (integerParameters + index) = value;
	else
		*(int32_t*) (localIntegerVariables + index - nrIntegerParameters)
				= value;
}
/**
 * Returns 32 bit integer local variable at index
 * @param index local variable slot number
 */
static inline int32_t getLocalInt(int index) {
	if (index < nrIntegerParameters)
		return *(int32_t*) (integerParameters + index);
	else
		return *(int32_t*) (localIntegerVariables + index - nrIntegerParameters);
}

/**
 * Set local reference variable at index
 * @param index local variable slot number
 * @param value 16 bit short value
 */
static inline void setLocalShort(int index, int16_t value) {
	if (index < nrIntegerParameters)
		integerParameters[index] = value;
	else
		localIntegerVariables[index - nrIntegerParameters] = value;
}

/**
 * Returns 16 bit short local variable at index
 * @param index local variable slot number
 */
static inline int16_t getLocalShort(int index) {
	if (index < nrIntegerParameters)
		return integerParameters[index];
	else
		return localIntegerVariables[index - nrIntegerParameters];
}

/**
 * Branches to PC + offset.
 */
static inline void branch(int16_t offset) {
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
static inline void callMethod(dj_global_id methodImplId, int virtualCall) {
	dj_frame *frame;
	dj_native_handler handler;

	// get a pointer in program space to the method implementation block from the method's global id
	dj_di_pointer methodImpl = dj_global_id_getMethodImplementation(
			methodImplId);

	// check if the method is a native methods
	if ((dj_di_methodImplementation_getFlags(methodImpl) & FLAGS_NATIVE) != 0) {
		// the method is native, check if we have a native handler for the infusion the method is in
		handler = methodImplId.infusion->native_handler;
		if (handler != NULL)
			// we can execute the method by calling the infusion's native handler
			handler(methodImplId);
		else{
			// there is no native handler for this method's infusion. Throw an exception
			dj_exec_createAndThrow(
					BASE_CDEF_javax_darjeeling_vm_NativeMethodNotImplementedError);
		}

	} else {
		// create new frame for the function
		frame = dj_frame_create(methodImplId);

		// not enough space on the heap to allocate the frame
		if (frame == NULL) {
			dj_exec_createAndThrow(BASE_CDEF_java_lang_StackOverflowError);
			return;
		}

		// save current state of the frame
		dj_exec_saveLocalState(dj_exec_getCurrentThread()->frameStack);

		// push the new frame on the frame stack
		dj_thread_pushFrame(dj_exec_getCurrentThread(), frame);

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
static inline void returnFromMethod() {
	dj_di_pointer methodImpl;

	// get the method from the stack frame so we can calculate how many parameters to pop off the operand stack
	methodImpl = dj_global_id_getMethodImplementation(
			dj_exec_getCurrentThread()->frameStack->method);

	// pop frame from frame stack and dealloc it
	dj_frame_destroy(dj_thread_popFrame(dj_exec_getCurrentThread()));

	// check if there are elements on the call stack
	if (dj_exec_getCurrentThread()->frameStack == NULL) {
		// done executing (exited last element on the call stack)
		dj_exec_getCurrentThread()->status = THREADSTATUS_FINISHED;
		dj_exec_breakExecution();
	} else {
		// perform context switch.
		dj_exec_activate_thread(dj_exec_getCurrentThread());

		// pop arguments off the stack
		refStack
				+= dj_di_methodImplementation_getReferenceArgumentCount(methodImpl)
						+ ((dj_di_methodImplementation_getFlags(methodImpl)
								& FLAGS_STATIC) ? 0 : 1);
		intStack
				-= dj_di_methodImplementation_getIntegerArgumentCount(methodImpl);
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
void dj_exec_createAndThrow(int exceptionId) {
	dj_object *obj = dj_vm_createSysLibObject(vm, exceptionId);
	// if we can't allocate the exception, we're really out of memory :(
	// throw the last resort panic exception object we pre-allocated
	if (obj == NULL)
		obj = dj_mem_getPanicExceptionObject();

	// in case we didn't preallocate an exception object for this corner case, simply panic
	if (obj == NULL) {
		DARJEELING_PRINTF("Not enough space to create an exception\n");
		dj_panic(DJ_PANIC_OUT_OF_MEMORY);
	}

	dj_exec_throwHere(obj);
}

/**
 * Throws an exception at the current PC. Ideally this function should be called only from outside the execution module.
 * @see dj_exe_throw()
 * @param obj the object to throw
 */
void dj_exec_throwHere(dj_object *obj) {
	dj_exec_throw(obj, pc);
}

#ifdef IS_SIMULATOR
char *getExceptionName(int exception_id)
{
	switch(exception_id){
	case BASE_CDEF_java_lang_ArrayStoreException :
	 return "Array store exception";
	case BASE_CDEF_java_lang_ClassCastException :
	 return "Class cast exception";
	case BASE_CDEF_java_lang_Error :
	 return "Error";
	case BASE_CDEF_java_lang_Exception :
	 return "General exception";
	case BASE_CDEF_java_lang_IllegalArgumentException :
	 return "Illegal argument exception";
	case BASE_CDEF_java_lang_IllegalThreadStateException :
	 return "Illegal thread exception";
	case BASE_CDEF_java_lang_IndexOutOfBoundsException :
	 return "Index out of bound exception";
	case BASE_CDEF_java_lang_NullPointerException :
	 return "Null pointer exception";
	case BASE_CDEF_java_lang_OutOfMemoryError :
	 return "Out of memory exception";
	case BASE_CDEF_java_lang_RuntimeException :
	 return "Runtime exception";
	case BASE_CDEF_java_lang_StackOverflowError :
	 return "Stack overflow error";
	case BASE_CDEF_java_lang_VirtualMachineError :
	 return "Virtual machine error";
	case BASE_CDEF_java_util_NoSuchElementException :
	 return "No such element exception";
	case BASE_CDEF_javax_darjeeling_vm_ClassUnloadedException :
	 return "Class unloaded exception";
	case BASE_CDEF_javax_darjeeling_vm_InfusionUnloadDependencyException :
	 return "Infusion unload dependency exception";
	case BASE_CDEF_javax_darjeeling_vm_NativeMethodNotImplementedError :
	 return "Native method not implemented error";
	default:
		return "Unknown exception";
	}
}
#endif
/**
 * Throws an exception at the given PC.
 * @see dj_exe_throw_here()
 * @param obj the object to throw
 * @param throw_pc the address at which the exception ocurred.
 */

void dj_exec_throw(dj_object *obj, uint16_t throw_pc) {
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
	while (!caught && dj_exec_getCurrentThread()->frameStack != NULL) {
		method = dj_global_id_getMethodImplementation(
				dj_exec_getCurrentThread()->frameStack->method);

		// loop through the exception handlers to try and find an appropriate one
		uint8_t nr_handlers =
				dj_di_methodImplementation_getNrExceptionHandlers(method);
		for (i = 0; (i < nr_handlers) && (caught == 0); i++) {
			catch_type_local_id
					= dj_di_methodImplementation_getExceptionHandlerType(method, i);

			DEBUG_LOG("Testing handler pc=%d, start=%d, end=%d, object entity id=%d, handler type=%d\n",
					pc,
					dj_di_methodImplementation_getExceptionHandlerStartPC(method, i),
					dj_di_methodImplementation_getExceptionHandlerEndPC(method, i),
					dj_mem_getChunkId(obj),
					dj_global_id_resolve(dj_exec_getCurrentInfusion(), catch_type_local_id).entity_id
			);

			// If the infusion_id of the catch type equals 255, the catch block applies for every type.
			// This special case implements the finally block, as per JVM spec version 2, section 7.13.
			if (catch_type_local_id.infusion_id == 255) {
				type_applies = 1;
			} else {
				catch_type = dj_global_id_resolve(dj_exec_getCurrentInfusion(),
						catch_type_local_id);
				type_applies = dj_global_id_testClassType(classGlobalId,
						catch_type);
			}

			// check if this handler applies
			if ((type_applies)
					&& (throw_pc
							>=dj_di_methodImplementation_getExceptionHandlerStartPC(method, i))
					&& (throw_pc
							<=dj_di_methodImplementation_getExceptionHandlerEndPC(method, i))) {
				// handler found, jump to catch adress
				pc
						= dj_di_methodImplementation_getExceptionHandlerCatchPC(method, i);

				// TODO is this correct?
				// pop all operands from the integer and reference stacks
				intStack
						= (int16_t*) dj_frame_stackStartOffset(dj_exec_getCurrentThread()->frameStack);
				refStack
						= (ref_t*) dj_frame_stackEndOffset(dj_exec_getCurrentThread()->frameStack);

				pushRef(VOIDP_TO_REF(obj));
				caught = 1;
			}
		}

		// if not caught, destroy the current frame and go into the next on the stack
		if (!caught) {

			dj_frame_destroy(dj_thread_popFrame(dj_exec_getCurrentThread()));

			if (dj_exec_getCurrentThread()->frameStack != NULL)
				// perform context switch
				dj_exec_activate_thread(dj_exec_getCurrentThread());

			throw_pc = pc;
		}

	}

	// if the exception was not caught, terminate the thread
	if (!caught) {
		dj_exec_getCurrentThread()->status = THREADSTATUS_FINISHED;
		dj_exec_breakExecution();

		// GS-07/10/2008-14:27(AEST)  while   I  agree  that  silently
		// terminating the thread is technically a correct beahaviour,
		// I  think that  at this  point, having  an  explicit failure
		// would be more useful
		// printf("Uncaught exception[%d]\n", classid.entity_id);
#ifdef IS_SIMULATOR
		DARJEELING_PRINTF("Uncaught exception[%d] - %c[31m%s%c[0m\n", classGlobalId.entity_id, 0x1b, getExceptionName(classGlobalId.entity_id), 0x1b );
#endif
		dj_panic(DJ_PANIC_UNCAUGHT_EXCEPTION);
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
int dj_exec_run(int nrOpcodes) {
#ifdef DARJEELING_DEBUG_TRACE
	int oldCallDepth = callDepth;
#endif

	uint8_t opcode, m, n;
	int i;
	nrOpcodesLeft = nrOpcodes;
	int32_t temp1, temp2, temp3;
	ref_t rtemp1, rtemp2, rtemp3;

	while (nrOpcodesLeft > 0) {
		nrOpcodesLeft--;
		opcode = fetch();

#ifdef ASSEMBLY_DEBUGGING
//		DARJEELING_PRINTF("_%d(pc=%d)\n" , opcode, pc);
		ASSEMBLY_DEBUG("=====\tOPCODE %d\t=====\n" , opcode);
#endif
#ifdef DARJEELING_DEBUG
		totalNrOpcodes++;
		oldPc = pc;
#endif

		switch (opcode) {
		// arithmetic
		case JVM_IADD:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IADD\t=====\n");
#endif
			INT_ARITHMETIC_OP(+);
			break;
		case JVM_ISUB:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ISUB\t=====\n");
#endif
			INT_ARITHMETIC_OP(-);
			break;
		case JVM_IMUL:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IMUL\t=====\n");
#endif
			INT_ARITHMETIC_OP(*);
			break;
		case JVM_IDIV:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IDIV\t=====\n");
#endif
			INT_ARITHMETIC_OP(/);
			break;
		case JVM_INEG:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_INEG\t=====\n");
#endif
			pushInt(-popInt());
			break;
		case JVM_ISHR:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ISHR\t=====\n");
#endif
			INT_ARITHMETIC_OP(>>);
			break;
		case JVM_IUSHR:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IUSHR\t=====\n");
#endif

			temp2 = popInt();
			temp1 = popInt();
			pushInt(((uint32_t) temp1) >> temp2);
			break;
		case JVM_ISHL:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ISHL\t=====\n");
#endif
			INT_ARITHMETIC_OP(<<);
			break;
		case JVM_IREM:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IREM\t=====\n");
#endif
			INT_ARITHMETIC_OP(%);
			break;
		case JVM_IAND:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IAND\t=====\n");
#endif
			INT_ARITHMETIC_OP(&);
			break;
		case JVM_IOR:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IOR\t=====\n");
#endif
			INT_ARITHMETIC_OP(|);
			break;
		case JVM_IXOR:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IXOR\t=====\n");
#endif
			INT_ARITHMETIC_OP(^);
			break;

			// arithmetic
		case JVM_SADD:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SADD\t=====\n");
#endif
			SHORT_ARITHMETIC_OP(+);
			break;
		case JVM_SSUB:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SSUB\t=====\n");
#endif
			SHORT_ARITHMETIC_OP(-);
			break;
		case JVM_SMUL:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SMUL\t=====\n");
#endif
			SHORT_ARITHMETIC_OP(*);
			break;
		case JVM_SDIV:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SDIV\t=====\n");
#endif
			SHORT_ARITHMETIC_OP(/);
			break;
		case JVM_SNEG:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SNEG\t=====\n");
#endif
			pushShort(-popShort());
			break;
		case JVM_SSHR:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SSHR\t=====\n");
#endif
			SHORT_ARITHMETIC_OP(>>);
			break;
		case JVM_SUSHR:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SUSHR\t=====\n");
#endif

			temp2 = popShort();
			temp1 = popShort();
			pushShort(((uint16_t) temp1) >> temp2);
			break;
		case JVM_SSHL:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SSHL\t=====\n");
#endif
			SHORT_ARITHMETIC_OP(<<);
			break;
		case JVM_SREM:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SREM\t=====\n");
#endif
			SHORT_ARITHMETIC_OP(%);
			break;
		case JVM_SAND:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SAND\t=====\n");
#endif
			SHORT_ARITHMETIC_OP(&);
			break;
		case JVM_SOR:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SOR\t=====\n");
#endif
			SHORT_ARITHMETIC_OP(|);
			break;
		case JVM_SXOR:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SXOR\t=====\n");
#endif
			SHORT_ARITHMETIC_OP(^);
			break;

			// TODO use peekInt/pokeInt
		case JVM_I2B:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_I2B\t=====\n");
#endif
			pushShort((int8_t) popInt());
			break;
		case JVM_I2C:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_I2C\t=====\n");
#endif
			pushShort((int8_t) popInt());
			break;
		case JVM_I2S:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_I2S\t=====\n");
#endif
			pushShort((int16_t) popInt());
			break;

		case JVM_S2B:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_S2B\t=====\n");
#endif
			pushShort((int8_t) popShort());
			break;
		case JVM_S2C:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_S2C\t=====\n");
#endif
			pushShort((int8_t) popShort());
			break;
		case JVM_S2I:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_S2I\t=====\n");
#endif
			pushInt((int32_t) popShort());
			break;

		case JVM_B2C:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_B2C\t=====\n");
#endif
			// TODO keep this opcode?
			break;

		case JVM_IINC:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IINC\t=====\n");
#endif

			temp1 = fetch();
			temp2 = (int8_t) fetch();
			setLocalInt(temp1, getLocalInt(temp1) + temp2);
			break;

		case JVM_IINC_W:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IINC_W\t=====\n");
#endif

			temp1 = fetch();
			temp2 = (int16_t) fetch16();
			setLocalInt(temp1, getLocalInt(temp1) + temp2);
			break;

		case JVM_SINC:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SINC\t=====\n");
#endif

			temp1 = fetch();
			temp2 = (int8_t) fetch();
			setLocalShort(temp1, getLocalShort(temp1) + temp2);
			break;

		case JVM_SINC_W:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SINC_W\t=====\n");
#endif

			temp1 = fetch();
			temp2 = (int16_t) fetch16();
			setLocalShort(temp1, getLocalShort(temp1) + temp2);
			break;

			// stack and local variables
		case JVM_ICONST_M1:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_\t=====\n");
#endif
			pushInt(-1);
			break;
		case JVM_ICONST_0:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ICONST_0\t=====\n");
#endif
			pushInt(0);
			break;
		case JVM_ICONST_1:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ICONST_1\t=====\n");
#endif
			pushInt(1);
			break;
		case JVM_ICONST_2:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ICONST_2\t=====\n");
#endif
			pushInt(2);
			break;
		case JVM_ICONST_3:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ICONST_3\t=====\n");
#endif
			pushInt(3);
			break;
		case JVM_ICONST_4:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ICONST_4\t=====\n");
#endif
			pushInt(4);
			break;
		case JVM_ICONST_5:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ICONST_5\t=====\n");
#endif
			pushInt(5);
			break;

		case JVM_SCONST_M1:
			pushShort(-1);
			break;
		case JVM_SCONST_0:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SCONST_0\t=====\n");
#endif
			pushShort(0);
			break;
		case JVM_SCONST_1:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SCONST_1\t=====\n");
#endif
			pushShort(1);
			break;
		case JVM_SCONST_2:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SCONST_2\t=====\n");
#endif
			pushShort(2);
			break;
		case JVM_SCONST_3:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SCONST_3\t=====\n");
#endif
			pushShort(3);
			break;
		case JVM_SCONST_4:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SCONST_4\t=====\n");
#endif
			pushShort(4);
			break;
		case JVM_SCONST_5:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SCONST_5\t=====\n");
#endif
			pushShort(5);
			break;

		case JVM_BIPUSH:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_BIPUSH\t=====\n");
#endif
			pushInt((int8_t) fetch());
			break;
		case JVM_BSPUSH:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_BSPUSH\t=====\n");
#endif
			pushShort((int8_t) fetch());
			break;
		case JVM_SIPUSH:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SIPUSH\t=====\n");
#endif
			pushInt((int16_t) fetch16());
			break;
		case JVM_SSPUSH:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SSPUSH\t=====\n");
#endif
			pushShort((int16_t) fetch16());
			break;
		case JVM_IIPUSH:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IIPUSH\t=====\n");
#endif
			pushInt((int32_t) fetch32());
			break;

		case JVM_LDS:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_LDS\t=====\n");
#endif
			LDS();
			break;

		case JVM_ISTORE:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ISTORE\t=====\n");
#endif
			setLocalInt(fetch(), popInt());
			break;
		case JVM_ISTORE_0:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ISTORE_0\t=====\n");
#endif
			setLocalInt(0, popInt());
			break;
		case JVM_ISTORE_1:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ISTORE_1\t=====\n");
#endif
			setLocalInt(1, popInt());
			break;
		case JVM_ISTORE_2:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ISTORE_2\t=====\n");
#endif
			setLocalInt(2, popInt());
			break;
		case JVM_ISTORE_3:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ISTORE_3\t=====\n");
#endif
			setLocalInt(3, popInt());
			break;

		case JVM_ILOAD:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ILOAD\t=====\n");
#endif
			pushInt(getLocalInt(fetch()));
			break;
		case JVM_ILOAD_0:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ILOAD_0\t=====\n");
#endif
			pushInt(getLocalInt(0));
			break;
		case JVM_ILOAD_1:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ILOAD_1\t=====\n");
#endif
			pushInt(getLocalInt(1));
			break;
		case JVM_ILOAD_2:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ILOAD_2\t=====\n");
#endif
			pushInt(getLocalInt(2));
			break;
		case JVM_ILOAD_3:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ILOAD_3\t=====\n");
#endif
			pushInt(getLocalInt(3));
			break;

		case JVM_SSTORE:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SSTORE\t=====\n");
#endif
			setLocalShort(fetch(), popShort());
			break;
		case JVM_SSTORE_0:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SSTORE_0\t=====\n");
#endif
			setLocalShort(0, popShort());
			break;
		case JVM_SSTORE_1:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SSTORE_1\t=====\n");
#endif
			setLocalShort(1, popShort());
			break;
		case JVM_SSTORE_2:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SSTORE_2\t=====\n");
#endif
			setLocalShort(2, popShort());
			break;
		case JVM_SSTORE_3:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SSTORE_3\t=====\n");
#endif
			setLocalShort(3, popShort());
			break;

		case JVM_SLOAD:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SLOAD\t=====\n");
#endif
			pushShort(getLocalShort(fetch()));
			break;
		case JVM_SLOAD_0:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SLOAD_0\t=====\n");
#endif
			pushShort(getLocalShort(0));
			break;
		case JVM_SLOAD_1:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SLOAD_1\t=====\n");
#endif
			pushShort(getLocalShort(1));
			break;
		case JVM_SLOAD_2:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SLOAD_2\t=====\n");
#endif
			pushShort(getLocalShort(2));
			break;
		case JVM_SLOAD_3:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SLOAD_3\t=====\n");
#endif
			pushShort(getLocalShort(3));
			break;

		case JVM_ACONST_NULL:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ACONST_NULL\t=====\n");
#endif
			pushRef(nullref);
			break;

		case JVM_ALOAD:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ALOAD\t=====\n");
#endif
			pushRef(getLocalRef(fetch()));
			break;
		case JVM_ALOAD_0:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ALOAD_0\t=====\n");
#endif
			pushRef(getLocalRef(0));
			break;
		case JVM_ALOAD_1:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ALOAD_1\t=====\n");
#endif
			pushRef(getLocalRef(1));
			break;
		case JVM_ALOAD_2:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ALOAD_2\t=====\n");
#endif
			pushRef(getLocalRef(2));
			break;
		case JVM_ALOAD_3:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ALOAD_3\t=====\n");
#endif
			pushRef(getLocalRef(3));
			break;

		case JVM_ASTORE:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ASTORE\t=====\n");
#endif
			setLocalRef(fetch(), popRef());
			break;
		case JVM_ASTORE_0:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ASTORE_0\t=====\n");
#endif
			setLocalRef(0, popRef());
			break;
		case JVM_ASTORE_1:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ASTORE_1\t=====\n");
#endif
			setLocalRef(1, popRef());
			break;
		case JVM_ASTORE_2:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ASTORE_2\t=====\n");
#endif
			setLocalRef(2, popRef());
			break;
		case JVM_ASTORE_3:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ASTORE_3\t=====\n");
#endif
			setLocalRef(3, popRef());
			break;

			// Integer stack operations
		case JVM_IPOP:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IPOP\t=====\n");
#endif
			intStack--;
			break;

		case JVM_IPOP2:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IPOP2\t=====\n");
#endif

			intStack -= 2;
			break;

		case JVM_IDUP:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IDUP\t=====\n");
#endif
			*intStack = *(intStack - 1);
			intStack++;
			break;

		case JVM_IDUP2:

#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IDUP2\t=====\n");
			ASSEMBLY_DEBUG("intStack -1 : %d, intStack - 2 : %d\n", *(intStack - 1), *(intStack - 2));
#endif

			*(intStack + 1) = *(intStack - 1);
			*(intStack) = *(intStack - 2);

			intStack += 2;
			break;

		case JVM_IDUP_X:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IDUP_X\t=====\n");
#endif

			m = fetch();
			n = m & 15;
			m >>= 4;

			// reserve space on the stack for the duplicated data
			intStack += m;

			if (n == 0) {
				// perform normal dup
				for (i = 0; i < m; i++)
					intStack[-i - m - 1] = intStack[-i - 1];

			} else {
				// move existing stuff forwards
				for (i = 0; i < n; i++)
					intStack[-i - 1] = intStack[-i - m - 1];

				// copy duplicated value into place
				for (i = 0; i < m; i++)
					intStack[-i - n - 1] = intStack[-i - 1];
			}

			break;

			// TODO make faster
		case JVM_IDUP_X1:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IDUP_X1\t=====\n");
#endif

			temp1 = popShort();
			temp2 = popShort();
			pushShort(temp1);
			pushShort(temp2);
			pushShort(temp1);
			break;

			// TODO make faster
		case JVM_IDUP_X2:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IDUP_X2\t=====\n");
#endif

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
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_APOP\t=====\n");
#endif

			refStack++;
			break;

		case JVM_APOP2:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_APOP2\t=====\n");
#endif
			refStack += 2;
			break;

		case JVM_ADUP:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ADUP\t=====\n");
#endif
			refStack--;
			*refStack = *(refStack + 1);
			break;

		case JVM_ADUP2:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ADUP2\t=====\n");
#endif

			refStack -= 2;
			*(refStack) = *(refStack + 2);
			*(refStack + 1) = *(refStack + 3);
			break;

			// TODO make faster
		case JVM_ADUP_X1:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ADUP_X1\t=====\n");
#endif

			rtemp1 = popRef();
			rtemp2 = popRef();
			pushRef(rtemp1);
			pushRef(rtemp2);
			pushRef(rtemp1);
			break;

			// TODO make faster
		case JVM_ADUP_X2:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ADUP_X2\t=====\n");
#endif

			rtemp1 = popRef();
			rtemp2 = popRef();
			rtemp3 = popRef();
			pushRef(rtemp1);
			pushRef(rtemp3);
			pushRef(rtemp2);
			pushRef(rtemp1);
			break;

			// program flow
		case JVM_GOTO:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_GOTO\t=====\n");
#endif
			GOTO();
			break;

		case JVM_IF_ICMPEQ:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ICMPEQ\t=====\n");
#endif
			IF_ICMPEQ();
			break;
		case JVM_IF_ICMPNE:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ICMPNE\t=====\n");
#endif
			IF_ICMPNE();
			break;
		case JVM_IF_ICMPLT:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ICMPLT\t=====\n");
#endif
			IF_ICMPLT();
			break;
		case JVM_IF_ICMPGE:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ICMPGE\t=====\n");
#endif
			IF_ICMPGE();
			break;
		case JVM_IF_ICMPGT:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ICMPGT\t=====\n");
#endif
			IF_ICMPGT();
			break;
		case JVM_IF_ICMPLE:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ICMPLE\t=====\n");
#endif
			IF_ICMPLE();
			break;

		case JVM_IF_SCMPEQ:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SCMPEQ\t=====\n");
#endif
			IF_SCMPEQ();
			break;
		case JVM_IF_SCMPNE:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SCMPNE\t=====\n");
#endif
			IF_SCMPNE();
			break;
		case JVM_IF_SCMPLT:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SCMPLT\t=====\n");
#endif
			IF_SCMPLT();
			break;
		case JVM_IF_SCMPGE:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SCMPGE\t=====\n");
#endif
			IF_SCMPGE();
			break;
		case JVM_IF_SCMPGT:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SCMPGT\t=====\n");
#endif
			IF_SCMPGT();
			break;
		case JVM_IF_SCMPLE:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SCMPLE\t=====\n");
#endif
			IF_SCMPLE();
			break;

		case JVM_IIFEQ:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IIFEQ\t=====\n");
#endif
			IIFEQ();
			break;
		case JVM_IIFNE:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IIFNE\t=====\n");
#endif
			IIFNE();
			break;
		case JVM_IIFLT:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IIFLT\t=====\n");
#endif
			IIFLT();
			break;
		case JVM_IIFGE:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IIFGE\t=====\n");
#endif
			IIFGE();
			break;
		case JVM_IIFGT:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IIFGT\t=====\n");
#endif
			IIFGT();
			break;
		case JVM_IIFLE:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IIFLE\t=====\n");
#endif
			IIFLE();
			break;

		case JVM_SIFEQ:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SIFEQ\t=====\n");
#endif
			SIFEQ();
			break;
		case JVM_SIFNE:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SIFNE\t=====\n");
#endif
			SIFNE();
			break;
		case JVM_SIFLT:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SIFLT\t=====\n");
#endif
			SIFLT();
			break;
		case JVM_SIFGE:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SIFGE\t=====\n");
#endif
			SIFGE();
			break;
		case JVM_SIFGT:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SIFGT\t=====\n");
#endif
			SIFGT();
			break;
		case JVM_SIFLE:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SIFLE\t=====\n");
#endif
			SIFLE();
			break;

		case JVM_IF_ACMPEQ:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IF_ACMPEQ\t=====\n");
#endif
			IF_ACMPEQ();
			break;
		case JVM_IF_ACMPNE:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IF_ACMPNE\t=====\n");
#endif
			IF_ACMPNE();
			break;
		case JVM_IFNULL:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IFNULL\t=====\n");
#endif
			IFNULL();
			break;
		case JVM_IFNONNULL:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IFNONNULL\t=====\n");
#endif
			IFNONNULL();
			break;

		case JVM_RETURN:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_RETURN\t=====\n");
#endif
			RETURN();
			break;
		case JVM_IRETURN:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IRETURN\t=====\n");
#endif
			IRETURN();
			break;
		case JVM_SRETURN:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SRETURN\t=====\n");
#endif
			SRETURN();
			break;
		case JVM_ARETURN:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ARETURN\t=====\n");
#endif
			ARETURN();
			break;
		case JVM_INVOKESTATIC:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_INVOKESTATIC\t=====\n");
#endif
			INVOKESTATIC();
			break;
		case JVM_INVOKESPECIAL:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_INVOKESPECIAL\t=====\n");
#endif
			INVOKESPECIAL();
			break;
		case JVM_INVOKEVIRTUAL:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_INVOKEVIRTUAL\t=====\n");
#endif

			INVOKEVIRTUAL();
			break;
		case JVM_INVOKEINTERFACE:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_INVOKEINTERFACE\t=====\n");
#endif
			INVOKEINTERFACE();
			break;

			// monitors
		case JVM_MONITORENTER:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_MONITORENTER\t=====\n");
#endif
			MONITORENTER();
			break;
		case JVM_MONITOREXIT:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_MONITOREXIT\t=====\n");
#endif
			MONITOREXIT();
			break;

			// arrays and classes
		case JVM_NEW:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_NEW\t=====\n");
#endif
			NEW();
			break;
		case JVM_INSTANCEOF:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_INSTANCEOF\t=====\n");
#endif
			INSTANCEOF();
			break;
		case JVM_CHECKCAST:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_CHECKCAST\t=====\n");
#endif
			CHECKCAST();
			break;

		case JVM_NEWARRAY:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_NEWARRAY\t=====\n");
#endif
			NEWARRAY();
			break;
		case JVM_ANEWARRAY:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ANEWARRAY\t=====\n");
#endif
			ANEWARRAY();
			break;
		case JVM_ARRAYLENGTH:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ARRAYLENGTH\t=====\n");
#endif
			ARRAYLENGTH();
			break;
		case JVM_BASTORE:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_BASTORE\t=====\n");
#endif
			BASTORE();
			break;
		case JVM_CASTORE:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_CASTORE\t=====\n");
#endif
			CASTORE();
			break;
		case JVM_SASTORE:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SASTORE\t=====\n");
#endif
			SASTORE();
			break;
		case JVM_IASTORE:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IASTORE\t=====\n");
#endif
			IASTORE();
			break;
		case JVM_AASTORE:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_AASTORE\t=====\n");
#endif
			AASTORE();
			break;

		case JVM_BALOAD:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_BALOAD\t=====\n");
#endif
			BALOAD();
			break;
		case JVM_CALOAD:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_CALOAD\t=====\n");
#endif
			CALOAD();
			break;
		case JVM_SALOAD:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_SALOAD\t=====\n");
#endif
			SALOAD();
			break;
		case JVM_IALOAD:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_IALOAD\t=====\n");
#endif
			IALOAD();
			break;
		case JVM_AALOAD:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_AALOAD\t=====\n");
#endif
			AALOAD();
			break;

		case JVM_GETSTATIC_I:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_GETSTATIC_I\t=====\n");
#endif
			GETSTATIC_I();
			break;
		case JVM_GETSTATIC_A:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_GETSTATIC_A\t=====\n");
#endif
			GETSTATIC_A();
			break;
		case JVM_GETSTATIC_B:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_GETSTATIC_B\t=====\n");
#endif
			GETSTATIC_B();
			break;
		case JVM_GETSTATIC_C:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_GETSTATIC_C\t=====\n");
#endif
			GETSTATIC_C();
			break;
		case JVM_GETSTATIC_S:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_GETSTATIC_S\t=====\n");
#endif
			GETSTATIC_S();
			break;

		case JVM_PUTSTATIC_I:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_PUTSTATIC_I\t=====\n");
#endif
			PUTSTATIC_I();
			break;
		case JVM_PUTSTATIC_A:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_PUTSTATIC_A\t=====\n");
#endif
			PUTSTATIC_A();
			break;
		case JVM_PUTSTATIC_B:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_PUTSTATIC_B\t=====\n");
#endif
			PUTSTATIC_B();
			break;
		case JVM_PUTSTATIC_C:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_PUTSTATIC_C\t=====\n");
#endif
			PUTSTATIC_C();
			break;
		case JVM_PUTSTATIC_S:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_PUTSTATIC_S\t=====\n");
#endif
			PUTSTATIC_S();
			break;

		case JVM_GETFIELD_I:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_GETFIELD_I\t=====\n");
#endif
			GETFIELD_I();
			break;
		case JVM_GETFIELD_A:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_GETFIELD_A\t=====\n");
#endif
			GETFIELD_A();
			break;
		case JVM_GETFIELD_B:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_GETFIELD_B\t=====\n");
#endif
			GETFIELD_B();
			break;
		case JVM_GETFIELD_C:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_GETFIELD_C\t=====\n");
#endif
			GETFIELD_C();
			break;
		case JVM_GETFIELD_S:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_GETFIELD_S\t=====\n");
#endif
			GETFIELD_S();
			break;

		case JVM_PUTFIELD_I:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_PUTFIELD_I\t=====\n");
#endif
			PUTFIELD_I();
			break;
		case JVM_PUTFIELD_A:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_PUTFIELD_A\t=====\n");
#endif
			PUTFIELD_A();
			break;
		case JVM_PUTFIELD_B:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_PUTFIELD_B\t=====\n");
#endif
			PUTFIELD_B();
			break;
		case JVM_PUTFIELD_C:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_PUTFIELD_C\t=====\n");
#endif
			PUTFIELD_C();
			break;
		case JVM_PUTFIELD_S:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_PUTFIELD_S\t=====\n");
#endif
			PUTFIELD_S();
			break;

		case JVM_TABLESWITCH:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_TABLESWITCH\t=====\n");
#endif
			TABLESWITCH();
			break;
		case JVM_LOOKUPSWITCH:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_LOOKUPSWITCH\t=====\n");
#endif
			LOOKUPSWITCH();
			break;

		case JVM_ATHROW:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_ATHROW\t=====\n");
#endif
			ATHROW();
			break;

			// misc
		case JVM_NOP:
#ifdef ASSEMBLY_DEBUGGING
			ASSEMBLY_DEBUG("=====\tJVM_NOP\t=====\n");
#endif
			/* do nothing */
			break;

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
