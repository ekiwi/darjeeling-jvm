#ifndef __spi_h__
#define __spi_h__

#define avr_sspiSlaveSelect(x)		PORTB = (x) ? (PORTB|(1<<PB4)) : (PORTB&~(1<<PB4))
#define avr_sspiMoSi(x)				PORTB = (x) ? (PORTB|(1<<PB5)) : (PORTB&~(1<<PB5))
#define avr_sspiMiSo(x)				PORTB = (x) ? (PORTB|(1<<PB6)) : (PORTB&~(1<<PB6))
#define avr_sspiClock(x)			PORTB = (x) ? (PORTB|(1<<PB7)) : (PORTB&~(1<<PB7))


#define avr_sspiSlaveSelectEnable()		PORTB &= ~(1<<PB4)
#define avr_sspiSlaveSelectDisable()	PORTB |= (1<<PB4)

#define avr_sspiClockPulse()		PORTB &= ~(1<<PB7); \
									PORTB |= 1<<PB7;

#define avr_sspiSendBit(x)			avr_sspiMoSi(x); \
									avr_sspiClockPulse(); \

#define avr_sspiSendByte(x)			avr_sspiSendBit((x & 0x80) >> 7); \
									avr_sspiSendBit((x & 0x40) >> 6); \
									avr_sspiSendBit((x & 0x20) >> 5); \
									avr_sspiSendBit((x & 0x10) >> 4); \
									avr_sspiSendBit((x & 0x8) >> 3); \
									avr_sspiSendBit((x & 0x4) >> 2); \
									avr_sspiSendBit((x & 0x2) >> 1); \
									avr_sspiSendBit(x & 0x1);

#define avr_sspiInit()				DDRB |= 0xf0;

#endif
