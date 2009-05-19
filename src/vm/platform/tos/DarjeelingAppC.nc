/*
 *	DarjeelingAppC.nc
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
#include "tosconfig.h"

// #define TOS_LEDS
// #define TOS_SERIAL

configuration DarjeelingAppC
{
}

implementation
{
	components MainC, DarjeelingC;

#ifdef TOS_LEDS
	components LedsC;
#endif

	components ActiveMessageC as Radio;
	components CC1000ControlP as CC1000;
	components CC1000CsmaRadioC;
#ifdef TOS_SERIAL
	components Atm128Uart0C as Uart;
#endif
	components new TimerMilliC();

	DarjeelingC -> MainC.Boot;
#ifdef TOS_LEDS
	DarjeelingC.Leds -> LedsC;
#endif
	DarjeelingC.Timer -> TimerMilliC;

	DarjeelingC.RadioReceive -> Radio.Receive[DJ_TOS_MESSAGE];
	DarjeelingC.RadioSend -> Radio.AMSend[DJ_TOS_MESSAGE];
	DarjeelingC.RadioPacket -> Radio;
	DarjeelingC.RadioControl -> Radio;
	DarjeelingC.PacketAcknowledgements -> Radio;

	DarjeelingC.CC1000 -> CC1000;
	DarjeelingC.LowPowerListening -> CC1000CsmaRadioC;

#ifdef TOS_SERIAL
	DarjeelingC.UartControl -> Uart.StdControl;
	DarjeelingC.UartByte -> Uart.UartByte;
	DarjeelingC.UartStream -> Uart.UartStream;
#endif

}

