#ifndef __program_mem_h
#define __program_mem_h

#include <stdint.h>

typedef void * dj_di_pointer;

#define DJ_DI_NOT_SET -1
#define DJ_DI_NOT_FOUND -2

#define dj_di_getU8(pointer)  (*(uint8_t*) ((int)(pointer)))
#define dj_di_getU16(pointer) ((uint16_t)((dj_di_getU8((int)(pointer)+1)<<8) | dj_di_getU8(pointer)))

// #define dj_di_getU16(pointer) ( ((*(uint8_t*) (pointer+1))<<8) | (*(uint8_t*) (pointer)) )
//#define dj_di_getU16(pointer) ((((int)(pointer)&1)==1) ?
//		((uint16_t)((dj_di_getU8(pointer+1)<<8) | dj_di_getU8(pointer))) :
//		((uint16_t)((dj_di_getU8(pointer)<<8) | dj_di_getU8(pointer+1))) )

#define dj_di_getU32(pointer) ((uint32_t)(\
	((*(uint8_t*) ((pointer)+3))<<24) |\
	((*(uint8_t*) ((pointer)+2))<<16) |\
	((*(uint8_t*) ((pointer)+1))<<8) |\
	((*(uint8_t*) ((pointer)+0))) ))
#define dj_di_getLocalId(pointer) ((dj_local_id){dj_di_getU8(pointer), dj_di_getU8(pointer+1)})

#endif
