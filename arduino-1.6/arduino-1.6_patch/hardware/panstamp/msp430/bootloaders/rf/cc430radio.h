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
 * Creation date: 04/03/2014
 */

#ifndef _CC430RADIO_H
#define _CC430RADIO_H

#include "cc430f5137.h"
#include "rf1a.h"
#include "datatypes.h"
#include "ccpacket.h"

/**
 * Carrier frequencies
 */
enum CFREQ
{
  CFREQ_868 = 0,
  CFREQ_915,
  CFREQ_433,
  CFREQ_LAST
};

/**
 * RF STATES
 */
enum RFSTATE
{
  RFSTATE_IDLE = 0,
  RFSTATE_RXON,
  RFSTATE_RXOFF,
  RFSTATE_PWRDWN
};

/**
 * Frequency channels
 */
#define NUMBER_OF_FCHANNELS      10

/**
 * CC430 configuration registers
 */
#define CCDEF_SYNC1      0xB5   // Synchronization word, high byte
#define CCDEF_SYNC0      0x47   // Synchronization word, low byte
#define CCDEF_FSCTRL1    0x08   // Frequency synthesizer control
#define CCDEF_FSCTRL0    0x00   // Frequency synthesizer control
// Carrier frequency = 868 MHz
#define CCDEF_FREQ2_868  0x21   // Frequency control word, high byte.
#define CCDEF_FREQ1_868  0x62   // Frequency control word, middle byte.
#define CCDEF_FREQ0_868  0x76   // Frequency control word, low byte.
// Carrier frequency = 902 MHz
#define CCDEF_FREQ2_915  0x22   // Frequency Control Word, High Byte
#define CCDEF_FREQ1_915  0xB1   // Frequency Control Word, Middle Byte
#define CCDEF_FREQ0_915  0x3B   // Frequency Control Word, Low Byte
// Carrier frequency = 433 MHz
#define CCDEF_FREQ2_433  0x10   // Frequency Control Word, High Byte
#define CCDEF_FREQ1_433  0xA7   // Frequency Control Word, Middle Byte
#define CCDEF_FREQ0_433  0x62   // Frequency Control Word, Low Byte
#define CCDEF_MDMCFG4    0xCA   // Modem configuration.
#define CCDEF_MDMCFG3    0x83   // Modem configuration.
#define CCDEF_MDMCFG2    0x93   // Modem configuration.
#define CCDEF_MDMCFG1    0x22   // Modem configuration.
#define CCDEF_MDMCFG0    0xF8   // Modem configuration.
#define CCDEF_CHANNR     0x00   // Channel number.
#define CCDEF_DEVIATN    0x35   // Modem deviation setting (when FSK modulation is enabled).
#define CCDEF_FREND1     0x56   // Front end RX configuration.
#define CCDEF_FREND0     0x10   // Front end TX configuration.
#define CCDEF_MCSM0      0x18   // Main Radio Control State Machine configuration.
#define CCDEF_MCSM1      0x20   // Main Radio Control State Machine configuration.
#define CCDEF_FOCCFG     0x16   // Frequency Offset Compensation Configuration.
#define CCDEF_BSCFG      0x6C   // Bit synchronization Configuration.
#define CCDEF_AGCCTRL2   0x43   // AGC control.
#define CCDEF_AGCCTRL1   0x40   // AGC control.
#define CCDEF_AGCCTRL0   0x91   // AGC control.
#define CCDEF_FSCAL3     0xE9   // Frequency synthesizer calibration.
#define CCDEF_FSCAL2     0x2A   // Frequency synthesizer calibration.
#define CCDEF_FSCAL1     0x00   // Frequency synthesizer calibration.
#define CCDEF_FSCAL0     0x1F   // Frequency synthesizer calibration.
#define CCDEF_FSTEST     0x59   // Frequency synthesizer calibration.
#define CCDEF_TEST2      0x81   // Various test settings.
#define CCDEF_TEST1      0x35   // Various test settings.
#define CCDEF_TEST0      0x09   // Various test settings.
#define CCDEF_FIFOTHR    0x07   // RXFIFO and TXFIFO thresholds.
#define CCDEF_IOCFG2     0x29   // GDO2 output pin configuration.
#define CCDEF_IOCFG0     0x06   // GDO0 output pin configuration. Refer to SmartRF® Studio User Manual for detailed pseudo register explanation.
#define CCDEF_PKTCTRL1   0x06   // Packet automation control.
#define CCDEF_PKTCTRL0   0x05   // Packet automation control.
#define CCDEF_ADDR       0x77   // Device address.
#define CCDEF_PKTLEN     0x3D   // Packet length.

/**
 * Macros
 */
// RADIO
#define MRFI_ENABLE_SYNC_PIN_INT()                  (RF1AIE |= BIT9)
#define MRFI_DISABLE_SYNC_PIN_INT()                 (RF1AIE &= ~BIT9)
#define MRFI_CLEAR_SYNC_PIN_INT_FLAG()              (RF1AIFG &= ~BIT9)
#define MRFI_SYNC_PIN_INT_FLAG_IS_SET()             (RF1AIFG & BIT9)
#define MRFI_STROBE_IDLE_AND_WAIT()              \
{                                                \
  Strobe(RF_SIDLE);                              \
  /* Wait for XOSC to be stable and radio in IDLE state */ \
  while (Strobe(RF_SNOP) & 0xF0) ;               \
}
#define MRFI_SYNC_PIN_INT_IS_ENABLED()     (RF1AIE & BIT9)

// Disable address check
#define disableAddressCheck()     enableAddressCheck(false)
// Disable CCA
#define disableCCA()              enableCCA(false)
// Set PATABLE single byte
#define setTxPowerAmp(setting)    paTableByte = setting
// PATABLE values
#define PA_LowPower               0x60
#define PA_LongDistance           0xC0
// Enable Rx state
#define setRxState                setRxOnState

/**
 * Class: CC430RADIO
 * 
 * Description:
 * CC430RADIO interface
 */
class CC430RADIO
{
  private:
    /**
     * setCCregs
     * 
     * Configure CC1101 registers
     */
    void setCCregs(void);

  public:
    /**
     * Tx Power byte (single PATABLE config)
     */
    uint8_t paTableByte;

    /**
     * Carrier frequency
     */
    uint8_t carrierFreq;

    /**
     * Frequency channel
     */
    uint8_t channel;

    /**
     * Synchronization word
     */
    uint8_t syncWord[2];

    /**
     * Device address
     */
    uint8_t devAddress;

    /**
     * CC430RADIO
     * 
     * Class constructor
     */
    CC430RADIO(void);

    /**
     * reset
     * 
     * Reset radio core
     */
    ALWAYS_INLINE
    void reset(void) 
    {
      Strobe(RF_SRES);                      // Reset the Radio Core
      Strobe(RF_SNOP);                      // Reset Radio Pointer

      setCCregs();                          // Configure CC1101 interface
      
      MRFI_DISABLE_SYNC_PIN_INT();          // Disable RF interrupts
    }

    /**
     * init
     * 
     * Initialize CC1101
     *
     * @param ch RF channel
     * @param sw Sync word
     * @param addr Device address
     */
    ALWAYS_INLINE
    void init(uint8_t freq=CFREQ_868)
    {
      carrierFreq = freq;
  
      // Reset radio interface
      reset();

      // Set transmission power
      WritePATable(paTableByte);

      // Enter RX state
      setRxOnState();
    }
    
    /**
     * setSyncWord
     * 
     * Set synchronization word
     * 
     * @param syncH	Synchronization word - High byte
     * @param syncL	Synchronization word - Low byte
     */
    ALWAYS_INLINE
    void setSyncWord(uint8_t syncH, uint8_t syncL) 
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
    ALWAYS_INLINE
    void setSyncWord(uint8_t *sync) 
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
    ALWAYS_INLINE
    void setDevAddress(uint8_t addr)
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
    ALWAYS_INLINE
    void setChannel(uint8_t chnl) 
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
    ALWAYS_INLINE
    void setCarrierFreq(uint8_t freq)
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
    ALWAYS_INLINE
    void setRxOnState(void)
    {  
      // Clear Rx interrupts
      MRFI_CLEAR_SYNC_PIN_INT_FLAG();

      // Radio is in IDLE following a TX, so strobe SRX to enter Receive Mode
      Strobe(RF_SRX);

      // Enable Rx interrupts
      MRFI_ENABLE_SYNC_PIN_INT();
    }

    /**
     * setRxOffState
     * 
     * Disable Rx mode
     */
    ALWAYS_INLINE
    void setRxOffState(void)
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
    }

    /**
     * setPowerDownState
     * 
     * Put radio into power-down state
     */
    ALWAYS_INLINE
    void setPowerDownState() 
    {
      /* Chip bug: Radio does not come out of this SLEEP when put to sleep
       * using the SPWD cmd. However, it does wakes up if SXOFF was used to
       * put it to sleep.
       */
      // Comming from RX state, we need to enter the IDLE state first
      Strobe(RF_SIDLE);
      // Enter Power-down state (XTAL off only)
      Strobe(RF_SXOFF);
    }

    /**
     * wakeUp
     * 
     * Wake-up core
     */
    ALWAYS_INLINE
    void wakeUp()
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
     *  @return True if the transmission succeeds. False otherwise
     */
    bool sendData(CCPACKET packet);

    /**
     * receiveData
     * 
     * Read data packet from RX FIFO
     * 
     * @return Amount of bytes received
     */
    uint8_t receiveData(CCPACKET *packet);

    /**
     * enableAddressCheck
     *
     * Turn on/ff address check
     *
     * @param enable True if address check has to be enabled
     */
    ALWAYS_INLINE
    void enableAddressCheck(bool enable)
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
     * @param enable True if address check has to be enabled
     */
    ALWAYS_INLINE
    void enableCCA(bool enable)
    {
      if (enable)
        WriteSingleReg(MCSM1, CCDEF_MCSM1);
      else
        WriteSingleReg(MCSM1, 0);
    }
};

#endif

