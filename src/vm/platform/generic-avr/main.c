#include <stdlib.h>
#include <stdio.h>

//needed for printf_P
#include <avr/pgmspace.h>

#include "common/debug.h"
#include "common/vm.h"
#include "common/heap/heap.h"
#include "common/infusion.h"
#include "common/types.h"
#include "common/vmthread.h"
#include "common/djtimer.h"
#include "common/execution/execution.h"

#include "loader.h"

unsigned char mem[HEAPSIZE];

int main()
{
	dj_vm *vm;

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
