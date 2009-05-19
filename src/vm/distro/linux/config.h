#ifndef __config_h
#define __config_h

#define MEMSIZE 2048
#define RUNSIZE 128

#define PACK_STRUCTS
// #define ALIGN_16

/* Please see common/debug.h */

#define DARJEELING_DEBUG
//#define DARJEELING_DEBUG_TRACE
// #define DARJEELING_DEBUG_CHECK_HEAP_SANITY
// #define DARJEELING_DEBUG_PERFILE
#define DARJEELING_PRINTF printf
#define DARJEELING_PGMSPACE_MACRO

#endif
