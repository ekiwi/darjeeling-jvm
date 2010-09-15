/*
 * DarjeelingAppC.nc
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
 

#include "tosconfig.h"
#include "config.h"

configuration DarjeelingAppC
{
}

implementation
{
	// Main components.
	components MainC, DarjeelingC;

	DarjeelingC -> MainC.Boot;
	
	// Millisecond timer.
	components new TimerMilliC();
	DarjeelingC.Timer -> TimerMilliC;

	// Leds.
#ifdef TOS_LEDS
	components LedsC;
	
	DarjeelingC.Leds -> LedsC;
#endif

	// Common radio stuff.
#ifdef TOS_RADIO
	components ActiveMessageC as Radio;
        components ActiveMessageAddressC as AddressC;
	
	DarjeelingC.RadioReceive -> Radio.Receive[DJ_TOS_MESSAGE];
	DarjeelingC.RadioSend -> Radio.AMSend[DJ_TOS_MESSAGE];
	DarjeelingC.RadioPacket -> Radio;
	DarjeelingC.RadioControl -> Radio;
	DarjeelingC.PacketAcknowledgements -> Radio;
        DarjeelingC.Address -> AddressC;

#endif

#ifdef TOS_CC1101
	components HalChipconControlP as CC1101;
	DarjeelingC.CC1101 -> CC1101;
#endif
	// Specifics for the CC1000 transceiver chip.
#ifdef TOS_CC1000
	components CC1000ControlP as CC1000;
	components CC1000CsmaRadioC;
	
	DarjeelingC.CC1000 -> CC1000;
	DarjeelingC.LowPowerListening -> CC1000CsmaRadioC;
#endif

	// Serial port.
#ifdef TOS_SERIAL
	components PlatformSerialC as Uart;
	
	DarjeelingC.UartControl -> Uart;
	DarjeelingC.UartStream -> Uart;
	DarjeelingC.UartByte -> Uart;
#endif

}

