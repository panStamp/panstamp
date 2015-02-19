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

#ifndef _SWPACKET_H
#define _SWPACKET_H

#include "ccpacket.h"
#include "datatypes.h"

/**
 * SWAP definitions
 */
#ifdef SWAP_EXTENDED_ADDRESS
#define SWAP_DATA_HEAD_LEN     9
#else
#define SWAP_DATA_HEAD_LEN     6
#endif
#define SWAP_REG_VAL_LEN       CC1101_DATA_LEN - SWAP_DATA_HEAD_LEN     // SWAP data payload - max length
#define SWAP_BCAST_ADDR        0x00                                     // SWAP broadcast address
#define SWAP_NB_TX_TRIES       3                                        // Number of transmission retries
#define SWAP_TX_DELAY          (panstamp.radio.devAddress & 0xFF) * 2   // Delay before sending
#define SWAP_EXTENDED_ADDRESS_BIT  0x80

/**
 * SWAP message functions
 */
enum SWAPFUNCT
{
  SWAPFUNCT_STA = 0x00,
  SWAPFUNCT_QRY,
  SWAPFUNCT_CMD,
  SWAPFUNCT_ACK  // 0x03 - Reserved for future adoption by SWAP
};

/**
 * Adressing schema
 */
enum SWAPADDR_SCHEMA
{
  SWAPADDR_SIMPLE = 0,
  SWAPADDR_EXTENDED
};

/**
 * Macros
 */
#ifndef SWAP_EXTENDED_ADDRESS
#define smartDecrypt()         smartEncrypt(true)
#endif

/**
 * SWDATA : SWAP data structure
 */
struct SWDATA
{
    /**
     * Data buffer
     */
    uint8_t *data;

    /**
     * Data length
     */
    uint8_t length;

    /**
     * Data type
     */
    SWDTYPE type;
};

class SWPACKET
{
  private:
    /**
     * Raw packet
     */
     CCPACKET ccPacket;
     
    /**
     * smartEncrypt
     * 
     * Apply Smart Encryption to the SWAP packet passed as argument
     *
     * 'decrypt': if true, Decrypt packet. Encrypt otherwise
     */
    #ifndef SWAP_EXTENDED_ADDRESS
    void smartEncrypt(bool decrypt=false);
    #endif

    /**
     * aesCrypto
     * 
     * Apply AES-128 encryption with CTR cipher to the SWAP packet passed
     * as argument
     */
    //#ifdef PANSTAMP_NRG
    void aesCrypto(void);
    //#endif

  public:
    /**
     * Destination address
     */
    uint16_t destAddr;

    /**
     * Source address
     */
    uint16_t srcAddr;

    /**
     * Hop counter. Incremented each time the message is repeated
     */
    uint8_t hop;

    /**
     * Security option byte
     */
    uint8_t security;

    /**
     * Security cyclic nonce
     */
    uint8_t nonce;

    /**
     * Function byte
     */
    uint8_t function;

    /**
     * Type of addressing schema
     */
    uint8_t addrType;

    /**
     * Register address
     */
    uint8_t regAddr;

    /**
     * Register id
     */
    uint8_t regId;

    /**
     * Register value
     */
    SWDATA value;

    /**
     * SWPACKET
     * 
     * Class constructor
     * 
     * @param packet Pointer to the raw CC1101 packet
     */
    SWPACKET(CCPACKET *packet);

    /**
     * SWPACKET
     * 
     * Class constructor
     */
    SWPACKET(void);
    
    /**
     * send
     * 
     * Send SWAP packet
     *
     * #return:
     *  True if the transmission succeeds
     *  False otherwise
     */
    bool send(void);
};

#endif
