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
 * Creation date: 05/30/2013
 */

#include "cc430radio.h"
#include "cc430x513x.h"

/**
 * Macros
 */
// Enter Tx state
#define setTxState()              Strobe(RF_STX)
// Enter IDLE state
#define setIdleState()            Strobe(RF_SIDLE)
// Flush Rx FIFO
#define flushRxFifo()             Strobe(RF_SFRX)
// Flush Tx FIFO
#define flushTxFifo()             Strobe(RF_SFTX)

/**
 * CC430RADIO
 * 
 * Class constructor
 */
CC430RADIO::CC430RADIO(void)
{
  carrierFreq = CFREQ_868;
  channel = CCDEF_CHANNR;
  syncWord[0] = CCDEF_SYNC1;
  syncWord[1] = CCDEF_SYNC0;
  devAddress = CCDEF_ADDR;
}

/**
 * init
 * 
 * Initialize CC1101
 *
 * @param freq Carrier frequency
 * @param mode Working mode (speed, ...)
 */
void CC430RADIO::init(uint8_t freq, uint8_t mode)
{
  carrierFreq = freq;
  workMode = mode;
  
  // Reset radio interface
  reset();

  // Set transmission power
  setTxPowerAmp(PA_LowPower);

  // Clear interrupt flags
  MRFI_CLEAR_SYNC_PIN_INT_FLAG();
  MRFI_CLEAR_GDO0_INT_FLAG();
  
  // Enter RX state
  setRxOnState();
}

/**
 * reset
 * 
 * Reset radio core
 */
void CC430RADIO::reset(void) 
{
  Strobe(RF_SRES);                      // Reset the Radio Core
  Strobe(RF_SNOP);                      // Reset Radio Pointer

  setCCregs();                          // Configure CC1101 interface
}

/**
 * setCCregs
 * 
 * Configure CC430 radio registers
 */
void CC430RADIO::setCCregs(void) 
{
  WriteSingleReg(IOCFG2, CCDEF_IOCFG2);
  WriteSingleReg(IOCFG0,  CCDEF_IOCFG0);
  WriteSingleReg(FIFOTHR,  CCDEF_FIFOTHR);
  WriteSingleReg(PKTLEN,  CCDEF_PKTLEN);
  WriteSingleReg(PKTCTRL1,  CCDEF_PKTCTRL1);
  WriteSingleReg(PKTCTRL0,  CCDEF_PKTCTRL0);

  // Set default synchronization word
  setSyncWord(syncWord);

  // Set default device address
  setDevAddress(devAddress);

  // Set default frequency channel
  setChannel(channel);

  WriteSingleReg(FSCTRL1,  CCDEF_FSCTRL1);
  WriteSingleReg(FSCTRL0,  CCDEF_FSCTRL0);    

  // Set carrier frequency
  setCarrierFreq(carrierFreq);

  // RF speed
  if (workMode == MODE_LOW_SPEED)
    WriteSingleReg(MDMCFG4,  CCDEF_MDMCFG4_4800);
  else
    WriteSingleReg(MDMCFG4,  CCDEF_MDMCFG4_38400);
    
  WriteSingleReg(MDMCFG3,  CCDEF_MDMCFG3);
  WriteSingleReg(MDMCFG2,  CCDEF_MDMCFG2);
  WriteSingleReg(MDMCFG1,  CCDEF_MDMCFG1);
  WriteSingleReg(MDMCFG0,  CCDEF_MDMCFG0);
  WriteSingleReg(DEVIATN,  CCDEF_DEVIATN);
  WriteSingleReg(MCSM0,  CCDEF_MCSM0);
  WriteSingleReg(FOCCFG,  CCDEF_FOCCFG);
  WriteSingleReg(BSCFG,  CCDEF_BSCFG);
  WriteSingleReg(AGCCTRL2,  CCDEF_AGCCTRL2);
  WriteSingleReg(AGCCTRL1,  CCDEF_AGCCTRL2);
  WriteSingleReg(AGCCTRL0,  CCDEF_AGCCTRL2);
  WriteSingleReg(FREND1,  CCDEF_FREND1);
  WriteSingleReg(FREND0,  CCDEF_FREND0);
  WriteSingleReg(FSCAL3,  CCDEF_FSCAL3);
  WriteSingleReg(FSCAL2,  CCDEF_FSCAL2);
  WriteSingleReg(FSCAL1,  CCDEF_FSCAL1);
  WriteSingleReg(FSCAL0,  CCDEF_FSCAL0);
  WriteSingleReg(FSTEST,  CCDEF_FSTEST);
  WriteSingleReg(TEST1,  CCDEF_TEST1);
  WriteSingleReg(TEST0,  CCDEF_TEST0);
  
  enableCCA();
}

/**
 * setSyncWord
 * 
 * Set synchronization word
 * 
 * @param syncH	Synchronization word - High byte
 * @param syncL	Synchronization word - Low byte
 */
void CC430RADIO::setSyncWord(uint8_t syncH, uint8_t syncL) 
{
  WriteSingleReg(SYNC1, syncH);
  WriteSingleReg(SYNC0, syncL);
  syncWord[0] = syncH;
  syncWord[1] = syncL;
}

/**
 * setSyncWord (overriding method)
 * 
 * Set synchronization word
 * 
 * @param syncH	Synchronization word - pointer to 2-byte
 */
void CC430RADIO::setSyncWord(uint8_t *sync) 
{
  CC430RADIO::setSyncWord(sync[0], sync[1]);
}

/**
 * setDevAddress
 * 
 * Set device address
 * 
 * @param addr Device address
 */
void CC430RADIO::setDevAddress(uint8_t addr)
{
  WriteSingleReg(ADDR, addr);
  devAddress = addr;
}

/**
 * setChannel
 * 
 * Set frequency channel
 * 
 * @param chnl Frequency channel
 */
void CC430RADIO::setChannel(uint8_t chnl) 
{
  WriteSingleReg(CHANNR,  chnl);
  channel = chnl;
}

/**
 * setCarrierFreq
 * 
 * Set carrier frequency
 * 
 * @param freq New carrier frequency
 */
void CC430RADIO::setCarrierFreq(uint8_t freq)
{
  switch(freq)
  {
    case CFREQ_915:
      WriteSingleReg(FREQ2,  CCDEF_FREQ2_915);
      WriteSingleReg(FREQ1,  CCDEF_FREQ1_915);
      WriteSingleReg(FREQ0,  CCDEF_FREQ0_915);
      break;
    case CFREQ_433:
      WriteSingleReg(FREQ2,  CCDEF_FREQ2_433);
      WriteSingleReg(FREQ1,  CCDEF_FREQ1_433);
      WriteSingleReg(FREQ0,  CCDEF_FREQ0_433);
      break;
    default:
      WriteSingleReg(FREQ2,  CCDEF_FREQ2_868);
      WriteSingleReg(FREQ1,  CCDEF_FREQ1_868);
      WriteSingleReg(FREQ0,  CCDEF_FREQ0_868);
      break;
  }
   
  carrierFreq = freq;  
}

/**
 * setRxOnState
 * 
 * Put radio in Rx mode
 */
void CC430RADIO::setRxOnState(void)
{  
  // Clear Rx interrupts
  MRFI_CLEAR_SYNC_PIN_INT_FLAG();

  // Radio is in IDLE following a TX, so strobe SRX to enter Receive Mode
  Strobe(RF_SRX);

  // Enable Rx interrupts
  MRFI_ENABLE_SYNC_PIN_INT();

  rfState = RFSTATE_RXON;
}

/**
 * setRxOffState
 * 
 * Disable Rx mode
 */
void CC430RADIO::setRxOffState(void)
{
  // Disable Rx interrupts
  MRFI_DISABLE_SYNC_PIN_INT();

  // It is possible that ReceiveOff is called while radio is receiving a packet.
  // Therefore, it is necessary to flush the RX FIFO after issuing IDLE strobe 
  // such that the RXFIFO is empty prior to receiving a packet.
  
  // Enter IDLE state
  MRFI_STROBE_IDLE_AND_WAIT()
  
  // Flush Rx FIFO
  Strobe(RF_SFRX);
  
  // Clear Rx interrupts
  MRFI_CLEAR_SYNC_PIN_INT_FLAG();

  rfState = RFSTATE_RXOFF;
}

/**
 * setPowerDownState
 * 
 * Put radio into power-down state
 */
void CC430RADIO::setPowerDownState() 
{
  // Disable RF reception
  setRxOffState();
  
  /* Chip bug: Radio does not come out of this SLEEP when put to sleep
   * using the SPWD cmd. However, it does wakes up if SXOFF was used to
   * put it to sleep.
   */
  // Comming from RX state, we need to enter the IDLE state first
  Strobe(RF_SIDLE);
  // Enter Power-down state (XTAL off only)
  Strobe(RF_SXOFF);

  rfState = RFSTATE_PWRDWN;
}

/**
 * wakeUp
 * 
 * Wake-up core
 */
void CC430RADIO::wakeUp()
{
  __bic_SR_register(LPM3_bits);  // clears the bits corresponding to LPM3 and exits the low power mode
  setRxState();
}

/**
 * sendData
 * 
 * Send data packet via RF
 * 
 * @param packet Packet to be transmitted. First byte is the destination address
 *
 * @return True if the transmission succeeds. False otherwise
 */
bool CC430RADIO::sendData(CCPACKET packet)
{
  bool res = false;
  uint8_t marcState;

  MRFI_CLEAR_SYNC_PIN_INT_FLAG();
  MRFI_CLEAR_GDO0_INT_FLAG();

  // Disable Rx and enter in IDLE state
  setRxOffState();

  // Enter RX state again
  setRxState();

  // Check that the RX state has been entered
  while (((marcState = ReadSingleReg(MARCSTATE)) & 0x1F) != 0x0D)
  {
    if (marcState == 0x11)        // RX_OVERFLOW
      flushRxFifo();              // flush receive queue
  }
 
  delayMicroseconds(500);
  
  // Set data length at the first position of the TX FIFO
  WriteSingleReg(RF_TXFIFOWR,  packet.length);
  // Write data into the TX FIFO
  WriteBurstReg(RF_TXFIFOWR, packet.data, packet.length);

  MRFI_CLEAR_GDO0_INT_FLAG();
  // Transmit
  setTxState();

  // Check that TX state is being entered (state = RXTX_SETTLING)
  marcState = ReadSingleReg(MARCSTATE) & 0x1F;
  if((marcState != 0x13) && (marcState != 0x14) && (marcState != 0x15))
  {
    setIdleState();       // Enter IDLE state
    flushTxFifo();        // Flush Tx FIFO
    setRxState();         // Back to RX state
    rfState = RFSTATE_RXON;
    return false;
  }

  // Wait until packet transmission
  while(!MRFI_GDO0_INT_FLAG_IS_SET());

  // Check that the TX FIFO is empty
  if((ReadSingleReg(TXBYTES) & 0x7F) == 0)
    res = true;

  // Clear interrupt flags
  MRFI_CLEAR_SYNC_PIN_INT_FLAG();
  MRFI_CLEAR_GDO0_INT_FLAG();
  
  // Enter back into RX state
  setRxOnState(); 
  rfState = RFSTATE_RXON;

  return res;
}

/**
 * receiveData
 * 
 * Read data packet from RX FIFO
 *
 * @param packet Container for the packet received
 * 
 * @return Amount of bytes received
 */
uint8_t CC430RADIO::receiveData(CCPACKET *packet)
{
  if (rfState != RFSTATE_RXON)
    return 0;
  uint8_t i;
  uint8_t rxBuffer[CCPACKET_BUFFER_LEN];
  uint8_t rxLength = ReadSingleReg(RXBYTES);

  // Any byte waiting to be read and no overflow?
  if ((rxLength & 0x7F) && !(rxLength & 0x80))
  {
    // If packet is too long
    if (rxLength > CCPACKET_BUFFER_LEN)
      packet->length = 0;   // Discard packet
    else
    {
      // Read data packet
      ReadBurstReg(RF_RXFIFORD, rxBuffer, rxLength);

      packet->length = rxBuffer[0];

      for(i=0 ; i<packet->length; i++)
        packet->data[i] = rxBuffer[i+1];

      // Read RSSI
      packet->rssi = rxBuffer[++i];
      // Read LQI and CRC_OK
      packet->lqi = rxBuffer[++i] & 0x7F;
      packet->crc_ok = rxBuffer[i] >> 7;
    }
  }
  else
    packet->length = 0;

  setIdleState();       // Enter IDLE state
  flushRxFifo();        // Flush Rx FIFO

  // Back to RX state
  setRxState();

  return packet->length;
}

/**
 * enableAddressCheck
 *
 * Turn on/ff address check
 *
 * @param enable True if address check has to be enabled
 */
void CC430RADIO::enableAddressCheck(bool enable)
{
  if (enable)
    WriteSingleReg(PKTCTRL1, 0x06);
  else
    WriteSingleReg(PKTCTRL1, 0x04);
}

/**
 * enableCCA
 *
 * Turn on/ff CCA mechanism
 *
 * @param enable True if CCA has to be enabled
 */
void CC430RADIO::enableCCA(bool enable)
{
  if (enable)
    WriteSingleReg(MCSM1, CCDEF_MCSM1);
  else
    WriteSingleReg(MCSM1, 0);
}

/**
 * setWorState
 *
 * Enter Wake-On-Radio state
 * 
 * @param millis Sleeping interval in milliseconds
 *   This interval has to be greater than 12 ms and not greater than 2000 ms
 */
void CC430RADIO::setWorState(uint16_t millis)
{
  uint32_t interval = millis * 32768;
  interval /= 1000;
  
  if (interval > 0xFFFF)
    interval = 0xFFFF;
  else if (interval < 0x189)
    interval = 0x189;
    
  disableWatchDog();
  
  WriteSingleReg(WOREVT1, ((uint16_t)interval >> 8) & 0xFF);
  WriteSingleReg(WOREVT0, (uint16_t)interval & 0xFF);
  WriteSingleReg(MCSM2, 0x00);  // Timeout for sync word search = 12.5%
  WriteSingleReg(WORCTRL, (7 << 4) | 0); // EVENT1 = 48 clk periods = 1.465 ms
  WriteSingleReg(IOCFG1, 0x29); // GDO1 = RF_RDY

  Strobe(RF_SWOR);
  RF1AIE |= ((BIT6) << 8);      // BIT14 = BIT6 << 8
  __bis_SR_register(LPM3_bits + GIE);

  enableWatchDog();
}
