#ifndef __config_h
#define __config_h

// define heap size
#define RUNSIZE 32
//#define HEAPSIZE 2048
#define HEAPSIZE 4096

// Don't pack but align. This is necessary for the crappy msp430-gcc compiler.
// #define PACK_STRUCTS
#define ALIGN_16

#include<stdarg.h>

void g301_serialVPrint(char * format, va_list arg);
void g301_serialPrintf(char * format, ...);

/* Please see common/debug.h */
//#define DARJEELING_DEBUG
//#define DARJEELING_DEBUG_TRACE
//#define DARJEELING_DEBUG_CHECK_HEAP_SANITY
//#define DARJEELING_DEBUG_PERFILE
//#define ASSEMBLY_DEBUGGING 1
//#define ASSEMBLY_DEBUG nesc_printf
#define DARJEELING_PRINTF g301_serialPrintf
#define DARJEELING_PGMSPACE_MACRO(x) (x)

#endif
