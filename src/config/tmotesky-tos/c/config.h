#ifndef __config_h
#define __config_h
#include <sys/types.h>

// define heap size
#define RUNSIZE 32
#define HEAPSIZE 2048

//Use 64-bit values to store time
typedef int64_t dj_time_t;

// define wether to pack structs (this is fine on all AVR targets)
// #define PACK_STRUCTS

/* Please see common/debug.h */
//#define DARJEELING_DEBUG
//#define DARJEELING_DEBUG_TRACE
//#define DARJEELING_DEBUG_CHECK_HEAP_SANITY
//#define DARJEELING_DEBUG_PERFILE
//#define DARJEELING_PRINTF nesc_printf
#define DARJEELING_PRINTF(...)
#define DARJEELING_PGMSPACE_MACRO

#endif
