#include <stdio.h>
#include <stdlib.h>

#include "panic.h"
#include "nesc.h"

#include "config.h"

void dj_panic(int32_t panictype)
{
    while (1)
	{
        switch(panictype)
        {
            case DJ_PANIC_OUT_OF_MEMORY:
            	nesc_printf("PANIC: OUT OF MEMORY\n");
                break;
            case DJ_PANIC_ILLEGAL_INTERNAL_STATE:
            	nesc_printf("PANIC: ILLEGAL INTERNAL STATE\n");
                break;
            case DJ_PANIC_UNIMPLEMENTED_FEATURE:
            	nesc_printf("PANIC: UNIMPLEMENTED FEATURE\n");
                break;
            case DJ_PANIC_UNCAUGHT_EXCEPTION:
            	nesc_printf("PANIC: UNCAUGHT EXCEPTION\n");
                break;
            default:
            	nesc_printf("PANIC: UNKNOWN TYPE\n");
                break;
        }
    }
}
