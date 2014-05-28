/*
 * pulseCounter
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
 * Creation date: 07/24/2012
 *
 * Device:
 * Counter module
 *
 * Description:
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
#include "panstamp.h"

/**
 * Macros
 */
#define enableINT1irq()          attachInterrupt(1, isrINT1event, RISING)
#define disableINT1irq()         detachInterrupt(1)

/**
 * LED pin
 */
#define LEDPIN               4

/**
 * Counter values
 */
unsigned long currentCounter = 0;
unsigned long oldCounter = 0;

/**
 * Interrupt flag
 */
boolean int1IRQ = false;

/**
 * isrINT1event
 *
 * Event on INT1
 */
void isrINT1event(void)
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

  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);

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
  // Transmit initial counter values
  getRegister(REGI_COUNTER)->getData();
  
  // Switch to Rx OFF state
  panstamp.enterSystemState(SYSTATE_RXOFF);

  // Enable INT1 Interrupt
  enableINT1irq();
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

  disableINT1irq();

  // Transmit counter value only if changed
  if (currentCounter != oldCounter)
  {
    getRegister(REGI_COUNTER)->getData();
    oldCounter = currentCounter;
  }

  enableINT1irq();
}

