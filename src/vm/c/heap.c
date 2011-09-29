/*
 * heap.c
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
 
/**
 * \defgroup heap Heap management
 * \ingroup heap
 * @{
 * \page heap_description
 *
 * Darjeeling manages its own heap using garbage collection. Java objects and internal objects (threads, stack frames,
 * infusions etc) are mixed. Each heap element (chunk) carries a byte that indicates its type. Some type numbers are
 * reserved for internal objects or arrays, the other numbers are used for Class ID's.
 *
 * The Heap manager implements a simple mark & compact garbage collector. Blocks are allocated using pointer bumping
 * for quick allocation.
 *
 * The marking algorithm is a stop-the-world tri-color marker. The root set is first marked directly and set to gray
 * or black. The heap is then sweeped pulling each gray block into the black set by following its references and
 * marking them gray. The heap is sweeped in consecutive passes until there are no more gray elements on the heap.
 *
 * Because Darjeeling separates reference and integer values in allocated Objects and on the stack, the root set can be
 * marked directly by following these reference values. This means that there are no false positives in the root set
 * and that marking the root set is done efficiently.
 *
 * Eventually the code from this garbage collector should be merged with the code from the old mark & sweep collector
 * so that allocation is done using a free list and compaction is only triggered when absolutely neccesairy.
 *
 * Because calls to dj_mem_alloc() can trigger garbage compaction, pointers (in the C code) may be made invalid as the
 * element they were pointing to may have been moved. A C pointer can be added to the 'safe pointer pool' which will cause
 * the object the pointer is pointing to to be marked as live, and the pointer to be updated in case of GC. The size of
 * the pool is limited, so it's best to add pointers only temporarily (i.e. during method calls where multiple allocations
 * are needed).
 *
 * For example:
 *
 * <code>
 *   // pop an object off the operand stack<br>
 *   dj_object * object = REF_TO_VOIDP(dj_exec_stackPopRef());<br>
 *   <br>
 *   // make sure the object pointer is updated on garbage compaction<br>
 *   dj_mem_addSafePointer((void**)&object);
 *   <br>
 *   // do some memory allocation<br>
 *   void * something = dj_mem_alloc(...);<br>
 *   <br>
 *   // remove the pointer from the safe pool when done
 *   dj_mem_removeSafePointer((void**)&object);
 *   <br>
 * </code>
 *
 */

#include <string.h>

#include "config.h"
#include "heap.h"

#include "vm.h"
#include "vmthread.h"
#include "infusion.h"

#include "array.h"
#include "execution.h"
#include "debug.h"
#include "panic.h"
#include "jlib_base.h"

static char *heap_base;
static dj_object *panicExceptionObject;
static uint16_t heap_size;

static void ** safePointerPool[SAFE_POINTER_POOL_SIZE];

static void *left_pointer, *right_pointer;

#ifdef DARJEELING_DEBUG_MEM_TRACE
static int nrTrace = 0;
#endif

/**
 * Initialises the memory manager. A call to this function may trigger garbage collection.
 * @param mem_pointer pointer to where Darjeeling can manage its heap
 * @param mem_size size of the heap
 */
void dj_mem_init(void *mem_pointer, uint16_t mem_size)
{
	uint16_t i;

    // initialize our private pointer to the heap memory
    heap_base =  mem_pointer;

    // the rest of the memory is for heap chunks
    heap_size = mem_size;

    for (i=0; i<SAFE_POINTER_POOL_SIZE; i++) safePointerPool[i] = NULL;

    panicExceptionObject = nullref;

    left_pointer = heap_base;
    right_pointer = heap_base + heap_size;

}

/**
 * Allocates <emph>size</emph> bytes.
 * @param size size in bytes
 * @param id chunk ID.
 */
void * dj_mem_alloc(uint16_t size, uint16_t id)
{
	heap_chunk *ret;

	// if ALIGN_16 is defined (for MSP430 platform) make sure the size of the
	// new chunk is a multiple of 2
#ifdef ALIGN_16
	if (size&1) size++;
#endif

	// we need to accommodate for a chunk header
	size += sizeof(heap_chunk);

	if (right_pointer-left_pointer<size)
	{
		// not enough memory
        DEBUG_LOG("dj_mem_alloc: not enough free space, triggering a collection\n");
		dj_mem_gc();
	}

	if (right_pointer-left_pointer<size)
	{
		// still not enough memory, return null
        DEBUG_LOG("Not enough memory, returning NULL\n");
        return nullref;
	}

	ret = (heap_chunk*)left_pointer;
	ret->size = size;
	ret->id = id;

	left_pointer += size;

	return (void*)((size_t)ret + sizeof(heap_chunk));
}

/**
 * Frees memory on the heap.
 * @ptr pointer to memory
 */
void dj_mem_free(void * ptr)
{
	heap_chunk *chunk = (heap_chunk*)(ptr - sizeof(heap_chunk));

	if (((void*)chunk + chunk->size)==left_pointer)
		left_pointer = (void*)chunk;
	else
		chunk->id = CHUNKID_FREE;

}


/**
 * @return the total number of bytes on the heap.
 */
uint16_t dj_mem_getSize()
{
	return heap_size;
}

/**
 * @return the number of bytes left on the heap.
 */
uint16_t dj_mem_getFree()
{
	return right_pointer - left_pointer;
}

void dj_mem_setPanicExceptionObject(dj_object *obj)
{
	panicExceptionObject = obj;
}

dj_object * dj_mem_getPanicExceptionObject()
{
	return panicExceptionObject;
}

/**
 * Pushes a pointer on the safe pointer stack.
 * @param void** the pointer to keep updated during compaction.
 */
void dj_mem_addSafePointer(void ** ptr)
{
	uint16_t i;

	for (i=0; i<SAFE_POINTER_POOL_SIZE; i++)
		if (safePointerPool[i]==NULL)
		{
			safePointerPool[i] = ptr;
			return;
		}

	dj_panic(DJ_PANIC_SAFE_POINTER_OVERFLOW);
}

uint16_t dj_mem_countSafePointers()
{
	uint16_t ret = 0, i;

	for (i=0; i<SAFE_POINTER_POOL_SIZE; i++)
		if (safePointerPool[i]!=NULL) ret ++;

	return ret;
}

/**
 * Pops a reference from the compaction update stack.
 * @return updated reference
 */
void dj_mem_removeSafePointer(void ** ptr)
{
	uint16_t i;

	for (i=0; i<SAFE_POINTER_POOL_SIZE; i++)
		if (safePointerPool[i]==ptr)
			safePointerPool[i] = NULL;

}

/**
 * Shifts the runtime IDs of objects on the heap to correct ranges when infusions are unloaded.
 * Any id within [start, start+range> is marked invalid (as the infusion that holds the Class
 * definition for that object is unloaded), and IDs in [start+range, ->> are mapped to [start,->>
 */
void dj_mem_shiftRuntimeIDs(runtime_id_t start, uint16_t range)
{

	heap_chunk *chunk;
	void * loc = heap_base;
	runtime_id_t id;

	// initialise all chunk headers to WHITE
	while (loc<left_pointer)
	{
		chunk = (heap_chunk*)loc;
		id = chunk->id;

		// if the ID is inside [start, start+range>, it belongs to the unloaded
		// infusion, so mark the object 'invalid'
		if (id>start&&id<(start+range))
			chunk->id = CHUNKID_INVALID;

		// if the ID is in [start+range, ->>, it belongs to some infusion with IDs
		// that are higher than the one in the unloaded infusion, so they should be
		// shifted back
		if (id>start+range)
			chunk->id-=range;

		loc += chunk->size;
	}


}


/**
 * Implements the marking phase using stop-the-world tri-color marking
 */
static inline void dj_mem_markObject(dj_vm *vm, heap_chunk * chunk)
{
	dj_di_pointer classDef;
	dj_ref_array *refArray;
	void * object;
	int i;
	ref_t *refs;

	// get the chunk that contains our object
	object = dj_mem_getData(chunk);

	// if it's a Java object, mark its children
	if (chunk->id>=CHUNKID_JAVA_START)
	{
		classDef = dj_vm_getRuntimeClassDefinition(vm, chunk->id);
		refs = dj_object_getReferences(object);
		for (i=0; i<dj_di_classDefinition_getNrRefs(classDef); i++)
			dj_mem_setRefGrayIfWhite(refs[i]);
	}

	// if it's a reference array, mark the members
	if (chunk->id==CHUNKID_REFARRAY)
	{
		refArray = (dj_ref_array*)object;
		for (i=0; i<refArray->array.length; i++)
			dj_mem_setRefGrayIfWhite(refArray->refs[i]);
	}

	// mark the object as 'black'
	chunk->color = TCM_BLACK;

}

/**
 * We'll add hooks to object finalisers here later
 */
static inline void dj_finalise(heap_chunk * chunk)
{
	switch (chunk->id)
	{

	default:
		chunk->id = CHUNKID_FREE;
		// nothing

	}

}

/**
 * Implements the marking phase using stop-the-world tri-color marking
 */
static inline void dj_mem_mark()
{
	dj_vm *vm = dj_exec_getVM();
	heap_chunk *chunk;
	void * loc = heap_base;
	uint8_t i;

	// Initialise chunk colors to white for managed objects (Java objects, infusions), and
	// black for built-in objects (frames, threads)
	while (loc<left_pointer)
	{
		chunk = (heap_chunk*)loc;
		chunk->color = chunk->id<CHUNKID_MANAGED_START?TCM_BLACK:TCM_WHITE;
		loc += chunk->size;
	}

	DEBUG_LOG("\tmark root set\n");

	// mark the root set (set all elements in the root set to 'gray')
	dj_vm_markRootSet(vm);

	DEBUG_LOG("\tmark reference stack\n");

	// mark the panic exception object
	if (panicExceptionObject!=nullref)
		dj_mem_setRefGrayIfWhite(VOIDP_TO_REF(panicExceptionObject));

	// mark the safe pointer pool
	for (i=0; i<SAFE_POINTER_POOL_SIZE; i++)
		if (safePointerPool[i]!=NULL)
			dj_mem_setPointerGrayIfWhite(*(safePointerPool[i]));

	// iterate over the chunks, make every gray chunk black
	int nrGray;
	do
	{
		DEBUG_LOG("\titeration\n");

		// move over the chunks and visit all the gray objects
		nrGray=0;
		loc = heap_base;
		while (loc<left_pointer)
		{
			chunk = (heap_chunk*)loc;

			if (chunk->color==TCM_GRAY)
			{
				dj_mem_markObject(vm, chunk);
				nrGray++;
			}

			loc += chunk->size;
		}

	} while (nrGray>0);

	// Call finalise on each of the chunks
	loc = heap_base;
	while (loc<left_pointer)
	{
		chunk = (heap_chunk*)loc;
		if (chunk->color==TCM_WHITE)
			dj_finalise(chunk);

		loc += chunk->size;
	}

}

static inline void dj_mem_updateManagedReference(dj_vm * vm, heap_chunk *chunk)
{
	int i;
	dj_di_pointer classDef;
	ref_t *refs;

	if (chunk->id>=CHUNKID_JAVA_START)
	{
		// object
                classDef = dj_vm_getRuntimeClassDefinition(vm, chunk->id);
                //refs = dj_object_getReferences((dj_object*)chunk);
		refs = dj_object_getReferences(dj_mem_getData(chunk));
		for (i=0; i<dj_di_classDefinition_getNrRefs(classDef); i++)
			refs[i] = dj_mem_getUpdatedReference(refs[i]);
	}

	if (chunk->id==CHUNKID_REFARRAY)
	{
		dj_ref_array_updatePointers((dj_ref_array*)dj_mem_getData(chunk));
	}

}

static inline void dj_mem_updateSystemReference(dj_vm * vm, heap_chunk *chunk)
{
	switch (chunk->id)
	{

		case CHUNKID_INFUSION:
			dj_infusion_updatePointers((dj_infusion*)dj_mem_getData(chunk));
			break;

		case CHUNKID_VM:
			dj_vm_updatePointers((dj_vm*)dj_mem_getData(chunk));
			break;

		case CHUNKID_THREAD:
			dj_thread_updatePointers((dj_thread*)dj_mem_getData(chunk));
			break;

		case CHUNKID_MONITOR_BLOCK:
			dj_monitor_block_updatePointers((dj_monitor_block*)dj_mem_getData(chunk));
			break;

		case CHUNKID_FRAME:
			dj_frame_updatePointers((dj_frame*)dj_mem_getData(chunk));
			break;

		default:
			break;

	}
}

void dj_mem_compact()
{
	int i;
	dj_vm *vm = dj_exec_getVM();

	heap_chunk *chunk;
	uint32_t shift = 0;
	uint32_t chunkSize;
	void * loc;

	// calculate how many bytes each non-free chunk will be shifted by compaction
	// and store this number in the chunk header
	loc = heap_base;
	while (loc<left_pointer)
	{
		chunk = (heap_chunk*)loc;

		if (chunk->id==CHUNKID_FREE) shift += chunk->size;

		chunk->shift = shift;
		loc += chunk->size;
	}

	// Update the managed pointers in each of the chunks.
	// The pointers are updated in two passes because to update the heap objects we
	// need the vm and infusions in tact.
	loc = heap_base;
	while (loc<left_pointer)
	{
		chunk = (heap_chunk*)loc;
		dj_mem_updateManagedReference(vm, chunk);
		loc += chunk->size;
	}

	// When the Java objects have been updated, we can safely update the system
	// objects.
	loc = heap_base;
	while (loc<left_pointer)
	{
		chunk = (heap_chunk*)loc;
		dj_mem_updateSystemReference(vm, chunk);
		loc += chunk->size;
	}

	// update pointers in the safe pointer pool
	for (i=0; i<SAFE_POINTER_POOL_SIZE; i++)
		if (safePointerPool[i]!=NULL)
			*(safePointerPool[i]) = dj_mem_getUpdatedPointer(*(safePointerPool[i]));

	// update the pointer to the panic exception object, if any
	if (panicExceptionObject!=nullref)
		panicExceptionObject = dj_mem_getUpdatedPointer(panicExceptionObject);

	// update global pointers in the execution engine
	dj_exec_updatePointers();

	// physically move the chunks to their new positions
	loc = heap_base;
	while (loc<left_pointer)
	{
		chunk = (heap_chunk*)loc;
		chunkSize = chunk->size;

		if (chunk->id!=CHUNKID_FREE)
			memmove(loc-chunk->shift, loc, chunkSize);

		loc += chunkSize;
	}

	left_pointer-=shift;

}

void dj_mem_gc()
{
	dj_thread * thread;

	DEBUG_LOG("GC start\n");
	dj_vm *vm = dj_exec_getVM();
	if (vm == NULL)
	{
		DARJEELING_PRINTF("Garbage collection cannot start, the VM is not yet initialized\n");
		dj_panic(DJ_PANIC_OUT_OF_MEMORY);
	}
	// Force the execution engine to store the nr_int_stack and nr_ref_stack in the current frame struct
	// we need this for the root set marking phase

	thread = dj_exec_getCurrentThread();
	if (thread && thread->frameStack) dj_exec_deactivateThread(thread);

	dj_mem_mark();
	dj_mem_compact();

	// re-get the VM instance, it might have been moved
	vm = dj_exec_getVM();

	thread = dj_exec_getCurrentThread();
	if (thread && thread->frameStack) dj_exec_activate_thread(thread);

	DEBUG_LOG("GC done\n");
}

//void dj_mem_thread_dump()
//{
//
//	dj_vm *vm = dj_exec_getVM();
//	if (vm==NULL)
//		return;
//
//	int i, threadTotal, grandTotal;
//	int nrThreads = dj_vm_countThreads(vm);
//	dj_thread *thread;
//	dj_frame *frame;
//
//	time++;
//	grandTotal = 0;
//
//	int threads[16];
//	for (i=0; i<16; i++)
//		threads[i] = 0;
//
//	if (nrThreads>0)
//	{
//		for (i=0; i<nrThreads; i++)
//		{
//			thread = dj_vm_getThread(vm, i);
//			threadTotal = dj_mem_get_chunk_size(thread) - 10;
//
//			for (frame=thread->frameStack; frame!=NULL; frame=frame->parent)
//				threadTotal += dj_mem_get_chunk_size(frame) - 4;
//
//			threads[thread->id] = threadTotal;
//			grandTotal += threadTotal;
//		}
//		printf("%d %d %d %d %d\n",
//				time,
//				threads[1],
//				threads[2],
//				threads[3],
//				grandTotal);
//	}
//
//}

#ifdef DARJEELING_DEBUG
void dj_mem_dump()
{
	heap_chunk *finger;

	// iterate over all blocks
	finger = (heap_chunk*)heap_base;

	int total = 0;

	while ( (char*)finger < left_pointer )
    {
        char pretty_printing_buffer[5];
        char *chunk_type_pretty_print;
        switch(finger->id)
        {
            case CHUNKID_REFARRAY:
                chunk_type_pretty_print="REFA";
                break;
            case CHUNKID_INTARRAY:
                chunk_type_pretty_print="INTA";
                break;
            case CHUNKID_FREE:
                chunk_type_pretty_print="FREE";
                break;
            case CHUNKID_INVALID:
                chunk_type_pretty_print="INV";
                break;
            case CHUNKID_MONITOR_BLOCK:
                chunk_type_pretty_print="MTBK";
                break;
            case CHUNKID_VM:
                chunk_type_pretty_print="  VM";
                break;
            case CHUNKID_INFUSION:
                chunk_type_pretty_print="IFSN";
                break;
            case CHUNKID_FRAME:
                chunk_type_pretty_print="STKF";
                break;
             case CHUNKID_THREAD:
                 chunk_type_pretty_print="THRD";
                 break;
            default:
                snprintf(pretty_printing_buffer,5," %03d",finger->id);
                chunk_type_pretty_print=pretty_printing_buffer;
                break;

        }

		// printf("%c[32mASSERT[%3d] PASSED%c[0m\n", 0x1b, (int)id, 0x1b);
        uint8_t color = 31 + finger->color;
		DEBUG_LOG("%c[%dm[%p %04d %s]%c[0m ", 0x1b, color, finger, finger->size, chunk_type_pretty_print, 0x1b);
		total += finger->size;
		if (finger->size==0)
			break;
		finger = (heap_chunk*)((char*)finger + finger->size);
	}

	DEBUG_LOG("\n");
	DEBUG_LOG("total %d\n", total);

}
#endif // ifdef DARJEELING_DEBUG


#ifdef DARJEELING_DEBUG_MEM_TRACE
void dj_mem_dumpMemUsage()
{
	heap_chunk *finger;
	dj_thread *thread;
	dj_frame *frame;
	dj_vm *vm = dj_exec_getVM();
	int id, total, grandTotal;

	if (vm==NULL)
		return;

	printf("TRACE %d", nrTrace);

	grandTotal = 0;

	// output the first 8 threads
	for (id=0; id<8; id++)
	{
		thread = dj_vm_getThreadById(vm, id);
		if (thread!=NULL)
		{
			total = dj_mem_getChunkSize(thread) - (4 * 2);

			frame = thread->frameStack;
			while (frame!=NULL)
			{
				total += dj_mem_getChunkSize(frame) - (2 * 2);
				frame = frame->parent;
			}

		} else
			total = 0;

		grandTotal += total;

		printf(", %d", total);
	}

	thread = dj_exec_getCurrentThread();
	if (thread==NULL)
		printf(", %d", -1);
	else
		printf(", %d", dj_exec_getCurrentThread()->id);

	printf(", %d", grandTotal);

	printf("\n");
	nrTrace++;
}
#endif // #ifdef DARJEELING_DEBUG_MEM_TRACE
