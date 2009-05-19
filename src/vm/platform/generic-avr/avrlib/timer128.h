/*! \file timer128.h \brief System Timer function library for Mega128. */
//*****************************************************************************
//
// File Name	: 'timer128.h'
// Title		: System Timer function library for Mega128
// Author		: Pascal Stang - Copyright (C) 2000-2003
// Created		: 11/22/2000
// Revised		: 02/10/2003
// Version		: 1.1
// Target MCU	: Atmel AVR Series
// Editor Tabs	: 4
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
///	\ingroup driver_avr
/// \defgroup timer128 Timer Function Library for ATmega128 (timer128.c)
/// \code #include "timer128.h" \endcode
/// \par Overview
///		This library provides functions for use with the timers internal to the
///		AVR ATmega128.� Functions include initialization, set prescaler,
///		calibrated pause function (in milliseconds), attaching and detaching
///		of user functions to interrupts, overflow counters, and PWM.
///
/// \par About Timers
///		The Atmel AVR-series processors each contain at least one
///		hardware timer/counter.  Many of the processors contain 2 or 3
///		timers.  Generally speaking, a timer is a hardware counter inside
///		the processor which counts at a rate related to the main CPU clock
///		frequency.  Because the counter value increasing (counting up) at
///		a precise rate, we can use it as a timer to create or measure
///		precise delays, schedule events, or generate signals of a certain
///		frequency or pulse-width.
/// \par
///		As an example, the ATmega163 processor has 3 timer/counters.
///		Timer0, Timer1, and Timer2 are 8, 16, and 8 bits wide respectively.
///		This means that they overflow, or roll over back to zero, at a
///		count value of 256 for 8bits or 65536 for 16bits.  A prescaler is
///		avaiable for each timer, and the prescaler allows you to pre-divide
///		the main CPU clock rate down to a slower speed before feeding it to
///		the counting input of a timer.  For example, if the CPU clock
///		frequency is 3.69MHz, and Timer0's prescaler is set to divide-by-8,
///		then Timer0 will "tic" at 3690000/8 = 461250Hz.  Because Timer0 is
///		an 8bit timer, it will count to 256 in just 256/461250Hz = 0.555ms.
///		In fact, when it hits 255, it will overflow and start again at
///		zero.  In this case, Timer0 will overflow 461250/256 = 1801.76
///		times per second.
/// \par
///		Timer0 can be used a number of ways simultaneously.  First, the
///		value of the timer can be read by accessing the CPU register \c TCNT0.
///		We could, for example, figure out how long it takes to execute a
///		C command by recording the value of \c TCNT0 before and after
///		execution, then subtract (after-before) = time elapsed.  Or we can
///		enable the overflow interrupt which goes off every time T0
///		overflows and count out longer delays (multiple overflows), or
///		execute a special periodic function at every overflow.
/// \par
///		The other timers (Timer1 and Timer2) offer all the abilities of
///		Timer0 and many more features.  Both T1 and T2 can operate as
///		general-purpose timers, but T1 has special hardware allowing it to
///		generate PWM signals, while T2 is specially designed to help count
///		out real time (like hours, minutes, seconds).  See the
///		Timer/Counter section of the processor datasheet for more info.
///
//*****************************************************************************
//@{

#ifndef TIMER128_H
#define TIMER128_H

#include "common/types.h"
#include "avrlibdefs.h"

// constants/macros/typdefs

// Timer/clock prescaler values and timer overflow rates
// tics = rate at which the timer counts up
// 8bitoverflow = rate at which the timer overflows 8bits (or reaches 256)
// 16bit [overflow] = rate at which the timer overflows 16bits (65536)
//
// overflows can be used to generate periodic interrupts
//
// for 8MHz crystal
// 0 = STOP (Timer not counting)
// 1 = CLOCK		tics= 8MHz			8bitoverflow= 31250Hz		16bit= 122.070Hz
// 2 = CLOCK/8		tics= 1MHz			8bitoverflow= 3906.25Hz		16bit=  15.259Hz
// 3 = CLOCK/64		tics= 125kHz		8bitoverflow=  488.28Hz		16bit=   1.907Hz
// 4 = CLOCK/256	tics= 31250Hz		8bitoverflow=  122.07Hz		16bit=	0.477Hz
// 5 = CLOCK/1024	tics= 7812.5Hz		8bitoverflow=   30.52Hz		16bit=   0.119Hz
// 6 = External Clock on T(x) pin (falling edge)
// 7 = External Clock on T(x) pin (rising edge)

// for 4MHz crystal
// 0 = STOP (Timer not counting)
// 1 = CLOCK		tics= 4MHz			8bitoverflow= 15625Hz		16bit=  61.035Hz
// 2 = CLOCK/8		tics= 500kHz		8bitoverflow= 1953.125Hz	16bit=   7.629Hz
// 3 = CLOCK/64		tics= 62500Hz		8bitoverflow=  244.141Hz	16bit=   0.954Hz
// 4 = CLOCK/256	tics= 15625Hz		8bitoverflow=   61.035Hz	16bit=   0.238Hz
// 5 = CLOCK/1024	tics= 3906.25Hz		8bitoverflow=   15.259Hz	16bit=   0.060Hz
// 6 = External Clock on T(x) pin (falling edge)
// 7 = External Clock on T(x) pin (rising edge)

// for 3.69MHz crystal
// 0 = STOP (Timer not counting)
// 1 = CLOCK		tics= 3.69MHz		8bitoverflow= 14414Hz		16bit=  56.304Hz
// 2 = CLOCK/8		tics= 461250Hz		8bitoverflow= 1801.758Hz	16bit=   7.038Hz
// 3 = CLOCK/64		tics= 57625.25Hz	8bitoverflow=  225.220Hz	16bit=   0.880Hz
// 4 = CLOCK/256	tics= 14414.063Hz	8bitoverflow=   56.305Hz	16bit=   0.220Hz
// 5 = CLOCK/1024	tics=  3603.516Hz	8bitoverflow=   14.076Hz	16bit=   0.055Hz
// 6 = External Clock on T(x) pin (falling edge)
// 7 = External Clock on T(x) pin (rising edge)

// for 32.768KHz crystal on timer 2 (use for real-time clock)
// 0 = STOP
// 1 = CLOCK		tics= 32.768kHz	8bitoverflow= 128Hz
// 2 = CLOCK/8		tics= 4096kHz		8bitoverflow=  16Hz
// 3 = CLOCK/64		tics= 512Hz			8bitoverflow=   2Hz
// 4 = CLOCK/256	tics= 128Hz			8bitoverflow=   0.5Hz
// 5 = CLOCK/1024	tics= 32Hz			8bitoverflow=   0.125Hz

#define TIMER_CLK_STOP			0x00	///< Timer Stopped
#define TIMER_CLK_DIV1			0x01	///< Timer clocked at F_CPU
#define TIMER_CLK_DIV8			0x02	///< Timer clocked at F_CPU/8
#define TIMER_CLK_DIV64			0x03	///< Timer clocked at F_CPU/64
#define TIMER_CLK_DIV256		0x04	///< Timer clocked at F_CPU/256
#define TIMER_CLK_DIV1024		0x05	///< Timer clocked at F_CPU/1024
#define TIMER_CLK_T_FALL		0x06	///< Timer clocked at T falling edge
#define TIMER_CLK_T_RISE		0x07	///< Timer clocked at T rising edge
#define TIMER_PRESCALE_MASK		0x07	///< Timer Prescaler Bit-Mask

#define TIMERRTC_CLK_STOP		0x00	///< RTC Timer Stopped
#define TIMERRTC_CLK_DIV1		0x01	///< RTC Timer clocked at F_CPU
#define TIMERRTC_CLK_DIV8		0x02	///< RTC Timer clocked at F_CPU/8
#define TIMERRTC_CLK_DIV32		0x03	///< RTC Timer clocked at F_CPU/32
#define TIMERRTC_CLK_DIV64		0x04	///< RTC Timer clocked at F_CPU/64
#define TIMERRTC_CLK_DIV128		0x05	///< RTC Timer clocked at F_CPU/128
#define TIMERRTC_CLK_DIV256		0x06	///< RTC Timer clocked at F_CPU/256
#define TIMERRTC_CLK_DIV1024	0x07	///< RTC Timer clocked at F_CPU/1024
#define TIMERRTC_PRESCALE_MASK	0x07	///< RTC Timer Prescaler Bit-Mask

// default prescale settings for the timers
// these settings are applied when you call
// timerInit or any of the timer<x>Init
#define TIMER0PRESCALE		TIMERRTC_CLK_DIV64	///< timer 0 prescaler default
#define TIMER1PRESCALE		TIMER_CLK_DIV64		///< timer 1 prescaler default
#define TIMER2PRESCALE		TIMER_CLK_DIV8		///< timer 2 prescaler default
#define TIMER3PRESCALE		TIMER_CLK_DIV64		///< timer 3 prescaler default

// interrupt macros for attaching user functions to timer interrupts
// use these with timerAttach( intNum, function )
// timer 0
#define TIMER0OVERFLOW_INT			0
#define TIMER0OUTCOMPARE_INT		1
// timer 1
#define TIMER1OVERFLOW_INT			2
#define TIMER1OUTCOMPAREA_INT		3
#define TIMER1OUTCOMPAREB_INT		4
#define TIMER1OUTCOMPAREC_INT		5
#define TIMER1INPUTCAPTURE_INT		6
// timer 2
#define TIMER2OVERFLOW_INT			7
#define TIMER2OUTCOMPARE_INT		8
// timer 3
#define TIMER3OVERFLOW_INT			9
#define TIMER3OUTCOMPAREA_INT		10
#define TIMER3OUTCOMPAREB_INT		11
#define TIMER3OUTCOMPAREC_INT		12
#define TIMER3INPUTCAPTURE_INT		13

#define TIMER_NUM_INTERRUPTS		14

// type of interrupt handler to use for timers
// *do not change unless you know what you're doing
// Value may be SIGNAL or INTERRUPT
#ifndef TIMER_INTERRUPT_HANDLER
#define TIMER_INTERRUPT_HANDLER		SIGNAL
#endif

// functions
#define delay		delay_us
#define delay_ms	timerPause
void delay_us(unsigned short time_us);

// initializes timing system
// runs all timer init functions
// sets all timers to default prescale values #defined in systimer.c
void timerInit(void);

// default initialization routines for each timer
void timer0Init(void);
void timer1Init(void);
void timer2Init(void);
void timer3Init(void);

// Clock prescaler set/get commands for each timer/counter
// For setting the prescaler, you should use one of the #defines
// above like TIMER_CLK_DIVx, where [x] is the division rate
// you want.
// When getting the current prescaler setting, the return value
// will be the [x] division value currently set.
void timer0SetPrescaler(uint8_t prescale);		///< set timer0 prescaler division index
void timer1SetPrescaler(uint8_t prescale);		///< set timer1 prescaler division index
void timer2SetPrescaler(uint8_t prescale);		///< set timer2 prescaler division index
void timer3SetPrescaler(uint8_t prescale);		///< set timer3 prescaler division index
uint16_t  timer0GetPrescaler(void);				///< get timer0 prescaler division rate
uint16_t  timer1GetPrescaler(void);				///< get timer1 prescaler division rate
uint16_t  timer2GetPrescaler(void);				///< get timer2 prescaler division rate
uint16_t  timer3GetPrescaler(void);				///< get timer3 prescaler division rate


// TimerAttach and Detach commands
//		These functions allow the attachment (or detachment) of any user function
//		to a timer interrupt.  "Attaching" one of your own functions to a timer
//		interrupt means that it will be called whenever that interrupt happens.
//		Using attach is better than rewriting the actual INTERRUPT() function
//		because your code will still work and be compatible if the timer library
//		is updated.  Also, using Attach allows your code and any predefined timer
//		code to work together and at the same time.  (ie. "attaching" your own
//		function to the timer0 overflow doesn't prevent timerPause from working,
//		but rather allows you to share the interrupt.)
//
//		timerAttach(TIMER1OVERFLOW_INT, myOverflowFunction);
//		timerDetach(TIMER1OVERFLOW_INT)
//
//		timerAttach causes the myOverflowFunction() to be attached, and therefore
//		execute, whenever an overflow on timer1 occurs.  timerDetach removes the
//		association and executes no user function when the interrupt occurs.
//		myOverflowFunction must be defined with no return value and no arguments:
//
//		void myOverflowFunction(void) { ... }

void timerAttach(uint8_t interruptNum, void (*userFunc)(void) );
void timerDetach(uint8_t interruptNum);


// timing commands
// timerPause pauses for the number of milliseconds specified in <pause_ms>
void timerPause(unsigned short pause_ms);

// overflow counters
// to be documented
void timer0ClearOverflowCount(void);
long timer0GetOverflowCount(void);
void timer2ClearOverflowCount(void);
long timer2GetOverflowCount(void);

// PWM initialization and set commands for timerX (where X is either 1 or 3)
// timerXPWMInit()
//		configures the timerX hardware for PWM mode on pins OCXA, OCXB, and OCXC.
//		bitRes should be 8,9,or 10 for 8,9,or 10bit PWM resolution
//
// timerXPWMOff()
//		turns off all timerX PWM output and set timer mode to normal state
//
// timerXPWMAOn(), timerXPWMBOn(), timerXPWMCOn()
//		turn on output of PWM signals to OCXA,B,C pins
//		NOTE: Until you define the OCXA,B,C pins as outputs, and run
//		this "on" command, no PWM output will be output
//
// timerXPWMAOff(), timerXPWMBOff(), timerXPWMCOff()
//		turn off output of PWM signals to OCXA,B,C pins
//
// timerXPWMASet(), timer1PWMBSet(), timerXPWMCSet()
//		sets the PWM duty cycle for each channel
//	NOTE:	<pwmDuty> should be in the range 0-255 for 8bit PWM
//			<pwmDuty> should be in the range 0-511 for 9bit PWM
//			<pwmDuty> should be in the range 0-1023 for 10bit PWM
// NOTE: the PWM frequency can be controlled in increments by setting the
//			prescaler for timer1

void timer1PWMInit(uint8_t bitRes);		///< initialize and set timer1 mode to PWM
void timer1PWMInitICR(uint16_t topcount);///< initialize and set timer1 mode to PWM with specific top count
void timer1PWMOff(void);			///< turn off all timer1 PWM output and set timer mode to normal
void timer1PWMAOn(void);			///< turn on timer1 Channel A (OC1A) PWM output
void timer1PWMBOn(void);			///< turn on timer1 Channel B (OC1B) PWM output
void timer1PWMCOn(void);			///< turn on timer1 Channel C (OC1C) PWM output
void timer1PWMAOff(void);			///< turn off timer1 Channel A (OC1A) PWM output
void timer1PWMBOff(void);			///< turn off timer1 Channel B (OC1B) PWM output
void timer1PWMCOff(void);			///< turn off timer1 Channel C (OC1C) PWM output
void timer1PWMASet(uint16_t pwmDuty);	///< set duty of timer1 Channel A (OC1A) PWM output
void timer1PWMBSet(uint16_t pwmDuty);	///< set duty of timer1 Channel B (OC1B) PWM output
void timer1PWMCSet(uint16_t pwmDuty);	///< set duty of timer1 Channel C (OC1C) PWM output

void timer3PWMInit(uint8_t bitRes);		///< initialize and set timer3 mode to PWM
void timer3PWMInitICR(uint16_t topcount);///< initialize and set timer3 mode to PWM with specific top count
void timer3PWMOff(void);			///< turn off all timer3 PWM output and set timer mode to normal
void timer3PWMAOn(void);			///< turn on timer3 Channel A (OC3A) PWM output
void timer3PWMBOn(void);			///< turn on timer3 Channel B (OC3B) PWM output
void timer3PWMCOn(void);			///< turn on timer3 Channel C (OC3C) PWM output
void timer3PWMAOff(void);			///< turn off timer3 Channel A (OC3A) PWM output
void timer3PWMBOff(void);			///< turn off timer3 Channel B (OC3B) PWM output
void timer3PWMCOff(void);			///< turn off timer3 Channel C (OC3C) PWM output
void timer3PWMASet(uint16_t pwmDuty);	///< set duty of timer3 Channel A (OC3A) PWM output
void timer3PWMBSet(uint16_t pwmDuty);	///< set duty of timer3 Channel B (OC3B) PWM output
void timer3PWMCSet(uint16_t pwmDuty);	///< set duty of timer3 Channel C (OC3C) PWM output

//@}

// Pulse generation commands have been moved to the pulse.c library

#endif
