/*
 * bininps
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
 * Creation date: 01/18/2012
 *
 * Device:
 * Binary input + counter module
 *
 * Description:
 * Device that reports the binary state of 12 digital inputs, 4 of them being
 * used as pulse counters as well.
 *
 * Binary inputs only: pins 2, 3, 4, 5, 6, 8, 9 and 10
 * Binary/counter inputs: pins 18, 20, 21 and 22
 *
 * This sketch can be used to detect key/switch presses, binary alarms or
 * any other binary sensor. It can also be used to count pulses from a vaste
 * variety of devices such as energy meters, water meters, gas meters, etc.
 * You may want to add some delays in updateValues in order to better debounce
 * the binary states. We suggest to add external capacitors to the inputs as well.
 * Capacitor values and delays should depend on the nature and frequence of the
 * input signals.
 *
 * This device is low-power enabled so it will enter low-power mode just
 * after reading the binary states and transmitting them over the SWAP
 * network.
 *
 * Associated Device Definition File, defining registers, endpoints and
 * configuration parameters:
 * bininps.xml (Binary/Counter input module)
 */
 
#include "regtable.h"
#include "panstamp.h"

/**
 * Interrupt masks
 */
#define PCINTMASK0    0x03  // PB[0:1]
#define PCINTMASK1    0x3F  // PC[0:5]
#define PCINTMASK2    0xE8  // PD[3], PD[5:7]. These are counter inputs too

/**
 * Macros
 */
#define pcEnableInterrupt()     PCICR = 0x07    // Enable Pin Change interrupts on all ports
#define pcDisableInterrupt()    PCICR = 0x00    // Disable Pin Change interrupts

/**
 * LED pin
 */
#define LEDPIN               4

/**
 * Pin Change Interrupt flag
 */
volatile boolean pcIRQ = false;

/**
 * Binary states
 */
byte stateLowByte = 0, stateHighByte = 0;

/**
 * Pure Binary inputs
 */
uint8_t binaryPin[] = {0, 1, 0, 1, 2, 3, 4, 5};                                              // Binary pins (Atmega port bits)
volatile uint8_t *binaryPort[] = {&PINB, &PINB, &PINC, &PINC, &PINC, &PINC, &PINC, &PINC};   // Binary ports (Atmega port)
int lastStateBinary[] = {-1, -1, -1, -1, -1, -1, -1, -1};                                    // Initial pin states

/**
 * Counters
 */
uint8_t counterPin[] = {3, 5, 6, 7};                                // Counter pins (Atmega port bits)
volatile uint8_t *counterPort[] = {&PIND, &PIND, &PIND, &PIND};     // Counter ports (Atmega port)
unsigned long counter[] = {0, 0, 0, 0};                             // Initial counter values
int lastStateCount[] = {-1, -1, -1, -1};                            // Initial pin states

/**
 * Pin Change Interrupt vectors
 */
SIGNAL(PCINT0_vect)
{
  panstamp.wakeUp();
  pcIRQ = true;
}
SIGNAL(PCINT1_vect)
{
  panstamp.wakeUp();
  pcIRQ = true;
}
SIGNAL(PCINT2_vect)
{
  panstamp.wakeUp();
  pcIRQ = true;
}

/**
 * updateValues
 *
 * Update binary state registers and counters
 *
 * Return:
 * 0 -> No binary state change
 * 1 -> Only binary states changed
 * 2 -> Binary states and counters changed
 */
byte updateValues(void)
{
  byte i, res = 0;
  int state;

  stateLowByte = 0;
  for(i=0 ; i<sizeof(binaryPin) ; i++)
  {
    state = bitRead(*binaryPort[i], binaryPin[i]);
    stateLowByte |= state << i;
    if (lastStateBinary[i] != state)
    {
      lastStateBinary[i] = state;
      res = 1;
    }
  }

  stateHighByte = 0;
  for(i=0 ; i<sizeof(counterPin) ; i++)
  {
    state = bitRead(*counterPort[i], counterPin[i]);
    stateHighByte |= state << i;
    if (lastStateCount[i] != state)
    {
      if (res == 0)
        res = 1;
        
      lastStateCount[i] = state;
    
      if (state == HIGH)
      {
        counter[i]++;
        res = 2;
      }
    }
  }

  return res;
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

  // Set pins as inputs
  DDRB &= ~PCINTMASK0;
  DDRC &= ~PCINTMASK1;  
  DDRD &= ~PCINTMASK2;

  // Set PC interrupt masks
  PCMSK0 = PCINTMASK0;
  PCMSK1 = PCINTMASK1;
  PCMSK2 = PCINTMASK2;
  
  // Init panStamp
  panstamp.init();

  // Transmit product code
  getRegister(REGI_PRODUCTCODE)->getData();

  // Enter SYNC state
  panstamp.enterSystemState(SYSTATE_SYNC);

  // During 3 seconds, listen the network for possible commands whilst the LED blinks
  for(i=0 ; i<6 ; i++)
  {
    digitalWrite(LEDPIN, HIGH);
    delay(100);
    digitalWrite(LEDPIN, LOW);
    delay(400);
  }
  // Transmit periodic Tx interval
  getRegister(REGI_TXINTERVAL)->getData();
  // Transmit power voltage
  getRegister(REGI_VOLTSUPPLY)->getData();
  
  updateValues();
  // Transmit initial binary states
  getRegister(REGI_BININPUTS)->getData();
  // Transmit initial counter values
  getRegister(REGI_COUNTERS)->getData();
  
  // Switch to Rx OFF state
  panstamp.enterSystemState(SYSTATE_RXOFF);

  // Enable Pin Change Interrupts
  pcEnableInterrupt();
}

/**
 * loop
 *
 * Arduino main loop
 */
void loop()
{
  // Sleep for panstamp.txInterval seconds (register 10)
  panstamp.goToSleep();

  pcDisableInterrupt();

  if (pcIRQ)
  {
    switch(updateValues())
    {
      case 2:
        // Transmit counter values
        getRegister(REGI_COUNTERS)->getData();
      case 1:
        // Transmit binary states
        getRegister(REGI_BININPUTS)->getData();
        break;
      default:
        break;
    }
    //Ready to receive new PC interrupts
    pcIRQ = false;
  }
  else
  {    
    // Just send states and counter values periodically, according to the value
    // of panstamp.txInterval (register 10)
    getRegister(REGI_COUNTERS)->getData();
    getRegister(REGI_BININPUTS)->getData();
  }

  pcEnableInterrupt();
}

