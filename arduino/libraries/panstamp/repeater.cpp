/**
 * Copyright (c) 2011 panStamp <contact@panstamp.com>
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
 * Creation date: 10/02/2012
 */

#include "repeater.h"
#include "swpacket.h"
#include "panstamp.h"

/**
 * init
 *
 * Initialize repeater
 *
 * 'maxHop': maximum hop count
 */
void REPEATER::init(byte maxHop)
{
  maxHopCount = maxHop;
  start();
}

/**
 * Class constructor
 */
REPEATER::REPEATER(void)
{
}

/**
 * packetHandler
 *
 * Handle incoming packet. Repeat if necessary
 *
 * 'packet': Pointer to the SWAP packet received
 */
void REPEATER::packetHandler(SWPACKET *packet)
{
  bool repeatPacket = true;
  static unsigned long currentTime;

  if (enabled)
  {
    // Don't repeat packets addressed to our device
    if (packet->destAddr != panstamp.swapAddress)
    {
      // Don't repeat beyond the maximum hop count
      if (packet->hop < maxHopCount)
      {
        byte i;        

        // Check received packet against the latest transactions
        for(i=0 ; i<REPEATER_TABLE_DEPTH ; i++)
        {
          // Same source/destination node?
          if (transactions[i].regAddr == packet->regAddr)
          {
            // Same SWAP function?
            if (transactions[i].function == packet->function)
            {
              // Same cyclic nonce?
              if (transactions[i].nonce == packet->nonce)
              {
                currentTime = millis();
                // Time stamp not expired?
                if ((currentTime - transactions[i].timeStamp) < REPEATER_EXPIRATION_TIME)
                  repeatPacket = false;   //Don't repeat packet
              }
            }
          }
        }

        // Repeat packet?
        if (repeatPacket)
        {
          packet->srcAddr = panstamp.swapAddress;          // Modify source address
          packet->hop++;                                  // Increment hop counter
          delay(SWAP_TX_DELAY);                           // Delay before sending
          if (packet->send())                             // Repeat packet
            saveTransaction(packet);                      // Save transaction
        }
      }
    }
  }
}

/**
 * saveTransaction
 *
 * Save transaction in array
 *
 * 'packet': SWAP packet being repeated
 */
void REPEATER::saveTransaction(SWPACKET *packet)
{
  byte i;

  // Move all packets one position forward
  for(i=REPEATER_TABLE_DEPTH-1 ; i>0 ; i--)
    transactions[i] = transactions[i-1];

  // Save current transaction in first position
  transactions[0].timeStamp = millis();         // Current time stamp
  transactions[0].function = packet->function;  // SWAP function
  transactions[0].srcAddr = packet->srcAddr;    // Source address
  transactions[0].nonce = packet->nonce;        // Cyclic nonce
  transactions[0].regAddr = packet->regAddr;    // Register address
}

