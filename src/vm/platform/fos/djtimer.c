/*
 *	djtimer.c
 *
 *	Copyright (c) 2008 CSIRO, Delft University of Technology.
 *
 *	This file is part of Darjeeling.
 *
 *	Darjeeling is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Darjeeling is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */
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
