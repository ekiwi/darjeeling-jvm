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
	components ActiveMessageC as Radio;
	components CC1000ControlP as CC1000;
	components CC1000CsmaRadioC;
#endif
#ifdef TOS_SERIAL
	components Atm128Uart0C as Uart;
#endif
	components new TimerMilliC();

	DarjeelingC -> MainC.Boot;
#ifdef TOS_LEDS
	DarjeelingC.Leds -> LedsC;
#endif
	DarjeelingC.Timer -> TimerMilliC;

#ifdef WITH_RADIO
	DarjeelingC.RadioReceive -> Radio.Receive[DJ_TOS_MESSAGE];
	DarjeelingC.RadioSend -> Radio.AMSend[DJ_TOS_MESSAGE];
	DarjeelingC.RadioPacket -> Radio;
	DarjeelingC.RadioControl -> Radio;
	DarjeelingC.PacketAcknowledgements -> Radio;

	DarjeelingC.CC1000 -> CC1000;
	DarjeelingC.LowPowerListening -> CC1000CsmaRadioC;
#endif

#ifdef TOS_SERIAL
	DarjeelingC.UartControl -> Uart.StdControl;
	DarjeelingC.UartByte -> Uart.UartByte;
	DarjeelingC.UartStream -> Uart.UartStream;
#endif

}

