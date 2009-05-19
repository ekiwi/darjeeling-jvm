#include <stdint.h>

#include "common/execution/execution.h"
#include "common/vm.h"
#include "common/array.h"
#include "radio.h"

#include "fos/fos_msg.h"

#include "base_definitions.h"

short receiveThreadId = -1;

// void javax.radio.Radio._waitForMessage()
void javax_radio_Radio_void__waitForMessage()
{

	if (dj_radio_get_nr_messages()==0)
	{
		dj_thread * currentThread = dj_exec_getCurrentThread();
		receiveThreadId = currentThread->id;
		currentThread->status = THREADSTATUS_BLOCKED_FOR_IO;
		dj_exec_breakExecution();
	}

}


// void javax.radio.Radio.setChannel(short)
void javax_radio_Radio_void_setChannel_short()
{
	int16_t channel = dj_exec_stackPopShort();
	dj_radio_set_channel(channel);
}

// void javax.radio.Radio._broadcast(byte[])
void javax_radio_Radio_void__broadcast_byte__()
{
	// pop byte array
	dj_int_array *arr = (dj_int_array*)dj_exec_stackPopRef();

	dj_radio_send_bytes((int16_t)0xffff, 0, 0, (uint8_t) arr->array.length, (uint8_t*) arr->data.bytes);
}

// boolean javax.radio.Radio._send(short, byte[])
void javax_radio_Radio_boolean__send_short_byte__()
{
	// pop byte array
	dj_int_array *arr = (dj_int_array*)dj_exec_stackPopRef();
	int16_t address = dj_exec_stackPopShort();

	// return status of the MAC transmission
	dj_exec_stackPushShort( dj_radio_send_bytes(address, 0, 0, (uint8_t) arr->array.length, (uint8_t*) arr->data.bytes) );
}

// void javax.radio.Radio._init()
void javax_radio_Radio_void__init()
{
	dj_radio_init();
	dj_radio_set_channel(1);
}

// byte[] javax.radio.Radio._readBytes()
void javax_radio_Radio_byte____readBytes()
{
	dj_exec_stackPushRef(dj_radio_bufTopBytes());
	dj_radio_bufPop();
}

// byte javax.radio.Radio.getNumMessages()
void javax_radio_Radio_byte__getNumMessages()
{
	dj_exec_stackPushShort(dj_radio_get_nr_messages());
}

void javax_radio_Radio_short_getMaxMessageLength()
{
	dj_exec_stackPushShort(FOS_MSG_DATA_SIZE);
}

void wake_radio()
{
	dj_thread * thread;

	// check if the message is for us
	int16_t address = dj_radio_bufTopAddress();
	if (address==NODEID||address==(int16_t)0xffff)
	{
		thread = dj_vm_getThreadById(dj_exec_getVM(), receiveThreadId);
		if (thread!=NULL)
		{
			thread->status = THREADSTATUS_RUNNING;
			receiveThreadId = -1;
		}
	} else
	{
		// discard
		dj_radio_bufPop();
	}

}
