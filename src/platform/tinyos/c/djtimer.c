#include <stddef.h>

#include "djtimer.h"
#include "types.h"

#include "nesc.h"

void dj_timer_init()
{
}

dj_time_t dj_timer_getTimeMillis()
{
	return (dj_time_t)nesc_getTime();
}
