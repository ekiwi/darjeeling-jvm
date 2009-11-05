#include <stddef.h>

#include "djtimer.h"
#include "types.h"

#include "nesc.h"

void dj_timer_init()
{
}

int32_t dj_timer_getTimeMillis()
{
	return nesc_getTime();
}
