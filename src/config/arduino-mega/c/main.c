/*
 * main.c
 * 
 * Copyright (c) 2008-2010 CSIRO, Delft University of Technology.
 * 
 * This file is part of Darjeeling.
 * 
 * Darjeeling is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Darjeeling is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */
 
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
#include "jlib_darjeeling2.h"
#include "jlib_nklcd.h"
#include "jlib_tank.h"

#include "pointerwidth.h"

extern unsigned char di_archive_data[];
extern size_t di_archive_size;

unsigned char mem[HEAPSIZE];

#include "avr.h"

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
			{ "base", &base_native_handler },
			{ "darjeeling2", &darjeeling2_native_handler },
			{ "nklcd", &nklcd_native_handler },
			{ "tank", &tank_native_handler },
		};

	int length = sizeof(handlers)/ sizeof(handlers[0]);
	dj_archive archive;
	archive.start = (dj_di_pointer)di_archive_data;
	archive.end = (dj_di_pointer)(di_archive_data + di_archive_size);

	dj_vm_loadInfusionArchive(vm, &archive, handlers, length);	

#ifdef DARJEELING_DEBUG
	avr_serialPrintf("Darjeeling is go!\n\r");
#endif

	// start the main execution loop
	while (dj_vm_countLiveThreads(vm)>0)
	{
		dj_vm_schedule(vm);

		if (vm->currentThread!=NULL)
			if (vm->currentThread->status==THREADSTATUS_RUNNING)
				dj_exec_run(RUNSIZE);

		// yield

	}

#ifdef DARJEELING_DEBUG
	avr_serialPrintf("All threads terminated.\n\r");
#endif

	return 0;

}
