/*
 *	darjeeling.c
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
#include "darjeeling.h"

#include "common/debug.h"
#include "common/vm.h"
#include "common/heap/heap.h"
#include "common/infusion.h"
#include "common/types.h"
#include "common/vmthread.h"
#include "common/djtimer.h"
#include "common/execution/execution.h"

#include "loader.h"

#include "nesc.h"

#include "base_definitions.h"

static unsigned char mem[HEAPSIZE];
static dj_vm *vm;

extern int16_t TOS_NODE_ID;

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
	dj_loadEmbeddedInfusions(vm);

	srand(TOS_NODE_ID);

	// pre-allocate an OutOfMemoryError object
	dj_object *obj = dj_vm_createSysLibObject(vm, BASE_CDEF_java_lang_OutOfMemoryError);
	dj_mem_setPanicExceptionObject(obj);


}

uint32_t dj_run()
{
	// find thread to schedule
	dj_vm_schedule(vm);

	if (vm->currentThread!=NULL)
		if (vm->currentThread->status==THREADSTATUS_RUNNING)
			dj_exec_run(RUNSIZE);

	return dj_vm_getVMSleepTime(vm);

}

void notify_radio_receive();
void notify_radio_sendDone();
void notify_serial_sendDone();

void dj_notifyRadioReceive()
{
	notify_radio_receive();
}

void dj_notifyRadioSendDone()
{
	notify_radio_sendDone();
}

void dj_notifySerialSendDone()
{
	notify_serial_sendDone();
}
