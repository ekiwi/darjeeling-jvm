/*
 *	DarjeelingC.nc
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
#include "Timer.h"

#include <stdarg.h>
#include <stdio.h>

#include "darjeeling.h"

#include "message.h"
#include "platform_message.h"

#include "dj_tos_message.h"

#include "tosconfig.h"

module DarjeelingC
{
	uses
	{
		interface Boot;
	    interface Leds;
		interface Timer<TMilli> as Timer;

		interface SplitControl as RadioControl;
		interface AMSend as RadioSend;
		interface Receive as RadioReceive;
		interface Packet as RadioPacket;
	    interface PacketAcknowledgements;

		interface CC1000Control as CC1000;

#ifdef TOS_SERIAL
		interface StdControl as UartControl;
		interface UartByte;
		interface UartStream;
#endif

		interface LowPowerListening;
	}
}

implementation
{

	#define bufferSize 4

	message_t radioPacket;
	tos_message_t messageBuffer[bufferSize];
	int bufferPos = 0;

	bool radioLocked, ackPending;
	bool wasAcked;

	int nesc_printf(char * msg) @C() @spontaneous()
	{
#ifdef TOS_SERIAL
		if (call UartStream.send(msg, strlen(msg)) == SUCCESS)
			return 0;
		else
			return -1;
#endif

#ifndef TOS_SERIAL
		// notify the VM
		dj_notifySerialSendDone();
#endif
	}

	void nesc_setLed(int nr, int on) @C() @spontaneous()
	{
#ifdef TOS_LEDS
		if (nr==0&&on==0) call Leds.led0Off();
		if (nr==0&&on==1) call Leds.led0On();
		if (nr==1&&on==0) call Leds.led1Off();
		if (nr==1&&on==1) call Leds.led1On();
		if (nr==2&&on==0) call Leds.led2Off();
		if (nr==2&&on==1) call Leds.led2On();
#endif
	}

	uint32_t nesc_getTime() @C() @spontaneous()
	{
		return call Timer.getNow();
	}

	uint16_t nesc_getMaxPayloadLength() @C() @spontaneous()
	{
		return call RadioPacket.maxPayloadLength();
	}

	int nesc_send(const char * message, int16_t receiverId, uint16_t length) @C() @spontaneous()
	{
		tos_message_t * tosmessage;

		// check for lock
		if (radioLocked) return -1;

		// get a pointer to the payload
		tosmessage = (tos_message_t*)call RadioPacket.getPayload(&radioPacket, (uint8_t)sizeof(tos_message_t));

		// data is NULL if length>max payload
		if (tosmessage == NULL) return -2;

		// copy data into the radioPacket
		memcpy(tosmessage->payload, message, length);

		// if not broadcast, request ack
		if (receiverId==0xffff)
			ackPending = FALSE;
		else
			ackPending = (call PacketAcknowledgements.requestAck(&radioPacket) == SUCCESS);

		// send radioPacket
		if (call RadioSend.send(receiverId, &radioPacket, sizeof(tos_message_t)) == SUCCESS)
		{
			radioLocked = TRUE;
			return 0;
		}

		return -3;

	}

	uint16_t nesc_peekMessageLength() @C() @spontaneous()
	{
		if (bufferPos>0)
		{
			return sizeof(tos_message_t);
		}
		else
			return 0;
	}

	void * nesc_popMessageBuffer() @C() @spontaneous()
	{
		int len;
		if (bufferPos>0)
		{
			len = nesc_peekMessageLength();
			bufferPos--;
			return (void *)(messageBuffer[bufferPos].payload);
		}
		else
			return NULL;
	}

	int nesc_getNrMessages() @C() @spontaneous()
	{
		return bufferPos;
	}

	int nesc_wasAcked() @C() @spontaneous()
	{
		return wasAcked?1:0;
	}

	task void run()
	{
		uint32_t sleepTime = dj_run();

		if (sleepTime==0) post run(); else
		if (sleepTime>0) call Timer.startOneShot(sleepTime); else
		if (sleepTime<0) call Timer.startOneShot(1000);
	}

	event void Timer.fired()
	{
		post run();
	}

	event void Boot.booted()
	{
		dj_init();
#ifdef TOS_SERIAL
		call UartControl.start();
#endif
		call RadioControl.start();
	}

	event void RadioControl.startDone(error_t error)
	{
		call CC1000.setRFPower(1);
		call LowPowerListening.setLocalSleepInterval(85);
		post run();
	}

	event void RadioControl.stopDone(error_t error)
	{
	}

	event message_t* RadioReceive.receive(message_t * message, void * payload, uint8_t len)
	{
		// push message into the buffer
		if (bufferPos<bufferSize)
		{
			messageBuffer[bufferPos] = *((tos_message_t*)payload);
			bufferPos++;
		}

		// notify the JVM
		dj_notifyRadioReceive();
		post run();

		return message;
	}

	event void RadioSend.sendDone(message_t* bufPtr, error_t error)
	{
	    if (&radioPacket == bufPtr)
	    {
			radioLocked = FALSE;

		    // record whether the last message was acknowledged
			if (ackPending)
		    	wasAcked = call PacketAcknowledgements.wasAcked(&radioPacket);

			dj_notifyRadioSendDone();
			post run();
	    }

	}

#ifdef TOS_SERIAL

	/**
	 * Uart 'send done' event. Signals that the last serial write command completed succesfully. The VM has to be notified
	 * so that it may unblock the thread waiting for this to complete.
	 */
	async event void UartStream.sendDone( uint8_t* buf, uint16_t len, error_t error )
	{
		// notify the VM
		dj_notifySerialSendDone();

		// wake up the VM if it was sleeping
		post run();
	}

	async event void UartStream.receivedByte( uint8_t byte )
	{
		// not implemented
	}

	async event void UartStream.receiveDone( uint8_t* buf, uint16_t len, error_t error )
	{
		// not implemented
	}

#endif

}

