#include <stddef.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "fos/timer.h"

#include "common/djtimer.h"
#include "common/types.h"

int32_t ticks;

#define MILLISBOUND 8000

void * Timer_tick(void * arg)
{
	ticks++;

    //dummy return statement to conform to FOS API
    return NULL;
}

void dj_timer_init()
{
	fos_timer_config(FOS_TIMER_1, MILLISBOUND, Timer_tick, nullref,  FOS_TIMER_MS | FOS_TIMER_REPEAT);
	fos_timer_start(FOS_TIMER_1);
}

int32_t dj_timer_getTimeMillis()
{
	return (ticks*MILLISBOUND) + MILLISBOUND - (int32_t)fos_timer_time_remaining(FOS_TIMER_1);
}
