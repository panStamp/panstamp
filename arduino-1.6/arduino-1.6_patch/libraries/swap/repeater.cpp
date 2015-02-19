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
 * Creation date: 06/03/2013
 */

#include "repeater.h"
#include "panstamp.h"
//#include "Arduino.h"
#include "swap.h"

/**
 * init
 *
 * Initialize repeater
 *
 * @param maxHop maximum hop count
 */
void REPEATER::init(uint8_t maxHop)
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
 * @param packet Pointer to the SWAP packet received
 */
void REPEATER::packetHandler(SWPACKET *packet)
{
  static bool repeatPacket = true;
  static uint32_t currentTime;

  if (enabled)
  {
    // Don't repeat packets addressed to our device
    if (packet->destAddr != swap.devAddress)
    {
      // Don't repeat beyond the maximum hop count
      if (packet->hop < maxHopCount)
      {
        uint8_t i;        

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
                {
                  repeatPacket = false;   //Don't repeat packet
                  break;
                }
              }
            }
          }
        }

        // Repeat packet?
        if (repeatPacket)
        {
          packet->srcAddr = swap.devAddress;              // Modify source address
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
 * @param packet SWAP packet being repeated
 */
void REPEATER::saveTransaction(SWPACKET *packet)
{
  static uint8_t transactionIndex = 0;

  // Save current transaction in first position
  transactions[transactionIndex].timeStamp = millis();         // Current time stamp
  transactions[transactionIndex].function = packet->function;  // SWAP function
  transactions[transactionIndex].srcAddr = packet->srcAddr;    // Source address
  transactions[transactionIndex].nonce = packet->nonce;        // Cyclic nonce
  transactions[transactionIndex].regAddr = packet->regAddr;    // Register address
   
  transactionIndex = (transactionIndex + 1) % REPEATER_TABLE_DEPTH;
}

/**
 * start
 *
 * Start repeater
 */
void REPEATER::start(void)
{
  enabled = true;
}

/**
 * stop
 *
 * Stop repeater
 */
void REPEATER::stop(void)
{
  enabled = false;
}


