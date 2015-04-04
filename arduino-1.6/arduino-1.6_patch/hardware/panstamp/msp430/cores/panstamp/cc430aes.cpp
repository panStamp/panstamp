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

#include "cc430aes.h"
#include "cc430f5137.h"

uint8_t CC430AES::aesKey[16];

/**
 * aesCrypto
 *
 * Apply AES-128 cryptographic algorithm
 *
 * @param data Data buffer to be encrypted/decrypted
 * @param len size of data in bytes.
 */
void CC430AES::aesCrypto(uint8_t *data, uint8_t len)
{
  uint8_t i = 0, j = 0, k, loop = 0;
  uint16_t intVal;
       
  // Update AES key
  updateKey();
  
  while(i < len)
  {
    loop++;
    
    intVal = data[i++];
    if (i < len)
      intVal |= (data[i++] << 8);
    AESADIN = intVal;

    if (loop == 8)  // 8 integers (16 bytes) passed to AESADIN?
    {
      // Wait whilst encrypting
      while(AESASTAT & AESBUSY);

      // Store encrypted data back into the buffer      
      for(k=0 ; k<loop ; k++)
      {
        intVal = AESADOUT;
        data[j++] = intVal & 0xFF;
        if (j < len)
          data[j++] = (intVal >> 8) & 0xFF;
      }

      loop = 0;
    }
  }
}

/**
 * ctrCrypto
 *
 * Apply AES-128 cryptographic algorithm along with CTR cipher method
 *
 * @param data Data buffer to be encrypted/decrypted
 * @param len size of data in bytes.
 * @param nonce initialization nonce used to start encrypting/decrypting
 * @param decrypt True in case of decryption, false for encryption
 */
void CC430AES::ctrCrypto(uint8_t *data, uint8_t len, uint32_t nonce, bool decrypt)
{
  uint8_t counter[16];
  int j;
  uint8_t i, k;
  uint8_t loops = len/16;
  uint32_t initNonce = nonce;
  
  if ((len % 16) > 0)
    loops++;

  // Iterate for every 16-byte packet extracted from data
  for(i=0 ; i<loops ; i++)
  {
    k = 0;
    for(j=3 ; j>=0 ; j--)
    {
      counter[j] = initNonce & 0xFF;
      counter[j+4] = initNonce & 0xFF;
      counter[j+8] = initNonce & 0xFF;
      counter[j+12] = initNonce & 0xFF;
      k += 8;
      initNonce = nonce >> k;
    }

    // Increment counter
    counter[15] += i;

    // Encrypt 16-byte counter
    aesEncrypt(counter, sizeof(counter));

    // For each byte in the 16-byte packet
    for(j=0 ; j<16 ; j++)
    {
      k = j + i*16;
      if (k < len)
      {       
        // XOR data against encypted nonce
        data[k] ^= counter[j];
      }
      else
        break;
    }
  }
}
