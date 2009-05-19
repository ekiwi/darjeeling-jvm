/*
 *	radio.h
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
#ifndef __radio_h
#define __radio_h

#include "fos/mac.h"

#include "common/object.h"
#include "common/array.h"

// These functions are the interface between DJ and the FOS radio API


void dj_radio_init();
void dj_radio_receive_message(fos_mac_message_t * message);
uint8_t dj_radio_send_message(fos_mac_message_t * message);
uint8_t dj_radio_send_bytes(uint16_t addr, uint8_t type, uint8_t group, uint8_t length, uint8_t data[]);
int dj_radio_get_nr_messages();

int dj_radio_bufferHasNext();
int dj_radio_bufTopAddress();
int dj_radio_bufTopGroup();
int dj_radio_bufTopType();
int dj_radio_bufTopLength();
dj_int_array * dj_radio_bufTopBytes();
void dj_radio_bufPop();

void dj_radio_set_channel(uint8_t channel);

#endif // __radio_h
