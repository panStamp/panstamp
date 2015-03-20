/**
 * Copyright (c) 2014 panStamp <contact@panstamp.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * any later version.
 * 
 * panStamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with panStamp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 
 * USA
 * 
 * Author: Daniel Berenguer
 * Creation date: 06/28/2014
 */

#ifndef _WIRING_H
#define _WIRING_H

#include <msp430.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include "pins.h"

/**
 * System clock
 */
#define SYSTEM_CLK_FREQ 12000000L
#ifndef F_CPU
#define F_CPU  SYSTEM_CLK_FREQ
#endif

#define PANSTAMP_NRG

#define HIGH 0x1
#define LOW  0x0

#define LSBFIRST 0
#define MSBFIRST 1

#define RISING 0
#define FALLING 1
#define CHANGE 2

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2
#define INPUT_PULLDOWN 0x4
#define PORT_SELECTION0 0x10
#define PORT_SELECTION1 0x20


#define true 0x1
#define false 0x0

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105

#ifndef BV
#define BV(x) (1 << (x))
#endif

/**
 * ADC references
 */
#define ADCREF_VCC  0
#define ADCREF_1V5  REFVSEL_0
#define ADCREF_2V0  REFVSEL_1
#define ADCREF_2V5  REFVSEL_2

#define DEFAULT       ADCREF_VCC
#define INTERNAL1V5   ADCREF_1V5
#define INTERNAL2V0   ADCREF_2V0
#define INTERNAL2V5   ADCREF_2V5

/**
 * Generic data types
 */
typedef uint8_t byte;
typedef uint8_t boolean;
typedef uint16_t word;

#define NAN 0

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define interrupts() __bis_SR_register(GIE)
#define noInterrupts() __bic_SR_register(GIE)

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define bit(b) (1UL << (b))

void init(void);
void setup(void);
void loop(void);

void attachInterrupt(uint8_t, void (*)(void), int mode);
void detachInterrupt(uint8_t);

extern const uint8_t digital_pin_to_timer[];
extern const uint8_t digital_pin_to_port[];
extern const uint8_t digital_pin_to_bit_mask[];
extern const uint16_t digital_pin_to_pmap[];
extern const uint16_t port_to_sel0[];
extern const uint16_t port_to_sel1[];
extern const uint16_t port_to_sel2[];
extern const uint16_t port_to_input[];
extern const uint16_t port_to_output[];
extern const uint16_t port_to_ren[];
extern const uint16_t port_to_pmap[];
extern const uint16_t digital_pin_to_analog_in[];

#define digitalPinToPort(P)    ( digital_pin_to_port[P] )
#define digitalPinToBitMask(P) ( digital_pin_to_bit_mask[P] )
#define digitalPinToTimer(P)   ( digital_pin_to_timer[P] )
#define digitalPinToADCIn(P)   ( digital_pin_to_analog_in[P] )
#define digitalPinToTimer(P)   ( digital_pin_to_timer[P] )
#define digitalPinToPortMap(P) ( (volatile uint8_t *)(digital_pin_to_pmap[P]) )
#define portDirRegister(P)     ( (volatile uint8_t *)( port_to_dir[P]) )
/*
 * We either of the compination   PxSEL and PxSEL2   or   PxSEL0 and PxSEL1
 * So we can remap  PxSEL and PxSEL2   to   PxSEL0 and PxSEL1
*/
#define portSelRegister(P)     ( (volatile uint8_t *)( port_to_sel[P]) )
#define portRenRegister(P)     ( (volatile uint8_t *)( port_to_ren[P]) )
#define portOutputRegister(P)  ( (volatile uint8_t *)( port_to_output[P]) )
#define portInputRegister(P)   ( (volatile uint8_t *)( port_to_input[P]) )
#define portPMReg(P)           ( (volatile uint8_t *)( port_to_pmap[P]) )

inline void delayMicroseconds(const uint16_t us)
{
  const uint16_t cyclesPerMicro = SYSTEM_CLK_FREQ/1000000L;
  __delay_cycles((us * cyclesPerMicro));
}

// wiring.cpp
void delay(uint32_t milliseconds);
void sleep(uint32_t milliseconds);
void sleepSeconds(uint32_t seconds);
uint32_t micros(void);
uint32_t millis(void);
void disableWatchDog();
void enableWatchDog();

// wiring_digital.cpp
void pinMode(uint8_t pin, uint8_t mode);
uint8_t digitalRead(uint8_t pin);
void digitalWrite(uint8_t pin, uint8_t state);

// wiring_analog.cpp
uint16_t analogRead(uint8_t pin);
void analogReference(uint16_t ref);
void analogWrite(uint8_t pin, uint16_t val);
void analogFrequency(uint16_t freq);

// wirong_pulse.cpp
unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout = 1000000L);

// wiring_shift.cpp
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);

// Logarithm
#define log  logf

/**
 * Macros
 */
#define map(x, in_min, in_max, out_min, out_max) ((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)

#endif
