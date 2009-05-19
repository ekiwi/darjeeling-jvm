#include <stddef.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "common/djtimer.h"
#include "common/types.h"

int32_t ticks;

#define MILLISBOUND 8000

void dj_timer_init()
{
}

int32_t dj_timer_getTimeMillis()
{
	return 0;
}
