#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "panic.h"
#include "nesc.h"

#include "config.h"
#include "tosconfig.h"
void dj_panic(int32_t panictype)
{
	switch(panictype)
	{
		case DJ_PANIC_OUT_OF_MEMORY:
			tossim_printf("PANIC: OUT OF MEMORY\n");
			break;
		case DJ_PANIC_ILLEGAL_INTERNAL_STATE:
			tossim_printf("PANIC: ILLEGAL INTERNAL STATE\n");
			break;
		case DJ_PANIC_UNIMPLEMENTED_FEATURE:
			tossim_printf("PANIC: UNIMPLEMENTED FEATURE\n");
			break;
		case DJ_PANIC_UNCAUGHT_EXCEPTION:
			tossim_printf("PANIC: UNCAUGHT EXCEPTION\n");
			break;
		case DJ_PANIC_ASSERTION_FAILURE:
			tossim_printf("PANIC: Assertion failed\n");
			break;
		default:
			tossim_printf("PANIC: UNKNOWN TYPE\n");
			break;
	}
	exit(-1);
}
