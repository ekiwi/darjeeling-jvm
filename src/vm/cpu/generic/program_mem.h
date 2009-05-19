#ifndef __program_mem_h
#define __program_mem_h

#include <stdint.h>

typedef unsigned long dj_di_pointer;

#define DJ_DI_NOT_SET -1
#define DJ_DI_NOT_FOUND -2

#define dj_di_getU8(pointer)  (*(uint8_t*) (pointer))
#define dj_di_getU16(pointer) (*(uint16_t*)(pointer))
#define dj_di_getU32(pointer) (*(uint32_t*)(pointer))
#define dj_di_getLocalId(pointer) (*(dj_local_id*)(pointer))

#endif
