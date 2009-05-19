/*
 *	main.c
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
#include "stdlib.h"
#include "stdio.h"

#include "common/parse_infusion.h"
#include "common/heap/heap.h"
#include "common/vmthread.h"
#include "common/infusion.h"
#include "common/vm.h"
#include "common/execution/execution.h"
#include "common/debug.h"

// included from build/generated
#include "base_native.h"
#include "base_definitions.h"
#include "darjeeling_native.h"

// TODO NB this should not be a global 'extern int' ffs :)
// ref_t is now  only 16-bits wide for everyone. Thus,  we need a base
// address to resolve ref_t references into 32-bits pointers.
// see linux/pointerwidth.h
char * ref_t_base_address;

// load raw infusion file into memory
dj_di_pointer loadDI(char *fileName)
{

	// TODO: error handling - but this is just for testing anyway

	FILE *diFile;
	long size = 0;
	char *bytes;
	dj_di_pointer di;


	diFile = fopen(fileName,"rb");

	if (!diFile)
	{
		printf("Fatal: file not found: %s\n", fileName);
		exit(-1);
	}

	// get file size
	fseek(diFile,0,SEEK_END);
	size = ftell(diFile);
	fseek(diFile,0,SEEK_SET);

	// load the entire thing as one block
	bytes = (char*)malloc(size);
	fread(bytes,1,size,diFile);

	// close file
	fclose(diFile);

	// cast the byte pointer to a flex file struct and dump its contents
	di = (dj_di_pointer)bytes;

	return di;

}

int main(int argc,char* argv[])
{
	int entryPointIndex;
	dj_vm *vm;
	dj_di_pointer di;
	dj_infusion *infusion;
	dj_thread *thread;
	dj_global_id entryPoint;

	// initialise memory manager
	void *mem = malloc(MEMSIZE);
	dj_mem_init(mem, MEMSIZE);
    ref_t_base_address = (char*)mem - 42;

	// create a new VM
	vm = dj_vm_create();

	// tell the execution engine to use the newly created VM instance
	dj_exec_setVM(vm);

	// load native infusion
	di = loadDI("build/infusions/base.di");
	infusion = dj_vm_loadSystemInfusion(vm, di);
	infusion->native_handler = base_native_handler;
	dj_vm_runClassInitialisers(vm, infusion);

	// load infusion files
	di = loadDI("build/infusions/darjeeling.di");
	infusion = dj_vm_loadInfusion(vm, di);
	infusion->native_handler = darjeeling_native_handler;
	dj_vm_runClassInitialisers(vm, infusion);

    if(argc>1)
        di = loadDI(argv[1]);
    else
        di=loadDI("build/infusions/testsuite.di");

	infusion = dj_vm_loadInfusion(vm, di);
	dj_vm_runClassInitialisers(vm, infusion);

	// pre-allocate an OutOfMemoryError object
	dj_object *obj = dj_vm_createSysLibObject(vm, BASE_CDEF_java_lang_OutOfMemoryError);
	dj_mem_setPanicExceptionObject(obj);

	// find the entry point for the infusion
	if ((entryPointIndex = dj_di_header_getEntryPoint(infusion->header))==255)
	{
		printf("No entry point found\n");
		return 0;
	} else
	{
		entryPoint.infusion = infusion;
		entryPoint.entity_id = entryPointIndex;
	}

	// create a new thread and add it to the VM
	thread = dj_thread_create_and_run(entryPoint);
	dj_vm_addThread(vm, thread);

    DEBUG_LOG("Starting the main execution loop\n");

	// start the main execution loop
	while (dj_vm_countLiveThreads(vm)>0)
	{
		dj_vm_schedule(vm);
		if (vm->currentThread!=NULL)
			if (vm->currentThread->status==THREADSTATUS_RUNNING)
				dj_exec_run(RUNSIZE);
	}

	dj_vm_schedule(vm);
	dj_mem_gc();

	return 0;
}
