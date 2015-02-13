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
 * Creation date: 09/22/2014
 */

#ifndef _SWAP_H
#define _SWAP_H

#include "datatypes.h"
#include "cc430flash.h"
#include "cc430radio.h"
#include "memconfig.h"

/**
 * SWADDR : SWAP address data type
 */
#ifdef SWAP_EXTENDED_ADDRESS
#define SWADDR uint16_t
#else
#define SWADDR uint8_t
#endif

/**
 * SWAP message functions
 */
enum SWAPFUNCT
{
  SWAPFUNCT_STA = 0x00,
  SWAPFUNCT_QRY,
  SWAPFUNCT_CMD
};

/**
 * Register index
 */
enum REGISTERS
{
  REGI_PRODUCTCODE = 0,
  REGI_HWVERSION,
  REGI_FWVERSION,
  REGI_SYSSTATE,
  REGI_FREQCHANNEL,
  REGI_SECUOPTION,
  REGI_PASSWORD,
  REGI_SECUNONCE,
  REGI_NETWORKID,
  REGI_DEVADDRESS,
  REGI_TXINTERVAL,
  REGI_FIRMWARE
};

/**
 * System states
 */
enum SYSTATE
{
  SYSTATE_RESTART = 0,
  SYSTATE_RXON,
  SYSTATE_RXOFF,
  SYSTATE_SYNC,
  SYSTATE_LOWBAT,
  SYSTATE_UPGRADE
};

/**
 * SWAP definitions
 */
#define SWAP_REG_VAL_LEN       CC1101_DATA_LEN - SWAP_DATA_HEAD_LEN     // SWAP data payload - max length
#define SWAP_BCAST_ADDR        0x00                                     // SWAP broadcast address
#define SWAP_NB_TX_TRIES       3                                        // Number of transmission retries
//#define SWAP_TX_DELAY          (panstamp.radio.devAddress & 0xFF) * 2   // Delay before sending
#define SWAP_EXTENDED_ADDRESS_BIT  0x80

/**
 * SWAP PACKET definitions
 */
#ifdef SWAP_EXTENDED_ADDRESS
#define SWAP_DATA_HEAD_LEN     9
#define SWAP_ADDRESS_LEN       2
#define SWAP_ADDRESS_POS       5
#define SERVER_SWAP_ADDRESS    {0, 1}
#else
#define SWAP_DATA_HEAD_LEN     6
#define SWAP_ADDRESS_LEN       1
#define SWAP_ADDRESS_POS       1
#define SERVER_SWAP_ADDRESS    {1}
#endif

#define SWAP_ADDRTYPE   data[4] & SWAP_EXTENDED_ADDRESS_BIT
#define SWAP_FUNCTION   data[4] & ~SWAP_EXTENDED_ADDRESS_BIT
#define SWAP_REGID      data[SWAP_DATA_HEAD_LEN]

/**
 * Adressing schema
 */
//#define SWAP_EXTENDED_ADDRESS 1
#define SWAPADDR_SIMPLE 0
#define SWAPADDR_EXTENDED 0x80

class SWAP
{
  public:
  
    /**
     * Radio object
     */
    CC430RADIO radio;

    /**
     * SWAP address
     */
    uint8_t devAddress[2];
    
    /**
     * init
     * 
     * Init SWAP comms
     */
    ALWAYS_INLINE
    void init(uint8_t freq=CFREQ_868)
    {
      uint8_t buf[2];
      CC430FLASH flash;
      
      // RF channel
      flash.read((uint8_t *)INFOMEM_FREQ_CHANNEL, buf, 1);  
      if (buf[0] != 0xFF)
        radio.channel = buf[0];

      // Network ID
      flash.read((uint8_t *)INFOMEM_SYNC_WORD, buf, 2);
      if (buf[0] != 0xFF && buf[1] != 0xFF)
      {
        radio.syncWord[0] = buf[0];
        radio.syncWord[1] = buf[1];
      }

      // Device address
      flash.read((uint8_t *)INFOMEM_DEVICE_ADDRESS, devAddress, SWAP_ADDRESS_LEN);  
      if (devAddress[0] != 0xFF)
        radio.devAddress = devAddress[SWAP_ADDRESS_LEN-1];

      // Init radio module
      radio.init(freq);
    }
    
    /**
     * sendPacket
     * 
     * Send SWAP packet
     *
     * @param funct Function code
     * @param regId Register ID
     * @param val   Register value
     * @param len   Register length
     */
    void sendPacket(uint8_t funct, uint8_t regId, uint8_t *val, uint8_t len);
    
    /**
     * sendPacketVal
     *
     * Send SWAP packet. Accept different types as data payload
     *
     * @param funct Function code
     * @param regId Register ID
     * @param val   Register value
     */
    template<class T> void sendPacketVal(uint8_t funct, uint8_t regId, T val)
    {
      int i;
      uint8_t buf[4];

      for(i=sizeof(val) ; i>0 ; i--)
      {
        buf[i-1] = val & 0xFF;
        val >>= 8;
      }

      sendPacket(funct, regId, buf, sizeof(val));
    }
};

#endif

