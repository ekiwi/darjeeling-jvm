#include <string.h>

#include "common/object.h"
#include "common/heap/heap.h"
#include "common/execution/execution.h"
#include "common/panic.h"
#include "common/heap/heap.h"

/**
 * Constructs a new object.
 * @param nr_refs the number of references to allocate
 * @param non_ref_size the number of bytes to allocate for integer values
 * @return a new object instance
 */
dj_object *dj_object_create(runtime_id_t type, int nr_refs, int non_ref_size)
{
	dj_object *ret;
	uint16_t size = nr_refs * sizeof(ref_t) + non_ref_size;

	ret = (dj_object*)dj_mem_alloc(size + sizeof(dj_object), type);

    if(ret == NULL) return NULL;

	// init fields to 0
	memset((void*)ret, 0, size);

	return ret;
}

runtime_id_t dj_object_getRuntimeId(dj_object * object)
{
	return dj_mem_getChunkId(object);
}
