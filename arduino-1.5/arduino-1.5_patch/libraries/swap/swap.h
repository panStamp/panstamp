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
 * Creation date: 07/10/2014
 */

#ifndef _SWAP_H
#define _SWAP_H

#include "register.h"
#include "config.h"
#include "repeater.h"

#ifdef PANSTAMP_NRG
#include "cc430aes.h"
#endif

/**
 * Macros
 */
#define eepromToFactoryDefaults()   swap.nvolatToFactoryDefaults()

#define enableAntiPlayback()    security |= 0x01

#define swapNetworkId    radio.syncWord

#define setSwapStatusCallBack(ptrFunc)     statusReceived = ptrFunc

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
 * Types of SWAP message for attachInterrupt
 */
#define STATUS  0
#define QUERY   1
#define COMMAND 2

/**
 * Array of registers
 */
extern REGISTER* regTable[];
extern uint8_t regTableSize;

/**
 * Class: SWAP
 * 
 * Description:
 * SWAP protocol class
 */
class SWAP
{   
  public:      
    /**
     * Pointer to repeater object
     */
    REPEATER *repeater;

    /**
     * SWAP address
     */
    #ifdef SWAP_EXTENDED_ADDRESS
    uint16_t devAddress;
    #else
    uint8_t devAddress;
    #endif

    /**
     * Security options
     */
    uint8_t security;

    /**
     * Security cyclic nonce
     */
    uint8_t nonce;
    
    /**
     * System state
     */
    uint8_t systemState;

    /**
     * Interval between periodic transmissions. 0 for asynchronous transmissions
     */
    uint16_t txInterval;

    /**
     * Smart encryption password
     */
    uint8_t *encryptPwd;

    /**
     * SWAP status packet received. Callback function
     */
    void (*statusReceived)(SWPACKET *status);
    
    /**
     * enableRepeater
     *
     * Enable repeater mode
     *
     * @param maxHop Maximum repeater count. Zero if omitted
     */
    void enableRepeater(uint8_t maxHop=0);

    /**
     * SWAP
     *
     * Class constructor
     */
    SWAP(void);

    /**
     * init
     *
     * Initialize SWAP registers
     */
    void init(void);

    /**
     * enterSystemState
     *
     * Enter system state
     *
     * @param state New system state
     */
    void __inline__ enterSystemState(SYSTATE state)
    {
      // System state register -> id = 3
      regTable[3]->setData((uint8_t *) &state);
    }
    
    /**
     * goToSleep
     *
     * put the MCU in sleep mode during txInterval seconds
     */
    void goToSleep(void);

    /**
     * nvolatToFactoryDefaults
     * 
     * Write default config values in non-volatile memory
     */
    void nvolatToFactoryDefaults(void);

    /**
     * attachInterrupt
     * 
     * Declare custom ISR, to be called whenever a SWAP status packet is received
     * 
     * @param type of packet that triggers the user function
     * @param funct pointer to the custom function
     */
     inline void attachInterrupt(uint8_t type, void (*funct)(SWPACKET*))
     {
       if (type == STATUS)
         statusReceived = funct;
     }
     
    /**
     * getRegister
     *
     * Return pointer to register with ID = regId
     *
     * @param regId Register ID
     */
    inline REGISTER * getRegister(unsigned char regId)
    {
      return regTable[regId]; 
    }
    
    /**
     * setSmartPassword
     * 
     * Set Smart Encryption password
     * 
     * @param password Encryption password. 12-byte length
     */
    inline void setSmartPassword(unsigned char* password)
    {
      // Save password
      encryptPwd = password;
      // Enable Smart Encryption
      security |= 0x02;
    }
    
    /**
     * setAesPassword
     * 
     * Set AES-128 Encryption password
     * 
     * @param password Encryption password. It must be 16 byte length
     */
    #ifdef PANSTAMP_NRG
    inline void setAesPassword(unsigned char* password)
    {
      // Set AES-128 key
      CC430AES::setKey(password);
     
      // Enable AES Encryption
      security |= 0x04;
    }
    #endif
};

/**
 * Global SWAP object
 */
extern SWAP swap;

#endif

