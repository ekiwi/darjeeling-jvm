#include <stdio.h>
#include <stdlib.h>

#include "common/execution/execution.h"
#include "common/panic.h"

void dj_panic(int32_t panictype)
{
    switch(panictype)
    {
        case DJ_PANIC_OUT_OF_MEMORY:
            printf("Darjeeling panic: out of memory\n");
            break;
        case DJ_PANIC_ILLEGAL_INTERNAL_STATE:
            printf("Darjeeling panic: illegal internal state\n");
            break;
        case DJ_PANIC_UNIMPLEMENTED_FEATURE:
            printf("Darjeeling panic: unimplemented feature\n");
            break;
        case DJ_PANIC_UNCAUGHT_EXCEPTION:
            printf("Darjeeling panic: uncaught exception\n");
            break;
        default:
            printf("Darjeeling panic: unknown panic type\n");
            break;
    }
    exit(-1);
}
