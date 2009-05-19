/*
 *	radio.c
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
#include <string.h>
#include <stdio.h>

#include "common/array.h"
#include "common/vmthread.h"
#include "common/global_id.h"
#include "common/vm.h"
#include "common/execution/execution.h"
#include "common/heap/heap.h"

// declaration of the functions implemented here
#include "radio.h"

// generated by the infuser
#include "base_definitions.h"


static dj_object * listener;

#define MESSAGE_BUFFER_SIZE 4
static fos_mac_message_t message_buffer[MESSAGE_BUFFER_SIZE];

static int message_buffer_index;
static fos_mac_message_t message;

// implemented in javax.radio.Radio
// hacky, I know :D
void wake_radio();

void dj_radio_init()
{
	message_buffer_index = 0;
}

dj_object * dj_radio_get_listener()
{
	return listener;
}

void dj_radio_receive_message(fos_mac_message_t * message)
{
	if (message_buffer_index<MESSAGE_BUFFER_SIZE)
	{
		message_buffer[message_buffer_index] = *message;
		message_buffer_index++;
	}
	wake_radio();
}

int dj_radio_get_nr_messages()
{
	return message_buffer_index;
}

uint8_t dj_radio_send_message(fos_mac_message_t * message)
{
	return (fos_mac_bsend(0, message) == 1) && (message->ack == 1);
}

int dj_radio_bufferHasNext()
{
	return message_buffer_index>0;
}

int dj_radio_bufTopAddress()
{
	return message_buffer[message_buffer_index-1].addr;
}

int dj_radio_bufTopGroup()
{
	return message_buffer[message_buffer_index-1].group;
}

int dj_radio_bufTopType()
{
	return message_buffer[message_buffer_index-1].type;
}

int dj_radio_bufTopLength()
{
    return message_buffer[message_buffer_index-1].length;
}


dj_int_array * dj_radio_bufTopBytes()
{
	int length = message_buffer[message_buffer_index-1].length;

	dj_int_array * arr = dj_int_array_create(T_BYTE, length);
	memcpy(arr->data.bytes, &(message_buffer[message_buffer_index-1].data), length);
	return arr;
}

void dj_radio_bufPop()
{
	if (message_buffer_index>0)
		message_buffer_index--;
}

void dj_radio_set_channel(uint8_t channel)
{
	fos_mac_set_radio_channel_parms(0, channel, 3);
}

uint8_t dj_radio_send_bytes(uint16_t addr, uint8_t type, uint8_t group, uint8_t length, uint8_t data[])
{
	message.addr = addr;
	message.type = type;
	message.group = group;
    message.length = length;
	if (data!=nullref)
		memcpy(&(message.data), data, length);

	// send message (blocking, so passing the message struct here should be ok)
	return dj_radio_send_message(&message);
}
