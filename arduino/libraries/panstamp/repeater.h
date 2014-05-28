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
 * Creation date: 10/01/2012
 */

#ifndef _REPEATER_H
#define _REPEATER_H

#include "Arduino.h"
#include "swpacket.h"
#include "datatypes.h"

/**
 * Macros
 */
#define start()     enabled = true
#define stop()      enabled = false

/**
 * Transaction
 */
typedef struct
{
  unsigned long timeStamp;  // Transmission time stamp (ms)
  byte function;            // SWAP function
  SWADDR srcAddr;           // Source address
  byte nonce;               // Cyclic nonce
  SWADDR regAddr;           // Register address
} Transaction;


/**
 * Cñass declaration
 */
class REPEATER
{
  private:
    /**
     * Maximum hop
     */
    byte maxHopCount;

    /**
     * Array of latest transactions
     */
    Transaction transactions[REPEATER_TABLE_DEPTH];

    /**
     * saveTransaction
     *
     * Save transaction in array
     *
     * 'packet': SWAP packet being repeated
     */
    void saveTransaction(SWPACKET *packet);

  public:
    /**
     * Enable flag
     */
    bool enabled;

    /**
     * init
     *
     * Initialize repeater
     *
     * 'maxHop': maximum hop count
     */
    void init(byte maxHop);

    /**
     * packetHandler
     *
     * Handle incoming packet. Repeat if necessary
     *
     * 'packet': Pointer to the SWAP packet received
     */
    void packetHandler(SWPACKET *packet);

    /**
     * Class constructor
     */
    REPEATER(void);
};

#endif

