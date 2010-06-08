#include "darjeeling.h"

#include <stdlib.h>
#include <stdio.h>
#include "debug.h"
#include "vm.h"
#include "heap.h"
#include "infusion.h"
#include "types.h"
#include "vmthread.h"
#include "djtimer.h"
#include "execution.h"

#include "tosconfig.h"

#include "jlib_base.h"
#include "jlib_darjeeling.h"
#ifdef WITH_RADIO
#include "jlib_radio.h"
#endif

#ifdef WITH_RADIO
#include "jlib_radio.h"
#endif
extern unsigned char di_archive_data[];
extern size_t di_archive_size;
static unsigned char mem[HEAPSIZE];
static dj_vm *vm;

//extern int16_t TOS_NODE_ID;

void dj_init()
{
	// initialise memory manager
	dj_mem_init(mem, HEAPSIZE);

	// initialise timer
	dj_timer_init();

	// create a new VM
	vm = dj_vm_create();

	// tell the execution engine to use the newly created VM instance
	dj_exec_setVM(vm);

	// load the embedded infusions

	dj_named_native_handler handlers[] = {
			{ "base", &base_native_handler },
			{ "darjeeling", &darjeeling_native_handler }
#ifdef WITH_RADIO
			,{ "radio", &radio_native_handler }
#endif
        };

	int length = sizeof(handlers)/ sizeof(handlers[0]);
        dj_archive archive;
        archive.start = (dj_di_pointer)di_archive_data;
        archive.end = (dj_di_pointer)(di_archive_data + di_archive_size);

	dj_vm_loadInfusionArchive(vm, &archive, handlers, length);	// load the embedded infusions

	// pre-allocate an OutOfMemoryError object
	dj_object *obj = dj_vm_createSysLibObject(vm, BASE_CDEF_java_lang_OutOfMemoryError);
	dj_mem_setPanicExceptionObject(obj);


}

uint32_t dj_run()
{
	// find thread to schedule
	dj_vm_schedule(vm);

	if (dj_exec_getCurrentThread()!=NULL)
		if (dj_exec_getCurrentThread()->status==THREADSTATUS_RUNNING)
			dj_exec_run(RUNSIZE);

	return dj_vm_getVMSleepTime(vm);

}

void notify_radio_receive();
void notify_radio_sendDone();
void notify_serial_sendDone();

void dj_notifyRadioReceive()
{
#ifdef WITH_RADIO
	notify_radio_receive();
#endif
}

void dj_notifyRadioSendDone()
{
#ifdef WITH_RADIO
	notify_radio_sendDone();
#endif
}

void dj_notifySerialSendDone()
{
	notify_serial_sendDone();
}
