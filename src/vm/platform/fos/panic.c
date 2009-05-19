/*
 *	panic.c
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
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>

#include "fos/kernel.h"
#include "fos/leds.h"
#include "fos/timer.h"

#include "common/panic.h"

#include "config.h"

void dj_panic(int32_t panictype)
{
    while (1)
	{
        switch(panictype)
        {
            case DJ_PANIC_OUT_OF_MEMORY:
                puts_P(PSTR("Darjeeling panic: out of memory\n"));
                break;
            case DJ_PANIC_ILLEGAL_INTERNAL_STATE:
                puts_P(PSTR("Darjeeling panic: illegal internal state\n"));
                break;
            case DJ_PANIC_UNIMPLEMENTED_FEATURE:
                puts_P(PSTR("Darjeeling panic: unimplemented feature\n"));
                break;
            case DJ_PANIC_UNCAUGHT_EXCEPTION:
                puts_P(PSTR("Darjeeling panic: uncaught exception\n"));
                break;
            default:
                puts_P(PSTR("Darjeeling panic: unknown panic type\n"));
                break;
        }

        // GS-15/10/2008-17:16(AEST) I disable the fos thread summmary
        // here  because fos_thread_print consumes  40 stack  bytes by
        // itself, and calls printf_P that uses another 43. With those
        // 83  bytes, the  stack estimator  thinks we  are at  risk to
        // overflow the stack
        /*
        puts_P(PSTR("FOS thread summary:\n"));
        fos_thread_print();
        */

        {
            int i;
            for(i=0;i<3;i++)
            {
                fos_leds_red_on();
                fos_timer_mspin(200);
                fos_leds_red_off();
                fos_timer_mspin(200);
            }
            for(i=0;i<3;i++)
            {
                fos_leds_red_on();
                fos_timer_mspin(1000);
                fos_leds_red_off();
                fos_timer_mspin(200);
            }

        }
    }
}
