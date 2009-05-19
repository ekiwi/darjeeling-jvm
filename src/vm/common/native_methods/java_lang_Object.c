#include <stddef.h>

#include "common/execution/execution.h"
#include "common/vmthread.h"
#include "common/vm.h"

#include "pointerwidth.h"

// generated by the infuser
#include "base_definitions.h"

// void java.lang.Object.wait(int)
void java_lang_Object_void_wait_int()
{
	dj_object * object = (dj_object*)REF_TO_VOIDP(dj_exec_stackPopRef());
	int32_t timeOut = dj_exec_stackPopInt();

	dj_thread * thread = dj_exec_getCurrentThread();

	dj_thread_wait(thread, object, timeOut);
	dj_exec_breakExecution();
}

// void java.lang.Object.wait()
void java_lang_Object_void_wait()
{
	dj_object * object = (dj_object*)REF_TO_VOIDP(dj_exec_stackPopRef());
	dj_thread * thread = dj_exec_getCurrentThread();

	dj_thread_wait(thread, object, 0);
	dj_exec_breakExecution();
}

// void java.lang.Object.notify()
void java_lang_Object_void_notify()
{
	dj_object * object = (dj_object*)REF_TO_VOIDP(dj_exec_stackPopRef());

	// make sure the thread owns the lock
	// TODO implement this check

	dj_vm_notify(dj_exec_getVM(), object, false);
}

// void java.lang.Object.notifyAll()
void java_lang_Object_void_notifyAll()
{
	dj_object * object = (dj_object*)REF_TO_VOIDP(dj_exec_stackPopRef());
	dj_vm_notify(dj_exec_getVM(), object, false);
}
