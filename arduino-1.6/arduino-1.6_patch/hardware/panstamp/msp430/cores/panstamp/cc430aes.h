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
 * Creation date: 10/14/2014
 */

#ifndef _CC430AES_H
#define _CC430AES_H

#include "wiring.h"

#define AES_ENCRYPTION_MODE()    AESACTL0 &= ~BIT1 & ~BIT0
#define AES_DECRYPTION_MODE()    AESACTL0 &= ~BIT1; AESACTL0 |= BIT0

/**
 * Class: CC430AES
 * 
 * Description:
 * AES-128 Encryption/Decryption calss
 */
class CC430AES
{
  private:
    /**
     * 128-bit key
     */
    static uint8_t aesKey[16];
    
    /**
     * updateKey
     *
     * Update AES-128 key
     */
    inline static void updateKey(void)
    {
      uint8_t i;
      
      for(i=0 ; i<8 ; i++)
      {       
        AESAKEY = (aesKey[i*2+1] << 8) | aesKey[i*2];
      }
    }

    /**
     * aesCrypto
     *
     * Apply AES-128 cryptographic algorithm
     *
     * @param data Data buffer to be encrypted/decrypted
     * @param len size of data in bytes.
     */
    static void aesCrypto(uint8_t *data, uint8_t len);

  public:
    /**
     * setKey
     *
     * Set AES-128 key value
     *
     * @param key AES 128-bit key
     */
    inline static void setKey(uint8_t *key)
    {
      uint8_t i;
      
      for(i=0 ; i<16 ; i++)
        aesKey[i] = key[i];
    }
   
    /**
     * aesEncrypt
     *
     * Perform AES-128 encryption
     *
     * @param data Data buffer to be encrypted
     * @param len size of data in bytes. Default size = 16 bytes
     */
    inline static void aesEncrypt(uint8_t *data, uint8_t len=16)
    { 
      // Configure for encryption
      AES_ENCRYPTION_MODE();

      aesCrypto(data, len);
    }
    
    /**
     * aesDecrypt
     *
     * Perform AES-128 decryption
     *
     * @param data Data buffer to be decrypted
     * @param len size of data in bytes. Default size = 16 bytes
     */
    inline static void aesDecrypt(uint8_t *data, uint8_t len=16)
    { 
      // Configure for decryption
      AES_DECRYPTION_MODE();
      
      aesCrypto(data, len);
    }
    
    /**
     * ctrCrypto
     *
     * Apply AES-128 cryptographic algorithm togheter with CTR cipher method
     *
     * @param data Data buffer to be encrypted/decrypted
     * @param len size of data in bytes.
     * @param nonce initialization nonce used to start encrypting/decrypting
     * @param decrypt True in case of decryption, false for encryption
     */
    static void ctrCrypto(uint8_t *data, uint8_t len, uint32_t nonce, bool decrypt=false);
};

#endif

