#include "Timer.h"

#include <stdlib.h>
#include <stdio.h>

#include "darjeeling.h"

#include "message.h"
#include "platform_message.h"

#include "dj_tos_message.h"

#include "tosconfig.h"
#include "AM.h"
module DarjeelingC
{
	uses
	{
		interface Boot;
		interface Leds;
		interface Timer<TMilli> as Timer;
#ifdef WITH_RADIO
	    interface Receive;
	    interface AMSend;
	    interface SplitControl as AMControl;
	    interface Packet;
		interface LowPowerListening;
#endif
	}
}

implementation
{

	#define bufferSize 10
	char bufferIsLocked = 0;
	message_t radioPacket;
	radio_count_msg_t messageBuffer[bufferSize];
	int bufferPos = 0;

	bool radioLocked, ackPending;
	bool wasAcked;

	int tossim_printf(char * string ) @C() @spontaneous()
	{
		dbg("DEBUG", string);
		return 1;
	}

	uint32_t nesc_getTime() @C() @spontaneous()
	{
		return call Timer.getNow();
	}

	uint16_t nesc_getMaxPayloadLength() @C() @spontaneous()
	{
		/*
		return call RadioPacket.maxPayloadLength();
		*/
		return 0;
	}

	int nesc_send(const char * message, int16_t receiverId, uint16_t length) @C() @spontaneous()
	{
#ifdef WITH_RADIO
		radio_count_msg_t * tosmessage;

		// check for lock
		if (radioLocked) return -1;

		// get a pointer to the payload (data part of radioPacket)
		tosmessage = (radio_count_msg_t*)call Packet.getPayload(&radioPacket, NULL);

		// data is NULL if length>max payload
		if (tosmessage == NULL) return -2;

		// copy data into the radioPacket
		memcpy(tosmessage->payload, message, length);
		tosmessage->length = length;


		// if not broadcast, request ack
/*	
TODO : put back ack if needed
		if (receiverId==0xffff)
			ackPending = FALSE;
		else
			ackPending = (call PacketAcknowledgements.requestAck(&radioPacket) == SUCCESS);
*/

		// send radioPacket
		if (call AMSend.send(AM_BROADCAST_ADDR, &radioPacket, sizeof(radio_count_msg_t)) == SUCCESS)
		{
			radioLocked = TRUE;
			return 0;
		}
#endif
		return -3;

	}

#ifdef WITH_RADIO
	uint16_t nesc_peekMessageLength() @C() @spontaneous()
	{
		if (bufferPos>0)
		{
			return (messageBuffer[bufferPos -1].length);
		}
		else
			return 0;
	}
	void nesc_setBufferIsLocked(char lock){
		bufferIsLocked = lock;
	}
	void * nesc_popMessageBuffer() @C() @spontaneous()
	{
		int len;
		
		int i;
		nx_uint8_t payload;
		if (bufferPos>0)
		{
			len = nesc_peekMessageLength();
			bufferPos--;
			payload = (messageBuffer[bufferPos].payload);
			bufferIsLocked = 0;

			return (void *) (messageBuffer[bufferPos].payload);
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
#endif
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
#ifdef WITH_RADIO
	    call AMControl.start();
#else
//if radio is not included nothing will invoke run(). this means that darjeeling will do nothing
		post run();
#endif

	}

#ifdef WITH_RADIO


	event void AMControl.startDone(error_t err) {
		post run();
	}

	event void AMControl.stopDone(error_t err) {
    	// do nothing
	}

	event message_t* Receive.receive(message_t * message, void * payload, uint8_t length)
	{
		// push message into the buffer
		if (bufferPos<bufferSize)
		{
//			while (bufferIsLocked);
			memcpy(&messageBuffer[bufferPos],  payload,  sizeof(radio_count_msg_t));

/*			messageBuffer[bufferPos] = *((radio_count_msg_t*)payload);*/
//			dbg("DEBUG", "Received packet of length %hhu.\n", messageBuffer[bufferPos].length);
			bufferPos++;
//			bufferIsLocked = 1;
		}
		// notify the JVM
		dj_notifyRadioReceive();
//		post run();

		return message;
	}

	event void AMSend.sendDone(message_t* bufPtr, error_t error)
	{
	    if (&radioPacket == bufPtr)
	    {
		radioLocked = FALSE;

		// record whether the last message was acknowledged
/*		if (ackPending)
			wasAcked = call PacketAcknowledgements.wasAcked(&radioPacket);
*/

		dj_notifyRadioSendDone();
//		post run();
	    }

	}
#endif
}

