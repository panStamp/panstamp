/*
 * pulsegen
 *
 * Copyright (c) 2012 Daniel Berenguer <dberenguer@usapiens.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 * 
 * panStamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with panStamp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
 * USA
 * 
 * Author: Daniel Berenguer
 * Creation date: 07/23/2012
 *
 * Device:
 * Pulse generator
 *
 * Description:
 * Device providing 4 programmable pulse outputs.
 * Frequency range: 0-200 Hz
 *
 * Pulse outputs : pins 4, 5, 6 and 8
 *
 * This sketch can be used to create intermitent pulses.
 *
 * Associated Device Definition File, defining registers, endpoints and
 * configuration parameters:
 * pulsegen.xml (Binary/PWM output module)
 */

#include "TimerOne.h"
#include "regtable.h"
#include "panstamp.h"

/**
 * Timer 1
 */
#define TIMER1_TICK_PERIOD_US    2500  // Timer1 tick = 2.5 ms
// Tick countes
int tickCount[] = {0, 0, 0, 0};
// Output frequencies
byte outFrequency[] = {0, 0, 0, 0};
// Total ticks per period
long maxTicks[] = {0, 0, 0, 0};
// Total ticks per high level
long highTicks[] = {0, 0, 0, 0};

/**
 * LED pin
 */
#define LEDPIN  4

/**
 * Pulse output pins (Atmega pins)
 */
#define PULSEPORT PORTC
#define PULSEDIR  DDRC
uint8_t pulseBit[] = {0, 1, 2, 3};

/**
 * isrT1event
 *
 * Timer1 interrupt routine
 */
void isrT1event(void)
{
  int i;
   
  for(i=0 ; i<sizeof(pulseBit) ; i++)
  {
    tickCount[i]++;
  
    if (tickCount[i] == highTicks[i])
      bitSet(PULSEPORT, pulseBit[i]);
    else if (tickCount[i] >= maxTicks[i])
    {
      bitClear(PULSEPORT, pulseBit[i]);
      tickCount[i] = 0;
    }
  }
}

/**
 * setup
 *
 * Arduino setup function
 */
void setup()
{
  int i;
  
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);

  // Configure and initialize output pins
  for(i=0 ; i<sizeof(pulseBit) ; i++)
  {
    bitSet(PULSEDIR, pulseBit[i]);
    bitClear(PULSEPORT, pulseBit[i]);
  }

  // Init panStamp
  panstamp.init();

  // Transmit product code
  getRegister(REGI_PRODUCTCODE)->getData();

  // Enter SYNC state
  panstamp.enterSystemState(SYSTATE_RXON);
  // Transmit initial frequency values
  getRegister(REGI_FREQUENCY0)->getData();
  getRegister(REGI_FREQUENCY1)->getData();
  getRegister(REGI_FREQUENCY2)->getData();
  getRegister(REGI_FREQUENCY3)->getData();
  
  // Init Timer1
  Timer1.initialize(TIMER1_TICK_PERIOD_US);
  // Attach interrupt function to Timer1
  Timer1.attachInterrupt(isrT1event);
}

/**
 * loop
 *
 * Arduino main loop
 */
void loop()
{
  delay(100);
}

