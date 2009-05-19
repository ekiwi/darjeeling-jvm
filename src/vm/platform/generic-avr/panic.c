#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>

#include "common/panic.h"

#include "config.h"

void dj_panic(int32_t panictype)
{
    while (1)
	{
        switch(panictype)
        {
            case DJ_PANIC_OUT_OF_MEMORY:
                break;
            case DJ_PANIC_ILLEGAL_INTERNAL_STATE:
                break;
            case DJ_PANIC_UNIMPLEMENTED_FEATURE:
                break;
            case DJ_PANIC_UNCAUGHT_EXCEPTION:
                break;
            default:
                break;
        }
    }
}
