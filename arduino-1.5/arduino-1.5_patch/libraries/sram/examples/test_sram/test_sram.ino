/**
 * Copyright (c) 2015 panStamp <contact@panstamp.com>
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
 * Creation date: 02/09/2015
 *
 * Simple library to read/write external SPI 23K256 SRAM
 */
 
#include <SPI.h>
#include <sram.h>
#include <HardwareSerial.h>

SRAM ram(14); // Chip select pin = D14

uint16_t addr = 0;
uint8_t value = 0x0A;
uint8_t res = 0;

void setup()
{
  ram.begin();
  Serial.begin(9600);
}

void loop()
{
  ram.writeByte(addr, value++);  // Write address
  res = ram.readByte(addr++);    // Read address
  Serial.println(res, HEX);
  
  delay(2000);
}
