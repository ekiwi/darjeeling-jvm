#include <stddef.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "djtimer.h"
#include "types.h"

#include "avr.h"

void dj_timer_init()
{
	avr_timerInit();
}

int64_t dj_timer_getTimeMillis()
{
	return (int64_t)avr_millis();
}
