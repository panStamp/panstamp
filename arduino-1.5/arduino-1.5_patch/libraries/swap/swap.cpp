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

#include "panstamp.h"
#include "swap.h"
#include "swpacket.h"
#include "commonregs.h"


DEFINE_COMMON_REGINDEX_START()
DEFINE_COMMON_REGINDEX_END()

/**
 * Array of registers
 */
//extern REGISTER* regTable[];
//extern uint8_t regTableSize;

/**
 * pacKetReceived
 *
 * CCPACKET received
 *
 * @param packet CCPACKET received
 */
void pacKetReceived(CCPACKET *packet)
{
  static SWPACKET swPacket = SWPACKET(*packet);
  REGISTER *reg;
  bool eval = true;


  #ifdef SWAP_EXTENDED_ADDRESS
  if (swPacket.addrType == SWAPADDR_EXTENDED)
  #else
  if (swPacket.addrType == SWAPADDR_SIMPLE)
  #endif
  {
    // Repeater enabled?
    if (swap.repeater != NULL)
      swap.repeater->packetHandler(&swPacket);

    // Smart encryption locally enabled?
    if (swap.security & 0x02)
    {
      // OK, then incoming packets must be encrypted too
      if (!(swPacket.security & 0x02))
        eval = false;
    }
  }
  else
    eval = false;
    
  if (eval)
  {
    // Function
    switch(swPacket.function)
    {
      case SWAPFUNCT_CMD:
        // Command not addressed to us?
        if (swPacket.destAddr != swap.devAddress)
          break;
        // Current version does not support data recording mode
        // so destination address and register address must be the same
        if (swPacket.destAddr != swPacket.regAddr)
          break;
        // Valid register?
        if ((reg = getRegister(swPacket.regId)) == NULL)
          break;
        // Anti-playback security enabled?
        if (swap.security & 0x01)
        {
          // Check received nonce
          if (swap.nonce != swPacket.nonce)
          {
            // Nonce missmatch. Transmit correct nonce.
            reg = getRegister(REGI_SECUNONCE);
            reg->sendSwapStatus();
            break;
          }
        }
        // Filter incorrect data lengths
        if (swPacket.value.length == reg->length)
          reg->setData(swPacket.value.data);
        else
          reg->sendSwapStatus();
        break;
      case SWAPFUNCT_QRY:
        // Only Product Code can be broadcasted
        if (swPacket.destAddr == SWAP_BCAST_ADDR)
        {
          if (swPacket.regId != REGI_PRODUCTCODE)
            break;
        }
        // Query not addressed to us?
        else if (swPacket.destAddr != swap.devAddress)
          break;
        // Current version does not support data recording mode
        // so destination address and register address must be the same
        if (swPacket.destAddr != swPacket.regAddr)
          break;
        // Valid register?
        if ((reg = getRegister(swPacket.regId)) == NULL)
          break;
        reg->getData();
        break;
      case SWAPFUNCT_STA:
        // User callback function declared?
        if (swap.statusReceived != NULL)
          swap.statusReceived(&swPacket);
        break;
      default:
        break;
    }
  }
}

/**
 * SWAP
 *
 * Class constructor
 * 
 * @param regTbl  Pointer to table of SWAP registers
 * @param numRegs  Number of registers
 */
SWAP::SWAP(REGISTER** regTbl, uint8_t numRegs)
{
  swap.regTable = regTbl;
  swap.regTableSize = numRegs;

  //panstamp.setPacketRxCallback(pacKetReceived);
  panstamp.attachInterrupt(pacKetReceived);
  statusReceived = NULL;
  repeater = NULL;
  encryptPwd = NULL;
  security = 0;
}

/**
 * enableRepeater
 *
 * Enable repeater mode
 *
 * @param maxHop Maximum repeater count. Zero if omitted
 */
void SWAP::enableRepeater(unsigned char maxHop)
{
  if (repeater == NULL)
  {
    static REPEATER repe;
    repeater = &repe;
    repeater->init(maxHop);
  }

  if (maxHop == 0)
    repeater->stop();
}

/**
 * getRegister
 *
 * Return pointer to register with ID = regId
 *
 * @param regId Register ID
 */
REGISTER * getRegister(unsigned char regId)
{
  if (regId >= swap.regTableSize)
    return NULL;

  return swap.regTable[regId]; 
}

/**
 * enterSystemState
 *
 * Enter system state
 *
 * @param state New system state
 */
void SWAP::enterSystemState(SYSTATE state)
{
  // Enter SYNC mode (full Rx mode)
  unsigned char newState[] = {state};
  regTable[REGI_SYSSTATE]->setData(newState);
}

/**
 * setSmartPassword
 * 
 * Set Smart Encryption password
 * 
 * @param password Encryption password
 */
void SWAP::setSmartPassword(unsigned char* password)
{
  // Save password
  encryptPwd = password;
  // Enable Smart Encryption
  security |= 0x02;
}

/**
 * goToSleep
 *
 * put the MCU in sleep mode during txInterval seconds
 */
void SWAP::goToSleep(void)
{
  systemState = SYSTATE_RXOFF;
  panstamp.sleepSec(txInterval);
  systemState = SYSTATE_RXON;
}
