#include <stddef.h>

#include "djtimer.h"
#include "types.h"

#include "nesc.h"

void dj_timer_init()
{
}

int64_t dj_timer_getTimeMillis()
{
	return (int64_t)nesc_getTime();
}
