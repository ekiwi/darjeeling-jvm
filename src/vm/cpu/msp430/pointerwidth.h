#ifndef __pointerwidth_h
#define __pointerwidth_h

typedef void* ref_t;
#define nullref ((ref_t)0)

#define REF_TO_VOIDP(ref)  (ref)
#define VOIDP_TO_REF(ref)  (ref)

#define REF_TO_UINT32(ref) ((uint32_t)((uint16_t)ref))
#define UINT32_TO_REF(ref) ((void *)((uint16_t)ref))


#endif // __pointerwidth_h
