#include <stddef.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "djtimer.h"
#include "types.h"

#include "avr.h"

int32_t ticks;

void dj_timer_init()
{
	avr_timerInit();
}

int32_t dj_timer_getTimeMillis()
{
	return (int32_t)avr_millis();
}
