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

#ifndef _CC430RADIO_H
#define _CC430RADIO_H

#ifdef __cplusplus
 extern "C" {
#endif
#include "rf1a.h"
#ifdef __cplusplus
}
#endif

#include "wiring.h"
#include "ccpacket.h"

/**
 * Carrier frequencies
 */
enum CFREQ
{
  CFREQ_868 = 0,
  CFREQ_915,
  CFREQ_918,
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
 * Working modes
 */
#define MODE_LOW_SPEED  0x01  // RF speed = 4800 bps (default is 38 Kbps)

/**
 * Frequency channels
 */
#define NUMBER_OF_FCHANNELS      10

/**
 * CC430 configuration registers
 */
#define CCDEF_SYNC1      0xB5   // Synchronization word, high byte
#define CCDEF_SYNC0      0x47   // Synchronization word, low byte
#define CCDEF_FSCTRL1    0x08   // Frequency synthesizer control.
#define CCDEF_FSCTRL0    0x00   // Frequency synthesizer control.
// Carrier frequency = 868 MHz
#define CCDEF_FREQ2_868  0x21   // Frequency control word, high byte.
#define CCDEF_FREQ1_868  0x62   // Frequency control word, middle byte.
#define CCDEF_FREQ0_868  0x76   // Frequency control word, low byte.
// Carrier frequency = 902 MHz
#define CCDEF_FREQ2_915  0x22   // Frequency Control Word, High Byte
#define CCDEF_FREQ1_915  0xB1   // Frequency Control Word, Middle Byte
#define CCDEF_FREQ0_915  0x3B   // Frequency Control Word, Low Byte
// Carrier frequency = 918 MHz
#define CCDEF_FREQ2_918  0x23   // Frequency Control Word, High Byte
#define CCDEF_FREQ1_918  0x4E   // Frequency Control Word, Middle Byte
#define CCDEF_FREQ0_918  0xC4   // Frequency Control Word, Low Byte
// Carrier frequency = 433 MHz
#define CCDEF_FREQ2_433  0x10   // Frequency Control Word, High Byte
#define CCDEF_FREQ1_433  0xA7   // Frequency Control Word, Middle Byte
#define CCDEF_FREQ0_433  0x62   // Frequency Control Word, Low Byte
#define CCDEF_MDMCFG4_4800    0xC7   // Modem configuration. Speed = 4800 bps
#define CCDEF_MDMCFG4_38400    0xCA   // Modem configuration. Speed = 38 Kbps
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
#define MRFI_CLEAR_GDO0_INT_FLAG()                  (RF1AIFG &= ~BIT0)
#define MRFI_GDO0_INT_FLAG_IS_SET()                 (RF1AIFG & BIT0)

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
    /*
     * RF state
     */
    uint8_t rfState;

  public:
    /**
     * Carrier frequency
     */
    uint8_t carrierFreq;
    
    /**
     * Working mode (speed, ...)
     */
    uint8_t workMode;

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
     * setCCregs
     * 
     * Configure CC1101 registers
     */
    void setCCregs(void);

    /**
     * reset
     * 
     * Reset CC1101
     */
    void reset(void);

    /**
     * init
     * 
     * Initialize CC1101
     *
     * @param freq Carrier frequency
     * @param mode Working mode (speed, ...)
     */
    void init(uint8_t freq=CFREQ_868, uint8_t mode=0);
    
    /**
     * setRxOnState
     * 
     * Put radio in Rx mode
     */
    void setRxOnState(void);

    /**
     * setRxOffState
     * 
     * Disable Rx mode
     */
    void setRxOffState(void);

    /**
     * setSyncWord
     * 
     * Set synchronization word
     * 
     * @param syncH	Synchronization word - High byte
     * @param syncL	Synchronization word - Low byte
     */
    void setSyncWord(uint8_t syncH, uint8_t syncL);

    /**
     * setSyncWord (overriding method)
     * 
     * Set synchronization word
     * 
     * @param syncH	Synchronization word - pointer to 2-byte array
     */
    void setSyncWord(uint8_t *sync);

    /**
     * setDevAddress
     * 
     * Set device address
     * 
     * @param addr Device address
     */
    void setDevAddress(uint8_t addr);

    /**
     * setCarrierFreq
     * 
     * Set carrier frequency
     * 
     * @param freq New carrier frequency
     */
    void setCarrierFreq(uint8_t freq);
    
    /**
     * setChannel
     * 
     * Set frequency channel
     * 
     * 'chnl'	Frequency channel
     */
    void setChannel(uint8_t chnl);

    /**
     * setPowerDownState
     * 
     * Put CC1101 into power-down state
     */
    void setPowerDownState();

    /**
     * wakeUp
     * 
     * Wake-up core
     */
    void wakeUp();

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
    void enableAddressCheck(bool enable=true);

    /**
     * enableCCA
     *
     * Turn on/ff CCA mechanism
     *
     * @param enable True if address check has to be enabled
     */
    void enableCCA(bool enable=true);
    
    /**
     * setWorState
     *
     * Enter Wake-On-Radio state
     * 
     * @param millis Sleeping interval in milliseconds
     *   This interval has to be greater than 12 ms and not greater than 2000 ms
     */
    void setWorState(uint16_t millis);
    
    /**
     * setTxPowerAmp
     * 
     * Set PATABLE value
     * 
     * @param paLevel amplification value
     */
    inline void setTxPowerAmp(uint8_t paLevel)
    {
      WritePATable(paLevel);
    }
};

#endif
