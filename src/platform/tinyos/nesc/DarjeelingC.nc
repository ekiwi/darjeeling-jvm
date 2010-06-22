/*
 * DarjeelingC.nc
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
 
#include "Timer.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#include "darjeeling.h"

#include "message.h"
#include "platform_message.h"
#include "AM.h"

// TinyOS options and switches.
#include "tosconfig.h"

// Configuration specific.
#include "config.h"

// TinyOS message format.
#include "dj_tos_message.h"

// NESC<->C interface.
#include "darjeeling.h"

module DarjeelingC
{
	uses
	{
		// Required components.
		interface Boot;
		interface Timer<TMilli> as Timer;

		// Leds.
#ifdef TOS_LEDS
		interface Leds;
#endif

		// Common radio. 
#ifdef TOS_RADIO
		interface SplitControl as RadioControl;
		interface AMSend as RadioSend;
		interface Receive as RadioReceive;
		interface Packet as RadioPacket;
		interface PacketAcknowledgements;
		interface LowPowerListening;
#endif

		// CC1000 transceiver specifics.
#ifdef TOS_CC1000
		interface CC1000Control as CC1000;
#endif

		// Serial port.
#ifdef TOS_SERIAL
		interface StdControl as UartControl;
		interface UartStream;
		interface UartByte;
#endif

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

	// Forward declaration.
	task void run();
	int nesc_printf(const char * msg);
	
	/**
	 * Boot event. Is called after TinyOS is done initialising. It in turn initialises the VM, radio, and so on.
	 */
	event void Boot.booted()
	{

		// Initialise the virtual machine.
		dj_init();

		// Initialise the serial port.
#ifdef TOS_SERIAL
		call UartControl.start();
#endif

		// Initialise the radio.
#ifdef TOS_RADIO
		// The RadioControl will post run (the VM run task) when initialised. 
		call RadioControl.start();
#else
		// If the radio is not enabled, run will not be posted by it init method. Do it here. 
		post run();
#endif
		
	}
	
	/**
	 * The run task calls dj_run to do a single 'run' of the interpreter, which will interpret a set maximum number of instructions.
	 * This amount of instructions per run can be set in the config.h in your configuration.
	 */
	task void run()
	{
		dj_time_t sleepTime = dj_run();

		// There's at least one active thread, so start a new run as soon as possible.
		if (sleepTime == 0)
			post run();
		// There are no active threads, schedule a new run as soon as the first thread is due to wake up.		
		else if (sleepTime > 0)
			call Timer.startOneShot(sleepTime);
		else
			call Timer.startOneShot(100);
	}

	/**
	 * The main millisecond timer is used to sleep when Darjeeling has no active threads. When it fires, execution should
	 * be resumed.
	 */
	event void Timer.fired()
	{
		post run();
	}

#ifdef TOS_SERIAL
	
	/**
	 * Uart 'send done' event. Signals that the last serial write command completed succesfully. The VM has to be notified
	 * so that it may unblock the thread waiting for this to complete.
	 */
	async event void UartStream.sendDone( uint8_t* buf, uint16_t len, error_t error )
	{

		// notify the VM
		// dj_notifySerialSendDone();

		// wake up the VM if it was sleeping
		// Can cause problems when sending only a few bytes!
		//post run();
	}

	async event void UartStream.receivedByte(uint8_t byte)
	{
		dj_notifyUartReceiveByte(byte);

		post run();		
	}

	async event void UartStream.receiveDone( uint8_t* buf, uint16_t len, error_t error )
	{
		// not implemented
	}

#endif
	
#ifdef TOS_RADIO
	
	event void RadioControl.startDone(error_t error)
	{
		// set the RF power to 1 - our test bed is very dense :)
//		call CC1000.setRFPower(1);
//		call CC1000.setRFPower(2U);
//		call LowPowerListening.setLocalSleepInterval(85);
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
#endif
	
	/**
	 * Sets the status of one of the leds. Can be called from c (hence the @spontaneous).
	 * @param nr led number.
	 * @param on turn the led on (1) or off (0).
	 */
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
	
	/**
	 * Returns the current time in milliseconds. Can be called from c (hence the @spontaneous).
	 * @param nr led number.
	 * @param on turn the led on (1) or off (0).
	 */
	dj_time_t nesc_getTime() @C() @spontaneous()
	{
		return (dj_time_t)(call Timer.getNow());
	}

	/**
	 * Prints a string to the standard output (serial port). Can be called from c (hence the @spontaneous).
	 * @param msg null-terminated message string.
	 */
	int nesc_printf(const char * msg) @C() @spontaneous()
	{
#ifdef TOS_SERIAL

		if (call UartStream.send(msg, strlen(msg)) == SUCCESS)
			return 0;
		
		/*
		//blocking send
		while (*msg != 0)
		{
			call UartByte.send(*msg);
			msg++;
		}
		*/
#endif
		return -1;
	}

	void nesc_uartWriteByte(uint8_t byte) @C() @spontaneous()
	{
		call UartByte.send(byte);
	}

	/**
	 * Sends a message over the radio.
	 */
	int nesc_send(const char * message, int16_t receiverId, uint16_t length) @C() @spontaneous()
	{
#ifdef TOS_RADIO
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
#endif
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
	
	uint16_t nesc_getMaxPayloadLength() @C() @spontaneous()
	{
#ifdef TOS_RADIO
		return call RadioPacket.maxPayloadLength();
#else
		return 0;
#endif		
	}
	
	int nesc_getNodeId() @C() @spontaneous()
	{
		return 0;
	}
	
}
