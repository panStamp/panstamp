/*
  ************************************************************************
  *	wiring.c
  *
  *	Arduino core files for MSP430
  *		Copyright (c) 2012 Robert Wessels. All right reserved.
  *
  *
  ***********************************************************************
  Derived from:
  wiring.c - Partial implementation of the Wiring API for the ATmega8.
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
   
  Adapted by Daniel Berenguer for panStamp: 06/28/2014
*/

#include "wiring.h"
#include "panstamp.h"

// the clock source is set so that watch dog timer (WDT) ticks every clock
// cycle (F_CPU), and the watch dog timer ISR is called every 512 ticks
// for F_CPU < 8MHz and every 8192 ticks for F_CPU > 8MHz.
#if F_CPU < 8000000L
#define TICKS_PER_WDT_OVERFLOW 512
#else
#define TICKS_PER_WDT_OVERFLOW 8192
#endif

// the whole number of microseconds per WDT overflow
#define MICROSECONDS_PER_WDT_OVERFLOW (clockCyclesToMicroseconds(TICKS_PER_WDT_OVERFLOW))

// the whole number of milliseconds per WDT overflow
#define MILLIS_INC (MICROSECONDS_PER_WDT_OVERFLOW / 1000)

// the fractional number of milliseconds per WDT overflow. 
#define FRACT_INC (MICROSECONDS_PER_WDT_OVERFLOW % 1000)
#define FRACT_MAX 1000

// Increments when sleeping. Depends on ACLK source.
uint16_t SMILLIS_INC;
uint16_t SFRACT_INC;

volatile unsigned long wdt_overflow_count = 0;
volatile unsigned long wdt_millis = 0;
volatile unsigned int wdt_fract = 0;
volatile uint8_t sleeping = false;
volatile uint16_t vlo_freq = 0;

void enableWatchDogIntervalMode(void);

/**
 * First routime being called!
 */
void init(void)
{
  disableWatchDog();

  // Init panStamp object
  panstamp.init();

  enableWatchDogIntervalMode();
  __eint();
}

void disableWatchDog(void)
{
  /* Diable watchdog timer */
  WDTCTL = WDTPW | WDTHOLD;
}

void enableWatchDog(void)
{
  enableWatchDogIntervalMode();
}

void enableWatchDogIntervalMode(void)
{
	/* WDT Password + WDT interval mode + Watchdog clock source /512 for F_CPU < 8MHz
	 * and /8192 for F_CPU > 8MHz + source from SMCLK.
	 * Note that we WDT is running in interval mode. WDT will not trigger a reset on expire in this mode. */
#if F_CPU < 8000000L
	WDTCTL = WDTPW | WDTTMSEL | WDTCNTCL | WDT_MDLY_0_5;
#else
	WDTCTL = WDTPW | WDTTMSEL | WDTCNTCL | WDT_MDLY_8;
#endif

	/* WDT interrupt enable */
#ifdef __MSP430_HAS_SFR__
	SFRIE1 |= WDTIE;
#else
	IE1 |= WDTIE;
#endif	
}

unsigned long micros()
{
	unsigned long m;

	// disable interrupts to ensure consistent readings
	// safe SREG to avoid issues if interrupts were already disabled
	uint16_t oldSREG = READ_SR;
	__dint();

	m = wdt_overflow_count;

	WRITE_SR(oldSREG);	// safe to enable interrupts again

	// MSP430 does not give read access to current WDT, so we
	// have to approximate microseconds from overflows and
	// fractional milliseconds.
	// With an WDT interval of SMCLK/512, precision is +/- 256/SMCLK,
	// for example +/-256us @1MHz and +/-16us @16MHz

	return (m * MICROSECONDS_PER_WDT_OVERFLOW);
}

unsigned long millis()
{
	unsigned long m;

	// disable interrupts to ensure consistent readings
	// safe SREG to avoid issues if interrupts were already disabled
	uint16_t oldSREG = READ_SR;
	__dint();

	m = wdt_millis;

	WRITE_SR(oldSREG);	// safe to enable interrupts again

 	return m;
}

/* Delay for the given number of microseconds.  Assumes a 1, 8 or 16 MHz clock. */
void delayMicroseconds(unsigned int us)
{
#if F_CPU >= 20000000L
	/* For a one-microsecond delay, simply wait 2 cycle and return. The overhead
	 * of the function call yields a delay of exactly one microsecond. */
	__asm__ __volatile__ (
		"nop" "\n\t"
		"nop");
	if (--us == 0)
		return;

	/* The following loop takes a 1/5 of a microsecond (4 cycles)
	 * per iteration, so execute it five times for each microsecond of
	 * delay requested. */
	us = (us<<2) + us; // x5 us

	/* Account for the time taken in the preceeding commands. */
	us -= 2;

#elif F_CPU >= 16000000L
	/* For the 16 MHz clock on most boards */

	/* For a one-microsecond delay, simply return.  the overhead
	 * of the function call yields a delay of approximately 1 1/8 us. */
	if (--us == 0)
		return;

	/* The following loop takes a quarter of a microsecond (4 cycles)
	 * per iteration, so execute it four times for each microsecond of
	 * delay requested. */
	us <<= 2;

	/* Account for the time taken in the preceeding commands. */
	us -= 2;
#elif F_CPU >= 12000000L
	/* For the 12 MHz clock on most boards */

	/* For a one-microsecond delay, simply return.  the overhead
	 * of the function call yields a delay of approximately 1 1/8 us. */
	if (--us == 0)
		return;

	/* The following loop takes a 1/3 of a microsecond (4 cycles)
	 * per iteration, so execute it four times for each microsecond of
	 * delay requested. */
	us <<= 1 + us; // x3 us;

	/* Account for the time taken in the preceeding commands. */
	us -= 2;
#else
	/* For the 1 MHz */

	/* For a one- or two-microsecond delay, simply return.  the overhead of
	 * the function calls takes more than two microseconds.  can't just
	 * subtract two, since us is unsigned; we'd overflow. */
	if (--us == 0)
		return;
	if (--us == 0)
		return;

	/* The following loop takes 4 microsecond (4 cycles)
	 * per iteration, so execute it ones for each 4 microsecond of
	 * delay requested. */
	us >>= 2;

	/* Partially compensate for the time taken by the preceeding commands.
	 * we can't subtract any more than this or we'd overflow w/ small delays. */
	us--;
#endif

	/* Busy wait */
  __asm__ __volatile__ (
          /* even steven */
          "L1: nop \n\t"   
          /* 1 instruction */
          "dec.w %[us] \n\t"
          /* 2 instructions */
          "jnz L1 \n\t"
          : [us] "=r" (us) : "[us]" (us)
  );
}

void sleepSeconds(uint32_t seconds)
{
	uint32_t start = millis();
	sleeping = true;

	if(vlo_freq) {
		SMILLIS_INC = 8192 / (vlo_freq/1000);
		SFRACT_INC = 300;
	} else {
		SMILLIS_INC = 250;
		SFRACT_INC = 0;
	}

	WDTCTL = WDT_ADLY_250;

	while(millis() - start <= seconds * 1000)
		/* Wait for WDT interrupt in LMP0 */
		__bis_status_register(LPM3_bits+GIE);

	sleeping = false;

	enableWatchDogIntervalMode();
}

void sleep(uint32_t milliseconds)
{
	if(milliseconds >= 1000) {
		uint16_t seconds = milliseconds/1000;
		sleepSeconds(seconds);
		milliseconds = milliseconds - (seconds * 1000);
	}

	if(milliseconds == 0)
		return;

	if(vlo_freq) {
		SMILLIS_INC = 64 / (vlo_freq/1000);
		SFRACT_INC = 500;
	} else {
		SMILLIS_INC = 1;
		SFRACT_INC = 950;
	}

	WDTCTL = WDT_ADLY_1_9;

	sleeping = true;
	uint32_t start = millis();

	while(millis() - start < milliseconds)
		/* Wait for WDT interrupt in LMP0 */
		__bis_status_register(LPM3_bits+GIE);

	sleeping = false;

	enableWatchDogIntervalMode();
}

/* (ab)use the WDT */
void delay(uint32_t milliseconds)
{
	uint32_t start = micros();
	while(milliseconds > 0) {
		if ((micros() - start) >= 1000) {
			milliseconds--;
			start += 1000;
		}
		__bis_status_register(LPM0_bits+GIE);
	}
}

__attribute__((interrupt(WDT_VECTOR)))
void watchdog_isr (void)
{
  // copy these to local variables so they can be stored in registers
  // (volatile variables must be read from memory on every access)
  unsigned long m = wdt_millis;
  unsigned int f = wdt_fract;

  m += sleeping ? SMILLIS_INC:MILLIS_INC;
  f += sleeping ? SFRACT_INC:FRACT_INC;
  if (f >= FRACT_MAX) {
    f -= FRACT_MAX;
    m += 1;
  }

  wdt_fract = f;
  wdt_millis = m;
  wdt_overflow_count++;

  /* Exit from LMP3 on reti (this includes LMP0) */
  __bic_status_register_on_exit(LPM3_bits);
}
