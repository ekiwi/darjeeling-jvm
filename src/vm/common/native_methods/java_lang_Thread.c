#include <stddef.h>

#include "common/execution/execution.h"
#include "common/global_id.h"
#include "common/debug.h"
#include "common/heap/heap.h"
#include "common/panic.h"

#include "pointerwidth.h"

// generated by the infuser
#include "base_definitions.h"

// short java.lang.Thread._create()
void java_lang_Thread_short__create()
{
	// create a new thread
	dj_thread *thread = dj_thread_create();

    if(thread == NULL)
    {
    	dj_exec_createAndThrow(BASE_CDEF_java_lang_OutOfMemoryError);
    	return;
    }

	dj_vm_addThread(dj_exec_getVM(), thread);

	// return thread ID as a short
	dj_exec_stackPushShort(thread->id);
}

// void java.lang.Thread._start(short)
void java_lang_Thread_void__start_short()
{
	// pop thread Id and get the corresponding Thread object
	int16_t id = dj_exec_stackPopShort();
	dj_thread * thread = dj_vm_getThreadById(dj_exec_getVM(), id);

	// create a ResolvedId to represent the method definition we're looking for
	dj_global_id methodDefId;
	methodDefId.infusion = dj_vm_getSystemInfusion(dj_exec_getVM());
	methodDefId.entity_id = BASE_MDEF_void_run;

	// lookup method
	dj_global_id methodImplId = dj_global_id_lookupVirtualMethod(methodDefId, thread->runnable);

	// create a frame for the 'run' function and push it on the thread stack
	dj_mem_pushCompactionUpdateStack(VOIDP_TO_REF(thread));
	dj_frame *frame = dj_frame_create(methodImplId);
	thread = REF_TO_VOIDP(dj_mem_popCompactionUpdateStack());

    // check that the frame alloc was succesful
	if(frame == NULL)
    {
		dj_exec_createAndThrow(BASE_CDEF_java_lang_StackOverflowError);
		return;
    }

	// push the new frame on the thread's frame stack
	thread->frameStack = frame;

	// copy the runnable object to the first reference local variable ('this') in the
	// new frame
	dj_frame_getLocalReferenceVariables(frame)[0] = VOIDP_TO_REF(thread->runnable);

	// mark new thread eligible for execution
	thread->status = THREADSTATUS_RUNNING;

}

// short java.lang.Thread._getStatus(short)
void java_lang_Thread_short__getStatus_short()
{
	int16_t id = dj_exec_stackPopShort();
	dj_thread * thread = dj_vm_getThreadById(dj_exec_getVM(), id);

	if (thread==0)
		dj_exec_stackPushShort(-1);
	else
		dj_exec_stackPushShort(thread->status);
}

// void java.lang.Thread._setRunnable(short, java.lang.Runnable)
void java_lang_Thread_void__setRunnable_short_java_lang_Runnable()
{
	ref_t runnable = dj_exec_stackPopRef();
	int16_t id = dj_exec_stackPopShort();

	dj_thread * thread = dj_vm_getThreadById(dj_exec_getVM(), id);
	thread->runnable = REF_TO_VOIDP(runnable);
}

// void java.lang.Thread.sleep(int)
void java_lang_Thread_void_sleep_int()
{
	int32_t time = dj_exec_stackPopInt();
	dj_thread *thread = dj_exec_getCurrentThread();
	dj_thread_sleep(thread, time);
	dj_exec_breakExecution();
}

// short java.lang.Thread._getCurrentThreadId()
void java_lang_Thread_short__getCurrentThreadId()
{
	dj_exec_stackPushShort(dj_exec_getCurrentThread()->id);
}

// int java.lang.Thread.activeCount()
void java_lang_Thread_int_activeCount()
{
	dj_exec_stackPushInt(dj_vm_countLiveThreads(dj_exec_getVM()));
}

// void java.lang.Thread.yield()
void java_lang_Thread_void_yield()
{
	dj_exec_breakExecution();
}
