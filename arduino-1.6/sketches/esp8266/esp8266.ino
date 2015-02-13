/*
 * binouts
 *
 * Copyright (c) 2015 panStamp <contact@panstamp.com>
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
 * Creation date: 01/30/2015
 *
 * Device:
 * Wifi gateway
 *
 * Description:
 * Wifi gateway around the ESP8266 module. with extended RAM memory and SD
 * flash
 *
 * Associated Device Definition File, defining registers, endpoints and
 * configuration parameters:
 * ipgateway.xml (IP gateway)
 */

#include "HardwareSerial.h"
//#include "SPI.h"
#include "sram.h"

#include "regtable.h"
#include "swap.h"

SRAM mem(14);
uint8_t res;

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
 * setup
 *
 * Arduino setup function
 */
void setup()
{
  Serial.begin(9600);
  Serial.flush();
  delay(200);
  Serial.println("Ready");

  delay(100);

  uint8_t res = 5;
  mem.writeByte(0, 0xAA);
}

/**
 * loop
 *
 * Arduino main loop
 */
void loop()
{
//  digitalWrite(LED, HIGH);
//  delay(100);
//  res = mem.readByte(0);
  Serial.println(res, HEX);
//  digitalWrite(LED, LOW);
  delay(4900);
}

