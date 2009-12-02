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
	message_t radioPacket;
	radio_count_msg_t messageBuffer[bufferSize];
	int bufferPos = 0;

	bool radioLocked, ackPending, wasAcked;
	void * darjeeling_global_variables;
	void * darjeeling_UGLY_global_variables;


	void* tossim_getDarjeelingUglyGlobals() @C() @spontaneous(){
		return darjeeling_UGLY_global_variables;
	}

	void tossim_setDarjeelingUglyGlobals(void *UGLY_global_variables) @C() @spontaneous(){
		darjeeling_UGLY_global_variables = UGLY_global_variables;
	}


	void* tossim_getDarjeelingGlobals() @C() @spontaneous(){
		return darjeeling_global_variables;
	}

	void tossim_setDarjeelingGlobals(void *_darjeeling_global_variables) @C() @spontaneous(){
		darjeeling_global_variables = _darjeeling_global_variables;
	}

	
	int tossim_printf(char * string ) @C() @spontaneous()
	{
		dbg_clear("OUTPUT", "%c[32mnode %d:%c[0m  %s", 0x1b, sim_node(), 0x1b, string);
		return 1;
	}

	int tossim_debug(char * string ) @C() @spontaneous()
	{
		dbg("DEBUG", string);
		return 1;
	}


	uint32_t tossim_getTime() @C() @spontaneous()
	{
		return call Timer.getNow();
	}

	uint16_t tossim_getMaxPayloadLength() @C() @spontaneous()
	{
		/*
		return call RadioPacket.maxPayloadLength();
		*/
		return 0;
	}

	int tossim_send(const char * message, int16_t receiverId, uint16_t length) @C() @spontaneous()
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
		if (call AMSend.send(receiverId, &radioPacket, sizeof(radio_count_msg_t)) == SUCCESS)
		{
			dbg("DEBUG", "tossim_send: packet sent.\n");
			radioLocked = TRUE;
			return 0;
		}
#endif
		return -3;

	}

#ifdef WITH_RADIO
	uint16_t tossim_peekMessageLength() @C() @spontaneous()
	{
		if (bufferPos>0)
		{
			return (messageBuffer[bufferPos -1].length);
		}
		else
			return 0;
	}

	void * tossim_popMessageBuffer() @C() @spontaneous()
	{
		int len;
		if (bufferPos>0)
		{
			len = tossim_peekMessageLength();
			bufferPos--;
			return (void *) (messageBuffer[bufferPos].payload);
		}
		else
			return NULL;
	}

	int tossim_getNrMessages() @C() @spontaneous()
	{
		return bufferPos;
	}

	int tossim_wasAcked() @C() @spontaneous()
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
			memcpy(&messageBuffer[bufferPos],  payload,  sizeof(radio_count_msg_t));

			dbg("DEBUG", "Received packet of length %hhu.\n", messageBuffer[bufferPos].length);
			bufferPos++;
		}
		// notify the JVM
		dj_notifyRadioReceive();
		post run();

		return message;
	}

	event void AMSend.sendDone(message_t* bufPtr, error_t error)
	{
		dbg("DEBUG", "AMSend.sendDone started\n");
	    if (&radioPacket == bufPtr)
	    {

		radioLocked = FALSE;

		// record whether the last message was acknowledged
/*
		if (ackPending)
			wasAcked = call PacketAcknowledgements.wasAcked(&radioPacket);
*/

		dbg("DEBUG", "notifyRadioSendDone is called\n");
		dj_notifyRadioSendDone();
		post run();
	    }
	}
#endif
}

