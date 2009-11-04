#include <stdio.h>
#include <stdlib.h>

#include "execution.h"
#include "panic.h"

void dj_panic(int32_t panictype)
{
    switch(panictype)
    {
        case DJ_PANIC_OUT_OF_MEMORY:
#ifdef DEBUG
        	avr_serialPrint("PANIC: out of memory!\n");
#endif
            break;
        case DJ_PANIC_ILLEGAL_INTERNAL_STATE:
#ifdef DEBUG
        	avr_serialPrint("PANIC: illegal internal state!\n");
#endif
            break;
        case DJ_PANIC_UNIMPLEMENTED_FEATURE:
#ifdef DEBUG
        	avr_serialPrint("PANIC: unimplemented feature!\n");
#endif
            break;
        case DJ_PANIC_UNCAUGHT_EXCEPTION:
#ifdef DEBUG
        	avr_serialPrint("PANIC: uncaught exception!\n");
#endif
            break;
        case DJ_PANIC_MALFORMED_INFUSION:
#ifdef DEBUG
        	avr_serialPrint("PANIC: malformed infusion!\n");
#endif
            break;
        default:
            break;
    }
    exit(-1);
}
