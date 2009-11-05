#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "panic.h"
#include "nesc.h"

#include "config.h"
#include "tosconfig.h"
void dj_panic(int32_t panictype)
{
    while (1)
	{
        switch(panictype)
        {
            case DJ_PANIC_OUT_OF_MEMORY:
            	tossim_printf(1, "PANIC: OUT OF MEMORY\n");
                break;
            case DJ_PANIC_ILLEGAL_INTERNAL_STATE:
            	tossim_printf(1, "PANIC: ILLEGAL INTERNAL STATE\n");
                break;
            case DJ_PANIC_UNIMPLEMENTED_FEATURE:
            	tossim_printf(1, "PANIC: UNIMPLEMENTED FEATURE\n");
                break;
            case DJ_PANIC_UNCAUGHT_EXCEPTION:
            	tossim_printf(1, "PANIC: UNCAUGHT EXCEPTION\n");
                break;
            default:
            	tossim_printf(1, "PANIC: UNKNOWN TYPE\n");
                break;
        }
    }
}
