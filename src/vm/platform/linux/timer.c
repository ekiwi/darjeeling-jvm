#include <time.h>

#include "common/types.h"

void dj_timer_init()
{

}

uint32_t dj_timer_getTimeMillis()
{
	return clock() / (CLOCKS_PER_SEC / 1000);
}
