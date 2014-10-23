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
 * Creation date: 07/23/2014
 */

/**
 * Basic beacon example
 *
 * This application sleeps (ultra-low-current consumption) most of the time and
 * wakes-up every five seconds to transmit a packet and then enters the sleep
 * mode again
 */

#define RFCHANNEL        0       // Let's use channel 0
#define SYNCWORD1        0xB5    // Synchronization word, high byte
#define SYNCWORD0        0x47    // Synchronization word, low byte
#define SOURCE_ADDR      5       // Device address

CCPACKET packet;

void setup()
{
  byte i;

  // Setup LED output pin
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
   
  panstamp.radio.setChannel(RFCHANNEL);
  panstamp.radio.setSyncWord(SYNCWORD1, SYNCWORD0);
  panstamp.radio.setDevAddress(SOURCE_ADDR);
  panstamp.radio.setCCregs();
  panstamp.setHighTxPower();

  packet.length = 10;
  
  for(i=0 ; i<packet.length ; i++)
    packet.data[i] = i;
}

void loop()
{
  digitalWrite(LED, HIGH);
  panstamp.radio.sendData(packet);
  digitalWrite(LED, LOW);
  panstamp.sleepSec(5);
}

