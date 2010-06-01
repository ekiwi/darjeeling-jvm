#include <stdlib.h>
#include <stdio.h>

#include <avr/pgmspace.h>
#include <avr/io.h>

#include "debug.h"
#include "vm.h"
#include "heap.h"
#include "infusion.h"
#include "types.h"
#include "vmthread.h"
#include "djtimer.h"
#include "execution.h"

#include "jlib_base.h"
#include "jlib_darjeeling.h"

#include "pointerwidth.h"

/*
extern char * * _binary_infusions_start;
extern char * * _binary_infusions_end;
extern size_t * _binary_infusions_size;
*/

extern unsigned char di_archive_data[];
extern size_t di_archive_size;

unsigned char mem[HEAPSIZE];

#include "avr.h"

#include "PCF8833.h"

int main()
{
	dj_vm *vm;

	// initialise serial port
	avr_serialInit(57600);

	// initialise timer
	dj_timer_init();

	// initialise memory managerw
	dj_mem_init(mem, HEAPSIZE);

	// create a new VM
	vm = dj_vm_create();

	// tell the execution engine to use the newly created VM instance
	dj_exec_setVM(vm);

	dj_named_native_handler handlers[] = {
		{ PSTR("base"), &base_native_handler },
		{ PSTR("darjeeling"), &darjeeling_native_handler }
	};

	int length = sizeof(handlers)/ sizeof(handlers[0]);
        dj_archive archive;
        archive.start = (dj_di_pointer)di_archive_data;
        archive.end = (dj_di_pointer)(di_archive_data + di_archive_size);

        dj_vm_loadInfusionArchive(vm, &archive, handlers, length);

	// dj_vm_loadInfusionArchive(vm, (dj_di_pointer)&_binary_infusions_start, (dj_di_pointer)&_binary_infusions_end, handlers, 2);

	// start the main execution loop
	while (dj_vm_countLiveThreads(vm)>0)
	{
		dj_vm_schedule(vm);

		if (vm->currentThread!=NULL)
			if (vm->currentThread->status==THREADSTATUS_RUNNING)
				dj_exec_run(RUNSIZE);

		// yield

	}

	return 0;

}
