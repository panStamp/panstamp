/*
 * pulseCounter
 *
 * Copyright (c) 2014 panStamp <contact@panstamp.com>
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
 * Creation date: 07/24/2012
 *
 * Device:
 * Counter module
 *
 * Description:
 * PANSTAMP_NRG
 * Pulse counter on Digital pin 1 (pin 22)
 * PANSTAMP_AVR
 * Pulse counter on INT1 (pin 18)
 *
 * This sketch can be used to count pulses and send this count at a
 * maximum transmission rate
 *
 * This device is low-power enabled so it will enter low-power mode just
 * after reading a count or transmitting to the SWAP network.
 *
 * Associated Device Definition File, defining registers, endpoints and
 * configuration parameters:
 * pulsecounter.xml (Pulse counter)
 */

#include "regtable.h"
#include "swap.h"

/**
 * Macros
 */
#define ENABLE_PIN_IRQ()          attachInterrupt(1, isrPinEvent, RISING)
#define DISABLE_PIN_IRQ()         detachInterrupt(1)

/**
 * Counter values
 */
unsigned long currentCounter = 0;
unsigned long oldCounter = 0;

/**
 * isrPinEvent
 *
 * Event on pin
 */
void isrPinEvent(void)
{
  currentCounter++;
}

/**
 * setup
 *
 * Arduino setup function
 */
void setup()
{
  int i;

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  // Init SWAP stack
  swap.init();
  
  // Transmit product code
  swap.getRegister(REGI_PRODUCTCODE)->getData();

  // Enter SYNC state
  swap.enterSystemState(SYSTATE_SYNC);

  // During 3 seconds, listen the network for possible commands whilst the LED blinks
  for(i=0 ; i<6 ; i++)
  {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(400);
  }
  // Transmit periodic Tx interval
  swap.getRegister(REGI_TXINTERVAL)->getData();
  // Transmit power voltage
  swap.getRegister(REGI_VOLTSUPPLY)->getData(); 
  // Transmit initial counter values
  swap.getRegister(REGI_COUNTER)->getData();
  
  // Switch to Rx OFF state
  swap.enterSystemState(SYSTATE_RXOFF);

  pinMode(1, INPUT);
  
  // Enable INT1 Interrupt
  ENABLE_PIN_IRQ();
}

/**
 * loop
 *
 * Arduino main loop
 */
void loop()
{
  // Sleep for panstamp.txInterval seconds (register 10)
  swap.goToSleep();

  DISABLE_PIN_IRQ();

  // Transmit counter value only if changed
  if (currentCounter != oldCounter)
  {
    swap.getRegister(REGI_COUNTER)->getData();
    oldCounter = currentCounter;
  }

  ENABLE_PIN_IRQ();
}

