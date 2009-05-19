#ifndef __pointerwidth_h
#define __pointerwidth_h

#include <stdint.h>
#include <stddef.h>

#include "common/types.h"

// The null java reference
#define nullref ((ref_t)0)

typedef uint16_t ref_t;

extern char * ref_t_base_address;

// ref_t is now  only 16-bits wide for everyone. Thus,  we need a base
// address  to resolve  ref_t references  into 32-bits  pointers. This
// pointer is  actually declared in linux/main.c, and  assigned to the
// base address  of the heap *minus  a small quantity* to  allow us to
// distinguish  between null references  (ref_t ==  0) and  "the first
// object of the heap"

static inline void* REF_TO_VOIDP(ref_t ref) {return (ref != nullref ? (void*)((uint16_t)ref + ref_t_base_address) : NULL ) ;}
static inline ref_t VOIDP_TO_REF(void* ref) {return (ref != NULL ? (uint16_t)((char*)ref - ref_t_base_address) : nullref ) ;}

#define REF_TO_UINT32(ref) ((uint32_t)ref)
#define UINT32_TO_REF(ref) ((uint16_t)ref)


#endif // __pointerwidth_h
