#ifndef __config_h
#define __config_h

// define heap size
#define RUNSIZE 128
#define HEAPSIZE 2048

// define wether to pack structs (this is fine on all AVR targets)
#define PACK_STRUCTS

/* Please see common/debug.h */
//#define DARJEELING_DEBUG
//#define DARJEELING_DEBUG_TRACE
//#define DARJEELING_DEBUG_CHECK_HEAP_SANITY
//#define DARJEELING_DEBUG_PERFILE
#define DARJEELING_PRINTF printf_P
#define DARJEELING_PGMSPACE_MACRO PSTR


#endif
