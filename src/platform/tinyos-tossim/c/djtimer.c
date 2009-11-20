#include <stddef.h>

#include "djtimer.h"
#include "types.h"

#include "tossim.h"

void dj_timer_init()
{
}

int32_t dj_timer_getTimeMillis()
{
	return tossim_getTime();
}
