/*
 * binouts
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
#include "panstamp.h"
//#include "repeater.h"

/**
 * LED pin
 */
#define LEDPIN               4

/**
 * EEPROM addresses
 */
#define EEPROM_CONFIG_REPEATER   EEPROM_FIRST_CUSTOM

/**
 * Maximum repeating count
 */
byte maxRepeaterHop = 0;

/**
 * Binary output pins (Arduino pins)
 */
uint8_t binaryPin[] = {8, 14, 15, 16, 17, 18, 19, 7};

/**
 * PWM output pins (Arduino pins)
 */
uint8_t pwmPin[] = {9, 6, 5, 3};

/**
 * swapStatusReceived
 *
 * Function automatically called by the panStamp API whenever a SWAP status
 * packet is received
 *
 * 'status'    SWAP status packet received
 */
 void swapStatusReceived(SWPACKET *status)
 {
   // Place here you code if you want the outputs to change
   // according to the value of any external register/endpoint
 }

/**
 * readConfig
 *
 * Read configuration from EEPROM
 */
void readConfig(void)
{
  // Read config from EEPROM
  maxRepeaterHop = EEPROM.read(EEPROM_CONFIG_REPEATER);
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

  // Configure output pins
  for(i=0 ; i<sizeof(binaryPin) ; i++)
    pinMode(binaryPin[i], OUTPUT);
  for(i=0 ; i<sizeof(pwmPin) ; i++)
    pinMode(pwmPin[i], OUTPUT);

  // Init panStamp
  panstamp.init();

  // Enable repeater mode
  panstamp.enableRepeater(maxRepeaterHop);

  // Read config from EEPROM
  readConfig();

  // Transmit product code
  getRegister(REGI_PRODUCTCODE)->getData();

  // Enter SYNC state
  panstamp.enterSystemState(SYSTATE_RXON);
  // Transmit initial binary states
  for(i=0 ; i<sizeof(binaryPin) ; i++)
    getRegister(REGI_BINOUTPUT0 + i)->getData();
  // Transmit initial PWM values
  for(i=0 ; i<sizeof(pwmPin) ; i++)
    getRegister(REGI_PWMOUTPUT0 + i)->getData();
}

/**
 * loop
 *
 * Arduino main loop
 */
void loop()
{
  digitalWrite(LEDPIN, HIGH);
  delay(100);
  digitalWrite(LEDPIN, LOW);
  delay(4900);
}

