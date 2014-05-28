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
#include "panstamp.h"

/**
 * SWPACKET
 * 
 * Class constructor
 * 
 * 'packet'	Raw CC1101 packet
 */
SWPACKET::SWPACKET(CCPACKET packet) 
{
  hop = (packet.data[2] >> 4) & 0x0F;
  security = packet.data[2] & 0x0F;
  nonce = packet.data[3];
  function = packet.data[4] & ~SWAP_EXTENDED_ADDRESS_BIT;

  if (packet.data[4] & SWAP_EXTENDED_ADDRESS_BIT)
  {
    addrType = SWAPADDR_EXTENDED;
    destAddr = packet.data[0];
    destAddr <<= 8;
    destAddr |= packet.data[1];
    srcAddr = packet.data[5];
    srcAddr <<= 8;
    srcAddr |= packet.data[6];
    regAddr = packet.data[7];
    regAddr <<= 8;
    regAddr |= packet.data[8];
    regId = packet.data[9];
  }
  else
  {
    addrType = SWAPADDR_SIMPLE;
    destAddr = packet.data[0];
    srcAddr = packet.data[1];
    hop = (packet.data[2] >> 4) & 0x0F;
    security = packet.data[2] & 0x0F;
    nonce = packet.data[3];
    regAddr = packet.data[5];
    regId = packet.data[6];
  }

  value.data = packet.data + SWAP_DATA_HEAD_LEN + 1;
  value.length = packet.length - SWAP_DATA_HEAD_LEN - 1;

  // Smart encryption only available for simple (1-byte) addressing schema
  #ifndef SWAP_EXTENDED_ADDRESS
  if (addrType == SWAPADDR_SIMPLE)
  {
    // Need to decrypt packet?
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
 * Return:
 *  True if the transmission succeeds
 *  False otherwise
 */
boolean SWPACKET::send(void)
{
  CCPACKET packet;
  byte i;
  boolean res;

  // LE -> BE conversion for numeric values
  if (value.type == SWDTYPE_INTEGER)
  {
    for(i=0 ; i<value.length ; i++)
      packet.data[i+SWAP_DATA_HEAD_LEN + 1] = value.data[value.length-1-i];
  }
  else
  {
    for(i=0 ; i<value.length ; i++)
      packet.data[i+SWAP_DATA_HEAD_LEN + 1] = value.data[i];
  }

  // Smart encryption only available for simple (1-byte) addressing schema
  #ifndef SWAP_EXTENDED_ADDRESS
    // Need to encrypt packet?
    if (security & 0x02)
      smartEncrypt();
  #endif

  packet.length = value.length + SWAP_DATA_HEAD_LEN + 1;

  packet.data[2] = (hop << 4) & 0xF0;
  packet.data[2] |= security & 0x0F;
  packet.data[3] = nonce;

  #ifdef SWAP_EXTENDED_ADDRESS
    addrType = SWAPADDR_EXTENDED;
    packet.data[0] = (destAddr >> 8) & 0xFF;
    packet.data[1] = destAddr & 0xFF;
    packet.data[4] = function | SWAP_EXTENDED_ADDRESS_BIT;
    packet.data[5] = (srcAddr >> 8) & 0xFF;
    packet.data[6] = srcAddr & 0xFF;
    packet.data[7] = (regAddr >> 8) & 0xFF;
    packet.data[8] = regAddr & 0xFF;
    packet.data[9] = regId;
  #else
    addrType = SWAPADDR_SIMPLE;
    packet.data[0] = destAddr;
    packet.data[1] = srcAddr;
    packet.data[4] = function;
    packet.data[5] = regAddr;
    packet.data[6] = regId;
  #endif

  i = SWAP_NB_TX_TRIES;
  while(!(res = panstamp.cc1101.sendData(packet)) && i>1)
  {
    i--;
    delayMicroseconds(SWAP_TX_DELAY * 1000);
  }

  return res;
}

/**
 * smartEncrypt
 * 
 * Apply Smart Encryption to the SWAP packet passed as argument
 *
 * 'decrypt': if true, Decrypt packet. Encrypt otherwise
 */
#ifndef SWAP_EXTENDED_ADDRESS
void SWPACKET::smartEncrypt(bool decrypt) 
{
  byte i, j = 0;
  static byte newData[CC1101_DATA_LEN];

  if (decrypt)
    nonce ^= panstamp.encryptPwd[9];

  function ^= panstamp.encryptPwd[11] ^ nonce;
  srcAddr ^= panstamp.encryptPwd[10] ^ nonce;
  regAddr ^= panstamp.encryptPwd[8] ^ nonce;
  regId ^= panstamp.encryptPwd[7] ^ nonce;

  for(i=0 ; i<value.length ; i++)
  {
    newData[i] = value.data[i] ^ panstamp.encryptPwd[j] ^ nonce;
    j++;
    if (j == 11)  // Don't re-use last byte from password
      j = 0;
  }
  if (value.length > 0)
    value.data = newData;

  if (!decrypt)
    nonce ^= panstamp.encryptPwd[9];
}
#endif

