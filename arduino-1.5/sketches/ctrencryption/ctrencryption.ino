/**
 * Copyright (c) 2014 Daniel Berenguer <contact@panstamp.com>
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
 * Creation date: 10/19/2014
 */
 
/**
 * This sketch is only compatible with the NRG platform
 */
 
#include "HardwareSerial.h"
#include "cc430aes.h"

void setup()
{ 
  Serial.begin(57600);
  
  // 128-bit key
  byte key[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
  byte data[] = "Data to be encrypted/decrypted does not need to be aligned to 16 bytes";
  long nonce = 1654;
  
  Serial.print("Input string: ");
  Serial.println((char*) data);
  Serial.println("Setting AES-128 key");

  // Set key
  CC430AES::setKey(key);

  Serial.println("Encripting...");
  // Encrypt
  CC430AES::ctrCrypto(data, sizeof(data)-1, nonce);

  Serial.println("Data encrypted");
  Serial.println("Now decripting...");

  // Decrypt
  CC430AES::ctrCrypto(data, sizeof(data)-1, nonce);

  Serial.print("Data decrypted: ");
  Serial.println((char*) data);
}

void loop()
{
}
