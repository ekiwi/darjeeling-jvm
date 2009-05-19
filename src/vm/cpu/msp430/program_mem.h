#ifndef __program_mem__
#define __program_mem__

#include "common/types.h"

typedef void * dj_di_pointer;

#define DJ_DI_NOT_SET -1
#define DJ_DI_NOT_FOUND -2

#define dj_di_getU8(pointer)  (*(uint8_t*) (pointer))
#define dj_di_getU16(pointer) ( ((*(uint8_t*) (pointer+1))<<8) | (*(uint8_t*) (pointer)) )
#define dj_di_getU32(pointer) (\
	((*(uint8_t*) (pointer+3))<<24) |\
	((*(uint8_t*) (pointer+2))<<16) |\
	((*(uint8_t*) (pointer+1))<<8) |\
	(*(uint8_t*) (pointer)) )
#define dj_di_getLocalId(pointer) ((dj_local_id){dj_di_getU8(pointer),dj_di_getU8(pointer+1)})

#endif
