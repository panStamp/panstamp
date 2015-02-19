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
 * pacKetReceived
 *
 * CCPACKET received
 *
 * @param packet CCPACKET received
 */
void pacKetReceived(CCPACKET *packet)
{
  if (packet->length <= SWAP_DATA_HEAD_LEN)
    return;
  
  SWPACKET swPacket = SWPACKET(packet);
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
        if ((reg = swap.getRegister(swPacket.regId)) == NULL)
          break;
        // Anti-playback security enabled?
        if (swap.security & 0x01)
        {
          // Check received nonce
          if (swap.nonce != swPacket.nonce)
          {
            // Nonce missmatch. Transmit correct nonce.
            reg = swap.getRegister(REGI_SECUNONCE);
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
        if ((reg = swap.getRegister(swPacket.regId)) == NULL)
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
 */
SWAP::SWAP(void)
{  
  statusReceived = NULL;
  repeater = NULL;
  encryptPwd = NULL;
  security = 0;
}

/**
 * init
 *
 * Initialize SWAP registers and stack
 */
void SWAP::init(void)
{
  uint8_t i;
  STORAGE nvMem;

  // Read signature from info/eeprom memory
  uint8_t signature[2];
  nvMem.read(signature, DEFAULT_NVOLAT_SECTION, NVOLAT_SIGNATURE, sizeof(signature));

  // Correct signature in non-volatile memory?
  if ((signature[0] != NVOLAT_SIGNATURE_HIGH) || (signature[1] != NVOLAT_SIGNATURE_LOW))
    nvolatToFactoryDefaults(); // Copy default settings in non-volatile memory  

  // Intialize registers
  for(i=0 ; i<regTableSize ; i++)
    regTable[i]->init();

  // Config radio settings
  panstamp.radio.devAddress = devAddress & 0xFF; 
  panstamp.radio.setCCregs();
  
  // Attach RF ISR
  panstamp.attachInterrupt(pacKetReceived);
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

/**
 * nvolatToFactoryDefaults
 * 
 * Write default config values in non-volatile memory
 */
void SWAP::nvolatToFactoryDefaults(void)
{
  STORAGE nvMem;

  // Signature
  uint8_t signature[] = {NVOLAT_SIGNATURE_HIGH, NVOLAT_SIGNATURE_LOW};
  nvMem.write(signature, DEFAULT_NVOLAT_SECTION, NVOLAT_SIGNATURE, sizeof(signature));
  
  // Frequency channel
  uint8_t channel[] = {CCDEF_CHANNR};
  nvMem.write(channel, DEFAULT_NVOLAT_SECTION, NVOLAT_FREQ_CHANNEL, sizeof(channel));
  
  // Sync word
  uint8_t syncW[] = {CCDEF_SYNC1, CCDEF_SYNC0};
  nvMem.write(syncW, DEFAULT_NVOLAT_SECTION, NVOLAT_SYNC_WORD, sizeof(syncW));

  // SWAP address (pseudo-random number)
  uint16_t random = panstamp.GET_RANDOM();
  uint8_t addr[] = {(random >> 8) & 0xFF, random & 0xFF};
  nvMem.write(addr, DEFAULT_NVOLAT_SECTION, NVOLAT_DEVICE_ADDR, sizeof(addr));
  
  // TX interval
  uint8_t txInt[] = {0xFF, 0};
  nvMem.write(txInt, DEFAULT_NVOLAT_SECTION, NVOLAT_TX_INTERVAL, sizeof(txInt));
}

/**
 * Pre-instantiate SWAP object
 */
SWAP swap;
