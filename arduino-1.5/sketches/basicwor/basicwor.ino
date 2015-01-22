/**
 * Copyright (c) 2014 panStamp <contact@panstamp.com>
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
 * Creation date: 01/21/2015
 */

/**
 * Basic radio example with Wake-On-Radio
 *
 * Loe-power receiver sleeping most of the time with WOR enabled.
 * The on-board LED toggles whenever a wireless packet is received
 */
 
#define RFCHANNEL        0       // Let's use channel 0
#define SYNCWORD1        0xB5    // Synchronization word, high byte
#define SYNCWORD0        0x47    // Synchronization word, low byte
#define SOURCE_ADDR      4       // Sender address

CCPACKET txPacket;  // packet object

byte count = 0;

/**
 * This function is called whenever a wireless packet is received
 */
void rfPacketReceived(CCPACKET *packet)
{   
  if (packet->length > 1)
  {
    // The LED will toggle when a new packet is received
    digitalWrite(LED, !digitalRead(LED));
  }
}

void setup()
{
  // Setup LED output pin
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  panstamp.radio.setChannel(RFCHANNEL);
  panstamp.radio.setSyncWord(SYNCWORD1, SYNCWORD0);
  panstamp.radio.setDevAddress(SOURCE_ADDR);
  panstamp.radio.setCCregs();

  // Let's disable address check for the current project so that our device
  // will receive packets even not addressed to it.
  panstamp.radio.disableAddressCheck();

  // Declare RF callback function
  panstamp.setPacketRxCallback(rfPacketReceived);
}

void loop()
{  
  panstamp.radio.setWorState(100);
  delay(1);
}

