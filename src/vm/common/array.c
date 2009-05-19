#include <string.h>
#include <stdlib.h>

#include "common/array.h"
#include "common/heap/heap.h"
#include "common/debug.h"
#include "common/execution/execution.h"
#include "common/panic.h"

/**
 * Creates a new integer array.
 * @param type integer type, one of T_BOOLEAN, T_CHAR, T_BYTE, T_SHORT,	T_INT, used for type checking (instanceof etc)
 * @param size number of elements
 */
dj_int_array *dj_int_array_create(uint8_t type, uint16_t size)
{
	dj_int_array *arr;
	uint16_t byteSize = size;

	if ( type==T_BOOLEAN || type==T_BYTE || type==T_CHAR) { byteSize = size; } else
	if ( type==T_SHORT ) byteSize = size*sizeof(int16_t); else
	if ( type==T_INT ) byteSize = size*sizeof(int32_t); else
	{
        DEBUG_LOG("Unsupported array type: %d\n", type);
		dj_panic(DJ_PANIC_UNIMPLEMENTED_FEATURE);
	}

	// allocate array
	arr = (dj_int_array*)dj_mem_alloc( byteSize + sizeof(dj_int_array), CHUNKID_INTARRAY );

    // let the caller deal with it
	if(arr == nullref) return nullref;

	// init array to zeroes
	memset(arr->data.bytes, 0, byteSize);

	// set array size
	arr->array.length = size;

	// set array type
	arr->type = type;

	// C be a harsh mistress!
	return arr;
}

/**
 * Destroys an integer array
 */
void dj_int_array_destroy(dj_int_array *array)
{
	dj_mem_free(array);
}

/**
 * Creates a new reference array.
 * @param runtime_class_id
 */
dj_ref_array *dj_ref_array_create(runtime_id_t runtime_class_id, uint16_t size)
{
	dj_ref_array *arr;

	// allocate array
	arr = (dj_ref_array*)dj_mem_alloc( size * sizeof(ref_t) + sizeof(dj_ref_array), CHUNKID_REFARRAY );

    // let the caller deal with out of memory
	if(arr == nullref) return nullref;

	// set array size
	arr->array.length = size;

	// set array runtime class
	arr->runtime_class_id = runtime_class_id;

	// init array to zeroes
	memset(arr->refs, 0, size*sizeof(ref_t));

	// C be a harsh mistress!
	return arr;

}

/**
 * Destroys a reference array
 */
void dj_ref_array_destroy(dj_ref_array *array)
{
	dj_mem_free(array);
}

void dj_ref_array_updatePointers(dj_ref_array *array)
{
	int i;
	for (i=0; i<array->array.length; i++)
		array->refs[i] = dj_mem_getUpdatedReference(array->refs[i]);
}
