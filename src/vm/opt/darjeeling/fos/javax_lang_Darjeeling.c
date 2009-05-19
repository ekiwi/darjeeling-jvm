/*
 *	javax_lang_Darjeeling.c
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
#include <string.h>
#include <stdlib.h>

#include <avr/pgmspace.h>

#include "fos/temp.h"
#include "fos/rtc.h"
#include "fos/power.h"
#include "fos/adc.h"

/* Sensor Includes */
#include "fos/boards/humiditytemp.h"
#include "fos/boards/echo.h"
#include "fos/boards/pulsecounter.h"

// generated at infusion time
#include "base_definitions.h"

#include "common/execution/execution.h"
#include "common/heap/heap.h"
#include "common/djtimer.h"

// generated at infusion time
#include "base_definitions.h"


// void javax.darjeeling.Darjeeling.assertTrue(int, boolean)
void javax_darjeeling_Darjeeling_void_assertTrue_int_boolean()
{
	// pop argument from the stack
	int16_t value = dj_exec_stackPopShort();
	int32_t id = dj_exec_stackPopInt();
	if (value==0)
		printf_P(PSTR("ASSERT[%3d] FAILED\n"), id);
	else
		printf_P(PSTR("ASSERT[%3d] PASSED\n"), id);

}

// void javax.darjeeling.Darjeeling.gc()
void javax_darjeeling_Darjeeling_void_gc()
{
	dj_mem_gc();
}

// void javax.darjeeling.Darjeeling.print(java.lang.String)
void javax_darjeeling_Darjeeling_void_print_java_lang_String()
{
	char *str = (char*)REF_TO_VOIDP(dj_exec_stackPopRef());
	printf_P(PSTR("%s"), str);
}

// void javax.darjeeling.Darjeeling.print(int)
void javax_darjeeling_Darjeeling_void_print_int()
{
	int32_t value = dj_exec_stackPopInt();
	printf_P(PSTR("%ld"), value);
}


// int javax.darjeeling.Darjeeling.getTime()
void javax_darjeeling_Darjeeling_int_getTime()
{
	dj_exec_stackPushInt(dj_timer_getTimeMillis());
}


// int javax.darjeeling.Darjeeling.getMemFree()
void javax_darjeeling_Darjeeling_int_getMemFree()
{
	dj_exec_stackPushInt(dj_mem_getFree());
}


// int javax.darjeeling.Darjeeling.getNodeId()
void javax_darjeeling_Darjeeling_int_getNodeId()
{
	dj_exec_stackPushInt(NODEID);
}


// int javax.darjeeling.Darjeeling.getBoardTemperature()
void javax_darjeeling_Darjeeling_int_getBoardTemperature()
{
	fos_temp_t temp;
	fos_temp_read(&temp, 0);
	dj_exec_stackPushInt(temp.deg);
}

// int java.lang.Darjeeling.getSecond()
void javax_darjeeling_Darjeeling_int_getSecond()
{
	rtc_calendar_time_t time;
	fos_rtc_time_get_calendar(&time);
	dj_exec_stackPushInt(time.second);
}

// int java.lang.Darjeeling.getMinute()
void javax_darjeeling_Darjeeling_int_getMinute()
{
	rtc_calendar_time_t time;
	fos_rtc_time_get_calendar(&time);
	dj_exec_stackPushInt(time.minute);
}

// int java.lang.Darjeeling.getHour()
void javax_darjeeling_Darjeeling_int_getHour()
{
	rtc_calendar_time_t time;
	fos_rtc_time_get_calendar(&time);
	dj_exec_stackPushInt(time.hour);
}


// int java.lang.Darjeeling.getBatteryVoltage()
void javax_darjeeling_Darjeeling_int_getBatteryVoltage()
{
	uint16_t    volts;
	volts = fos_power_battery_voltage();
	dj_exec_stackPushInt((int32_t)volts);
}

// int javax.darjeeling.Darjeeling.getVoltage()
void javax_darjeeling_Darjeeling_int_getVoltage()
{
	javax_darjeeling_Darjeeling_int_getBatteryVoltage();
}

// int java.lang.Darjeeling.getSolarVoltage()
void javax_darjeeling_Darjeeling_int_getSolarVoltage()
{
	uint16_t    volts;
	volts = fos_power_solar_voltage();
	dj_exec_stackPushInt((int32_t)volts);
}

// int java.lang.Darjeeling.getSolarCurrent()
void javax_darjeeling_Darjeeling_int_getSolarCurrent()
{
	uint16_t        current;

	current = fos_power_solar_current();
	dj_exec_stackPushInt((int32_t)current);
}

// int java.lang.Darjeeling.getADC()
void javax_darjeeling_Darjeeling_int_getADC_short()
{
	// pop byte argument off the stack
	int32_t arg = dj_exec_stackPopInt();

	// read value from the ADC
	uint16_t result = -1;
	fos_adc_single_read(0,arg, &result);

	// push result on the stack
	dj_exec_stackPushInt((int32_t)result);
}

// int javax.darjeeling.Darjeeling.random()
void javax_darjeeling_Darjeeling_int_random()
{
	dj_exec_stackPushInt(rand());
}


// short javax.darjeeling.Darjeeling.getNrThreads()
void javax_darjeeling_Darjeeling_short_getNrThreads()
{
	dj_exec_stackPushShort(dj_vm_countThreads(dj_exec_getVM()));
}


// java.lang.Thread javax.darjeeling.Darjeeling.getThread(short)
void javax_darjeeling_Darjeeling_java_lang_Thread_getThread_short()
{
	dj_thread *thread;
	int index = dj_exec_stackPopShort();

	// check for out of bounds
	if ( (index<0) || (index>=dj_vm_countThreads(dj_exec_getVM())) )
		dj_exec_throwHere(dj_vm_createSysLibObject(dj_exec_getVM(), BASE_CDEF_java_lang_IndexOutOfBoundsException));
	else
	{
		thread = dj_vm_getThread(dj_exec_getVM(), index);
		dj_exec_stackPushRef(VOIDP_TO_REF(thread));
	}
}

// routines to access the Springbrook sensors


// short java.lang.Darjeeling.getEcho()
void javax_darjeeling_Darjeeling_short_getEcho_short()
{
    static uint8_t initd = 0;

    // lazy initialization
    if (initd == 0) {
        fos_echo_init();
        initd = 1;
    }

	// pop argument off the stack
	int32_t arg = dj_exec_stackPopShort();

	// read value from the Echo interface
	uint16_t result = -1;
	fos_echo_readraw(arg, &result);

	// push result on the stack
	dj_exec_stackPushShort(result);
}

static uint8_t humtemp_initd = 0;

// short java.lang.Darjeeling.getHumidity()
void javax_darjeeling_Darjeeling_short_getHumidity()
{
    // lazy initialization
    if (humtemp_initd == 0) {
        fos_humidity_temp_init(FOS_HUMIDITY_TEMP_HIGH_RESOLUTION);
        humtemp_initd = 1;
    }

	// read value from the humidity sensor
	uint16_t result = -1;
	fos_humidity_temp_measure(FOS_HUMIDITY_TEMP_CMD_MEASURE_HUMIDITY, &result);

	// push result on the stack
	dj_exec_stackPushShort(result);
}

// short java.lang.Darjeeling.getTemperature()
void javax_darjeeling_Darjeeling_short_getTemperature()
{
    // lazy initialization
    if (humtemp_initd == 0) {
        fos_humidity_temp_init(FOS_HUMIDITY_TEMP_HIGH_RESOLUTION);
        humtemp_initd = 1;
    }

	// read value from the humidity sensor
	uint16_t result = -1;
	fos_humidity_temp_measure(FOS_HUMIDITY_TEMP_CMD_MEASURE_TEMP, &result);

	// push result on the stack
	dj_exec_stackPushShort(result);
}


// int java.lang.Darjeeling.getPulseCounter()
void javax_darjeeling_Darjeeling_int_getPulseCounter()
{
    static uint8_t initd = 0;

    // lazy initialization
    if (initd == 0) {
        fos_pulsecounter_init();
        initd = 1;
    }

	// read value from the pulse counter
	uint32_t result = -1;
	fos_pulsecounter_reset();
    fos_pulsecounter_read(&result);

	// push result on the stack
	dj_exec_stackPushInt(result);
}

// int java.lang.Darjeeling.setExpansionPower(short on)
void javax_darjeeling_Darjeeling_void_setExpansionPower_short()
{
	// pop byte argument off the stack
	int32_t arg = dj_exec_stackPopShort();

    fos_power_eb(arg);
}
