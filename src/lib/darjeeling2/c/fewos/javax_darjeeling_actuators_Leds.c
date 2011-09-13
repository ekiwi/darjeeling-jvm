/*
 * javax_darjeeling_actuators_Leds.c
 * 
 * Copyright (c) 2008-2010 CSIRO, Delft University of Technology.
 * 
 * This file is part of Darjeeling.
 * 
 * Darjeeling is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Darjeeling is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <stdio.h>

#include "array.h"
#include "execution.h"
#include "jlib_base.h"
#include "gpio.h"

#if (BOARD==MATRIXONE)
#define NUM_LEDS 4
static int portNum[] = {4, 4, 4, 4, 5, 5};
static uint8_t bitNum[] = {0x08, 0x10, 0x20, 0x40, 0x10, 0x20};
#define LED_ENABLE_IDX		4
#define LED_GROUP_SEL_IDX	5

#elif (BOARD==EXPERIMENTER)
#define NUM_LEDS 2
static int portNum[] = {1, 1};
static uint8_t bitNum[] = {0x01, 0x02};

#elif (BOARD==FEUERWHERE)
#define NUM_LEDS 8
static int portNum[] = {8, 8, 8, 8, 8, 8, 8, 8};
static uint8_t bitNum[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

#else
#define NUM_LEDS 0
static int portNum[] = {};
static uint8_t bitNum[] = {};
#endif

void initIO( int nr ) {
	int err;
	uint8_t data;
	// Set port select = I/O
	data = gpioRead( portNum[nr], gpioPortSelection );
	err = gpioWrite( portNum[nr], gpioPortSelection, data & ~bitNum[nr] );
	// Set direction = output.
	data = gpioRead( portNum[nr], gpioDirection );
	err = gpioWrite( portNum[nr], gpioDirection, data | bitNum[nr] );
	// Set drive strength = full.
	data = gpioRead( portNum[nr], gpioDriveStrength );
	err = gpioWrite( portNum[nr], gpioDriveStrength, data | bitNum[nr] );
}

void setIO( int nr, int on ) {
	int err;
	uint8_t data;
	// Set LED on/off
	data = gpioRead( portNum[nr], gpioOutput );
	if( on ) {
		err = gpioWrite( portNum[nr], gpioOutput, data | bitNum[nr] );
	} else {
		err = gpioWrite( portNum[nr], gpioOutput, data & ~bitNum[nr] );
	}
}

void initLed() {
	int idx=0;

#if (BOARD==MATRIXONE)
	initIO(LED_ENABLE_IDX);
	initIO(LED_GROUP_SEL_IDX);
	setIO(LED_ENABLE_IDX, 1);		// LED enable
	setIO(LED_GROUP_SEL_IDX, 1);	// Group A = LED 1,3,5,7
#endif

	for( idx=0; idx < NUM_LEDS; idx++ ) {
		initIO(idx);
	}
	for( idx=0; idx < NUM_LEDS; idx++ ) {
		setIO(idx, 0);
	}
}

//short javax.darjeeling.actuators.Leds.getNrLeds()
void javax_darjeeling_actuators_Leds_short_getNrLeds()
{
	dj_exec_stackPushShort(NUM_LEDS);
}

// void javax.darjeeling.actuators.Leds.set(short, boolean)
void javax_darjeeling_actuators_Leds_void_set_short_boolean()
{
	uint16_t on = dj_exec_stackPopShort();
	// Get the led index argument
	uint16_t nr = dj_exec_stackPopShort();
	if( nr < NUM_LEDS ) {
		setIO(nr, on);
	}
}
