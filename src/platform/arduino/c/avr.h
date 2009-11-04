#ifndef __avr_h__
#define __avr_h__

#include <avr/io.h>
#include <avr/interrupt.h>
// #include <avr/delay.h>
#include <stdio.h>
#include <stdarg.h>

// 16 MHz clock speed
#define F_CPU 16000000

// clear bit, set bit macros
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

void avr_timerInit();
unsigned long avr_millis();
void avr_delay(unsigned long ms);

void avr_serialInit(uint32_t baud);
void avr_serialPrint(char * str);
void avr_serialVPrint(char * format, va_list arg);
void avr_serialPrintf(char * format, ...);
void avr_serialWrite(unsigned char value);


#endif
