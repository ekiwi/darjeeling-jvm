
#include <stdint.h>

#include "fos/fos_msg.h"

#include "common/array.h"
#include "common/execution/execution.h"

// interface between DJ and FOS radio API
#include "radio.h"


// void javax.fleck.Radio.setChannel(short)
void javax_fleck_Radio_void_setChannel_short()
{
	int16_t channel = dj_exec_stackPopShort();
	dj_radio_set_channel(channel);
}

// short javax.fleck.Radio.getMessageLength()
void javax_fleck_Radio_short_getMessageLength()
{
	dj_exec_stackPushShort(FOS_MAC_MESSAGE_LEN);
}

// void javax.fleck.Radio._sendMessage(short, byte, byte, byte[])
void javax_fleck_Radio_void__sendMessage_short_byte_byte_byte__()
{
	// pop byte array
	dj_int_array *arr = (dj_int_array*)dj_exec_stackPopRef();

	// pop adress, message type
	uint8_t group = dj_exec_stackPopShort();
	uint8_t type = dj_exec_stackPopShort();
	uint16_t addr = dj_exec_stackPopShort();

	dj_radio_send_bytes(addr, type, group, (uint8_t) arr->array.length, (uint8_t*) arr->data.bytes);
}

// boolean javax.fleck.Radio.bufferHasMessages()
void javax_fleck_Radio_boolean_bufferHasMessages()
{
	dj_exec_stackPushShort(dj_radio_bufferHasNext());
}

// short javax.fleck.Radio.bufferGetTopAddress()
void javax_fleck_Radio_short_bufferGetTopAddress()
{
	dj_exec_stackPushShort(dj_radio_bufTopAddress());
}

// byte javax.fleck.Radio.bufferGetTopGroup()
void javax_fleck_Radio_byte_bufferGetTopGroup()
{
	dj_exec_stackPushShort(dj_radio_bufTopGroup());
}

// byte javax.fleck.Radio.bufferGetTopType()
void javax_fleck_Radio_byte_bufferGetTopType()
{
	dj_exec_stackPushShort(dj_radio_bufTopType());
}

// byte[] javax.fleck.Radio.bufferGetTopBytes()
void javax_fleck_Radio_byte___bufferGetTopBytes()
{
	dj_exec_stackPushRef(dj_radio_bufTopBytes());
}

// void javax.fleck.Radio.bufferPop()
void javax_fleck_Radio_void_bufferPop()
{
	dj_radio_bufPop();
}
