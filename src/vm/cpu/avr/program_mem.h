#ifndef __program_mem_h
#define __program_mem_h

#include <avr/pgmspace.h>

typedef unsigned int dj_di_pointer;

extern char DJ_SYSTEM_INFUSION_NAME[];

#define DJ_DI_NOT_SET -1
#define DJ_DI_NOT_FOUND -2

#define dj_di_getU8(pointer)  (pgm_read_byte_far(pointer))
#define dj_di_getU16(pointer) (pgm_read_word_far(pointer))
#define dj_di_getU32(pointer) (pgm_read_dword_far(pointer))
#define dj_di_getLocalId(pointer) ((dj_local_id){pgm_read_byte_far(pointer),pgm_read_byte_far(pointer+1)})

#endif
