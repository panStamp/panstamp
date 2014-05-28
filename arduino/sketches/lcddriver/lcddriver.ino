/*
 * lcddriver
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
 * Alphanumeric LCD driver
 *
 * Description:
 * Driver for alphanumeric LCD's, up to 4 lines and 20 characters per line.
 * This device also provides 7 binary inputs for detecting puch buttons
 * and a LED pin.
 *
 * Binary inputs : pins 18, 20, 21 and 22
 * LCD pins : pins 4(RS), 5(E), 6(D4), 8(D5), 9(D6) and 10(D7). Pin 3 for backlight control
 * LED pin : pin 2
 *
 * Important, backlight control needs an external transistor.
 *
 * This sketch can be used to display custom messages and inter-actuate with
 * other SWAP devices via pushbutton presses. With a bit of work, this device
 * could be used as a complete room controller capable to browse and control
 * external values.
 *
 * This device is not low-power enabled so you will need to power it from an
 * external power supply. The reason for this is that this device needs to
 * continuously listening the network.
 *
 * Associated Device Definition File, defining registers, endpoints and
 * configuration parameters:
 * lcddriver.xml (Alphanumeric LCD driver module)
 */
 
#include "regtable.h"
#include "panstamp.h"
#include <LiquidCrystal.h>


/**
 * LCD: initialize the library with the numbers of the interface pins (Arduino digital pins)
 */
LiquidCrystal lcd(14, 15, 16, 17, 18, 19);  // PC0:PC5 used to drive the LCD
#define PIN_BACKLIGHT    9  // PB1 used as PWM output for the backlight control

/**
 * Macros
 */
#define pcEnableInterrupt()     PCICR = 0x04    // Enable Pin Change interrupt on port D
#define pcDisableInterrupt()    PCICR = 0x00    // Disable Pin Change interrupts

/**
 * LED pin
 */
#define LEDPIN   8

/**
 * Pin Change Interrupt flag
 */
volatile boolean pcIRQ = false;

/**
 * Binary inputs (push buttons)
 */
byte binaryPin[] = {3, 5, 6, 7};            // Binary pins (Atmega port bits)
#define BINARY_INP_PORT    PIND             // Atmega port register
int lastState[] = {-1, -1, -1, -1};         // Initial pin states
#define PCINTMASK    0xE8                   // PD[3], PD[5:7] - Pin Change interrupt mask
byte states;                                // Each bit represents a binary state

/**
 * Pin Change Interrupt vectors
 */
SIGNAL(PCINT2_vect)
{
  pcIRQ = true;
}

/**
 * swapStatusReceived
 *
 * Function automatically called by the panStamp API whenever a SWAP status
 * packet is received.
 * This function can be used to display values coming from other SWAP motes.
 *
 * 'status'    SWAP status packet received
 */
void swapStatusReceived(SWPACKET *status)
{
  // Do something with the status received
}
 
/**
 * updateValues
 *
 * Update binary state registers
 *
 * Return:
 * 0 -> No binary state change
 * 1 -> Binary states changed
 */
byte updateValues(void)
{
  byte i, res = 0;
  int state;

  states = 0;
  for(i=0 ; i<sizeof(binaryPin) ; i++)
  {
    state = !bitRead(BINARY_INP_PORT, binaryPin[i]);
    states |= state << i;
    if (lastState[i] != state)
    {
      lastState[i] = state;
      res = 1;
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
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);

  // Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("    panStamp    ");
  lcd.setCursor(0, 1);
  lcd.print("   LCD driver   ");

  // Configure binary pins as inputs
  DDRD &= ~PCINTMASK;
  
  // Set PC interrupt mask
  PCMSK2 = PCINTMASK;
  
  // Configure LCD backlight pin as output
  pinMode(PIN_BACKLIGHT, OUTPUT);
  
  // Init panStamp
  panstamp.init();

  // Transmit product code
  getRegister(REGI_PRODUCTCODE)->getData();

  // Enter SYNC state
  panstamp.enterSystemState(SYSTATE_RXON);
  
  //updateValues();
  // Transmit initial binary states
  getRegister(REGI_BININPUTS)->getData();
  
  // Read binary inputs
  updateValues();
  
  // Enable Pin Change interrupt
  pcEnableInterrupt();
}

/**
 * loop
 *
 * Arduino main loop
 */
void loop()
{
  if (pcIRQ)
  {
    pcDisableInterrupt();
    if(updateValues())
    {
      // Transmit binary states
      getRegister(REGI_BININPUTS)->getData();
    }
    //Ready to receive new PC interrupts
    pcIRQ = false;
    pcEnableInterrupt();
  }
}

