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
 * Creation date: 03/03/2011
 */

#include "swpacket.h"
#include "swap.h"
#include "panstamp.h"

/**
 * SWPACKET
 * 
 * Class constructor
 * 
 * @param packet Pointer to the raw CC1101 packet
 */
SWPACKET::SWPACKET(CCPACKET *packet) 
{
  uint8_t i;
   
  // Save raw data and length
  ccPacket.length = packet->length;
  for(i=0 ; i<ccPacket.length ; i++)
    ccPacket.data[i] = packet->data[i];
  
  hop = (ccPacket.data[2] >> 4) & 0x0F;
  security = ccPacket.data[2] & 0x0F;
  
  #ifdef PANSTAMP_NRG
  // AES-128 encrypted?
  if (security & 0x04)
    aesCrypto();  // Decrypt
  #endif
  
  nonce = ccPacket.data[3];
  function = ccPacket.data[4] & ~SWAP_EXTENDED_ADDRESS_BIT;

  if (ccPacket.data[4] & SWAP_EXTENDED_ADDRESS_BIT)
  {
    addrType = SWAPADDR_EXTENDED;
    destAddr = ccPacket.data[0];
    destAddr <<= 8;
    destAddr |= ccPacket.data[1];
    srcAddr = ccPacket.data[5];
    srcAddr <<= 8;
    srcAddr |= ccPacket.data[6];
    regAddr = ccPacket.data[7];
    regAddr <<= 8;
    regAddr |= ccPacket.data[8];
    regId = ccPacket.data[9];
  }
  else
  {
    addrType = SWAPADDR_SIMPLE;
    destAddr = ccPacket.data[0];
    srcAddr = ccPacket.data[1];
    regAddr = ccPacket.data[5];
    regId = ccPacket.data[6];
  }

  value.data = ccPacket.data + SWAP_DATA_HEAD_LEN + 1;
  value.length = ccPacket.length - SWAP_DATA_HEAD_LEN - 1;

  // Smart encryption only available for simple (1-byte) addressing schema
  #ifndef SWAP_EXTENDED_ADDRESS
  if (addrType == SWAPADDR_SIMPLE)
  {
    // Smart Encryption - Need to decrypt packet?
    if (security & 0x02)
      smartDecrypt();
  }
  #endif
}

/**
 * SWPACKET
 * 
 * Class constructor
 */
SWPACKET::SWPACKET(void) 
{
}

/**
 * send
 * 
 * Send SWAP packet. Do up to 5 retries if necessary
 *
 * @return
 *  True if the transmission succeeds
 *  False otherwise
 */
bool SWPACKET::send(void)
{
  byte i;
  boolean res;

  // LE -> BE conversion for numeric values
  if (value.type == SWDTYPE_INTEGER)
  {
    for(i=0 ; i<value.length ; i++)
      ccPacket.data[i+SWAP_DATA_HEAD_LEN + 1] = value.data[value.length-1-i];
  }
  else
  {
    for(i=0 ; i<value.length ; i++)
      ccPacket.data[i+SWAP_DATA_HEAD_LEN + 1] = value.data[i];
  }

  // Smart encryption only available for simple (1-byte) addressing schema
  #ifndef SWAP_EXTENDED_ADDRESS
    // Need to encrypt packet?
    if (security & 0x02)
      smartEncrypt();
  #endif

  ccPacket.length = value.length + SWAP_DATA_HEAD_LEN + 1;

  ccPacket.data[2] = (hop << 4) & 0xF0;
  ccPacket.data[2] |= security & 0x0F;
  ccPacket.data[3] = nonce;

  #ifdef SWAP_EXTENDED_ADDRESS
    addrType = SWAPADDR_EXTENDED;
    ccPacket.data[0] = (destAddr >> 8) & 0xFF;
    ccPacket.data[1] = destAddr & 0xFF;
    ccPacket.data[4] = function | SWAP_EXTENDED_ADDRESS_BIT;
    ccPacket.data[5] = (srcAddr >> 8) & 0xFF;
    ccPacket.data[6] = srcAddr & 0xFF;
    ccPacket.data[7] = (regAddr >> 8) & 0xFF;
    ccPacket.data[8] = regAddr & 0xFF;
    ccPacket.data[9] = regId;
  #else
    addrType = SWAPADDR_SIMPLE;
    ccPacket.data[0] = destAddr;
    ccPacket.data[1] = srcAddr;
    ccPacket.data[4] = function;
    ccPacket.data[5] = regAddr;
    ccPacket.data[6] = regId;
  #endif

  #ifdef PANSTAMP_NRG
  // Need to be AES-128 encrypted?
  if (security & 0x04)
    aesCrypto();  // Encrypt
  #endif

  i = SWAP_NB_TX_TRIES;
  while(!(res = panstamp.radio.sendData(ccPacket)) && i>1)
  {
    i--;
    delay(SWAP_TX_DELAY);
  }

  return res;
}

/**
 * smartEncrypt
 * 
 * Apply Smart Encryption to the SWAP packet passed as argument
 *
 * @param decrypt if true, Decrypt packet. Encrypt otherwise
 */
#ifndef SWAP_EXTENDED_ADDRESS
void SWPACKET::smartEncrypt(bool decrypt) 
{
  byte i, j = 0;
  static uint8_t newData[CCPACKET_DATA_LEN];

  if (decrypt)
    nonce ^= swap.encryptPwd[9];

  function ^= swap.encryptPwd[11] ^ nonce;
  srcAddr ^= swap.encryptPwd[10] ^ nonce;
  regAddr ^= swap.encryptPwd[8] ^ nonce;
  regId ^= swap.encryptPwd[7] ^ nonce;

  for(i=0 ; i<value.length ; i++)
  {
    newData[i] = value.data[i] ^ swap.encryptPwd[j] ^ nonce;
    j++;
    if (j == 11)  // Don't re-use last byte from password
      j = 0;
  }
  if (value.length > 0)
    value.data = newData;

  if (!decrypt)
    nonce ^= swap.encryptPwd[9];
}
#endif

/**
 * aesCrypto
 * 
 * Apply AES-128 encryption with CTR cipher to the SWAP packet passed
 * as argument
 */
#ifdef PANSTAMP_NRG
void SWPACKET::aesCrypto(void) 
{
  uint8_t i;
  uint32_t initNonce = 0;
 
  // Create initial CTR nonce with first four bytes
  // None of these fields are encrypted
  for(i=0 ; i<4 ; i++)
  {
    initNonce <<= 8;
    initNonce |= ccPacket.data[i];
  }
  
  CC430AES::ctrCrypto(ccPacket.data + 4, ccPacket.length - 4, initNonce);
}
#endif
