/*
 * binouts
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
 * Creation date: 01/26/2012
 *
 * Device:
 * Binary output + PWM module
 *
 * Description:
 * Device providing 8 controllable binary (ON/OFF) outputs and 4 PWM
 * outputs. Each output is controlled by an indewpendent register so there
 * is no need to know the state of the outputs to be kept unmodified.
 * Optional repeater mode
 *
 * PANSTAMP_AVR and PANSTAMP_NRG
 * Binary outputs : pins 2, 4, 5, 6, 8, 9, 10 and 22
 * PWM outputs: pins 3, 18, 20 and 21
 *
 * This sketch can be used to control loads (ON/OFF) and even run some type
 * of progressive control via PWM (dim lights, control motors, blinds, etc)
 *
 * Associated Device Definition File, defining registers, endpoints and
 * configuration parameters:
 * binouts.xml (Binary/PWM output module)
 */
 
#include "regtable.h"
#include "swap.h"


/**
 * Maximum repeating count
 */
const byte maxRepeaterHop = 5;

#ifdef PANSTAMP_NRG
// Binary output pins (Arduino digital pins)
uint8_t binaryPin[] = {16, 18, 15, 14, 12, 11, 10, 1};
// PWM output pins (Arduino digital pins)
uint8_t pwmPin[] = {17, 2, 3, 5};
#else // PANSTAMP_AVR
// Binary output pins (Arduino digital pins)
uint8_t binaryPin[] = {8, 14, 15, 16, 17, 18, 19, 7};
// PWM output pins (Arduino digital pins)
uint8_t pwmPin[] = {9, 6, 5, 3};
#endif

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

  // Configure output pins
  for(i=0 ; i<sizeof(binaryPin) ; i++)
    pinMode(binaryPin[i], OUTPUT);
  for(i=0 ; i<sizeof(pwmPin) ; i++)
    pinMode(pwmPin[i], OUTPUT);

  // Init SWAP stack
  swap.init();
  
  // Optionally set transmission amplifier to its maximum level (10dB)
  //panstamp.setHighTxPower();

  // Enable repeater mode
  swap.enableRepeater(maxRepeaterHop);

  // Transmit product code
  swap.getRegister(REGI_PRODUCTCODE)->getData();

  // Enter SYNC state
  swap.enterSystemState(SYSTATE_RXON);
  // Transmit initial binary states
  for(i=0 ; i<sizeof(binaryPin) ; i++)
    swap.getRegister(REGI_BINOUTPUT0 + i)->getData();
  // Transmit initial PWM values
  for(i=0 ; i<sizeof(pwmPin) ; i++)
    swap.getRegister(REGI_PWMOUTPUT0 + i)->getData();
}

/**
 * loop
 *
 * Arduino main loop
 */
void loop()
{
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(4900);
}

