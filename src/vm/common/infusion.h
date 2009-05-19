#ifndef __infusion__
#define __infusion__

#include "common/types.h"
#include "common/parse_infusion.h"
#include "common/debug.h"

#include "config.h"

dj_infusion *dj_infusion_create(dj_di_pointer staticFieldInfo, int nr_referenced_infusions);
void dj_infusion_destroy(dj_infusion *infusion);
void dj_infusion_markRootSet(dj_infusion *infusion);
void dj_infusion_updatePointers(dj_infusion *infusion);
int dj_infusion_getReferencedInfusionIndex(dj_infusion *infusion, dj_infusion *searchInfusion);

dj_di_pointer dj_infusion_getMethodImplementation(dj_infusion *infusion, int entity_id);
dj_di_pointer dj_infusion_getClassDefinition(dj_infusion * infusion, int entity_id);
dj_di_pointer dj_infusion_getString(dj_infusion * infusion, int entity_id);

// functions that need to be quick and should be inlined
static inline dj_infusion * dj_infusion_resolve(dj_infusion *infusion, int id)
{
    dj_infusion *result;
	result=(id==0)?infusion:infusion->referencedInfusions[id-1];
    return result;
}

#endif
