#ifndef __global_id__
#define __global_id__

#include "types.h"
#include "object.h"

#define LOCAL_INFUSION 0
#define SYSLIB_INFUSION_NAME = "sys";

inline dj_global_id dj_global_id_resolve(dj_infusion *infusion, dj_local_id local_id);
dj_local_id dj_global_id_mapToInfusion(dj_global_id dj_global_id, dj_infusion *infusion);

dj_global_id dj_global_id_get_parent(dj_global_id global_class_id);

char dj_global_id_equals(dj_global_id a, dj_global_id b);
char dj_global_id_implements(dj_global_id class, dj_global_id interface);
char dj_global_id_isEqualToOrChildOf(dj_global_id child, dj_global_id parent);
char dj_global_id_testClassType(dj_global_id refClass, dj_global_id testClass);
char dj_global_id_testType(void * ref, dj_local_id localClassId);
char dj_global_id_isJavaLangObject(dj_global_id class);

dj_global_id dj_global_id_lookupVirtualMethod(dj_global_id resolvedMethodDefId, dj_object *object);
runtime_id_t dj_global_id_getRuntimeClassId(dj_global_id dj_global_id);

dj_di_pointer dj_global_id_getClassDefinition(dj_global_id class);
dj_di_pointer dj_global_id_getMethodImplementation(dj_global_id dj_global_id);
dj_di_pointer dj_global_id_getString(dj_global_id dj_global_id);

#endif
