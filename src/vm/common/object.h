#ifndef __object__
#define __object__

#include "types.h"
#include "config.h"

dj_object *dj_object_create(runtime_id_t type, int nr_refs, int non_ref_size);
runtime_id_t dj_object_getRuntimeId(dj_object * object);

#endif
