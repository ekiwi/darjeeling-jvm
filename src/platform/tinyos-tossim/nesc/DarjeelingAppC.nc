#include "tosconfig.h"

configuration DarjeelingAppC
{
}

implementation
{
	components MainC, DarjeelingC;
#ifdef TOS_LEDS
	components LedsC;
#endif

#ifdef WITH_RADIO
	components new AMSenderC(AM_RADIO_COUNT_MSG);
	components new AMReceiverC(AM_RADIO_COUNT_MSG);
	components ActiveMessageC;
#endif
	components new TimerMilliC();

	DarjeelingC -> MainC.Boot;
#ifdef TOS_LEDS
	DarjeelingC.Leds -> LedsC;
#endif
	DarjeelingC.Timer -> TimerMilliC;

#ifdef WITH_RADIO
	DarjeelingC.Receive -> AMReceiverC;
	DarjeelingC.AMSend -> AMSenderC;
	DarjeelingC.AMControl -> ActiveMessageC;
	DarjeelingC.Packet -> AMSenderC;
#endif

}

