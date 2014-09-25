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

#include "swap.h"

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
void SWAP::sendPacket(uint8_t funct, uint8_t regId, uint8_t *val, uint8_t len) 
{
  uint8_t i;
  CCPACKET packet;
  static uint8_t nonce = 0;

  packet.length = SWAP_DATA_HEAD_LEN + 1 + len;

  packet.data[2] = 0;
  packet.data[3] = nonce++;
  packet.data[4] = funct;

  #ifdef SWAP_EXTENDED_ADDRESS
    packet.data[0] = 0;   // Destination address
    packet.data[1] = (funct != SWAPFUNCT_STA);   // Send queries and commands to address 1, broadcast statuses
    packet.data[4] |= SWAP_EXTENDED_ADDRESS_BIT;
    if (funct == SWAPFUNCT_STA)
    {
      packet.data[5] = devAddress[0];
      packet.data[6] = devAddress[1];
    }
    else
    {
      packet.data[7] = 0;
      packet.data[8] = 1;
    }
    packet.data[9] = regId;
  #else
    packet.data[0] = (funct != SWAPFUNCT_STA);   // Send queries and commands to address 1, broadcast statuses
    packet.data[1] = devAddress[0];
    if (funct == SWAPFUNCT_STA)
      packet.data[5] = devAddress[0];
    else
      packet.data[5] = 1;
    packet.data[6] = regId;
  #endif

  for(i=0 ; i<len ; i++)
    packet.data[SWAP_DATA_HEAD_LEN + 1 + i] = val[i];

  radio.sendData(packet);
}
