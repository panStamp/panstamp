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
 * Creation date: 06/03/2013
 */

#ifndef _REPEATER_H
#define _REPEATER_H

#include "swpacket.h"
#include "config.h"

/**
 * Transaction
 */
typedef struct
{
  unsigned long timeStamp;  // Transmission time stamp (ms)
  unsigned char function;   // SWAP function
  unsigned char srcAddr;    // Source address
  unsigned char nonce;      // Cyclic nonce
  unsigned char regAddr;    // Register address
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
    uint8_t maxHopCount;

    /**
     * Array of latest transactions
     */
    Transaction transactions[REPEATER_TABLE_DEPTH];

    /**
     * saveTransaction
     *
     * Save transaction in array
     *
     * @param packet SWAP packet being repeated
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
     * @param maxHop maximum hop count
     */
    void init(uint8_t maxHop);

    /**
     * packetHandler
     *
     * Handle incoming packet. Repeat if necessary
     *
     * @param packet Pointer to the SWAP packet received
     */
    void packetHandler(SWPACKET *packet);

    /**
     * Class constructor
     */
    REPEATER(void);

    /**
     * start
     *
     * Start repeater
     */
    void start(void);

    /**
     * stop
     *
     * Stop repeater
     */
    void stop(void);
};

#endif

