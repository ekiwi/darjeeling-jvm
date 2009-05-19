#include "common/types.h"
#include "sys/clock.h"

void dj_timer_init()
{
	clock_init();
}

uint32_t dj_timer_getTimeMillis()
{
	return (uint32_t)clock_time()*10;
}
