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
 * Creation date: 06/03/2013
 */

#ifndef _PANSTAMP_H
#define _PANSTAMP_H

#include "cc1101.h"
#include "avrrtc.h"
#include "Arduino.h"

/**
 * Default carrier frequency
 */
#define DEFAULT_CARRIER_FREQ   CFREQ_868

/**
 * Alias
 */
#define cc1101 radio

/**
 * Macros
 */
#define setPacketRxCallback(ptrFunc)     ccPacketReceived = ptrFunc

#define setHighTxPower()    radio.setTxPowerAmp(PA_LongDistance)
#define setLowTxPower()     radio.setTxPowerAmp(PA_LowPower)

/**
 * System states
 */
enum PANSTAMP_STATE
{
  RXON = 0,
  RXOFF,
};

/**
 * Class: PANSTAMP
 * 
 * Description:
 * panStamp main class
 */
class PANSTAMP
{
  private:
    /**
     * System state
     */
    uint8_t state;

  public:
    /**
     * CC1101 radio interface
     */
    CC1101 cc1101;
  
    /**
     * RTC timer object (supports WDT and Timer2)
     */
    AVRRTC rtc;
    
    /**
     * CC11XX packet received. Callback function
     */
    void (*ccPacketReceived)(CCPACKET *packet);

    /**
     * PANSTAMP
     *
     * Class constructor
     */
    PANSTAMP(void);

    /**
     * rxOn
     *
     * Enable RF reception
     */
    void rxOn(void);

    /**
     * rxOff
     *
     * Disable RF reception
     */
    void rxOff(void);

    /**
     * init
     * 
     * Initialize panStamp board
     *
     * @param freq Carrier frequency
     */
    void init(uint8_t freq=DEFAULT_CARRIER_FREQ);

    /**
     * reset
     * 
     * Reset panStamp
     */
    void reset(void);

    /**
     * wakeUp
     *
     * Wake from sleep mode
     */
    void wakeUp(void);

    /**
     * sleep
     *
     * Enter sleep mode forever and wake up only in case of INT1 event
     */
    void sleep(void);
    
    /**
     * sleepSec
     *
     * put the MCU in sleep mode
     *
     * @param time Sleep time in seconds
     * @param source Source of interruption (RTCSRC_WDT or RTCSRC_XT2)
     */
    void sleepSec(uint16_t time, RTCSRC source=RTCSRC_XT2);

    /**
     * getVcc
     *
     * Read voltage supply
     *
     * @return voltage in mV
     */
    uint16_t getVcc(void);
    
    /**
     * attachInterrupt
     * 
     * Declare custom ISR, to be called whenever a wireless packet is received
     * 
     * @param funct pointer to the custom function
     */
     inline void attachInterrupt(void (*funct)(CCPACKET*))
     {
       ccPacketReceived = funct;
     }
     
    /**
     * GET_RANDOM
     * 
     * Pseudo-random generator. Use OSCAL.
     * 
     * @return pseudo-random number
     */
     inline uint16_t GET_RANDOM(void)
     {
       return OSCCAL;
     }
     
    /**
     * goToWirelessBoot
     *
     * Start wireless bootloader
     */
     inline void goToWirelessBoot(void)
     {
     }
};

/**
 * Global PANSTAMP object
 */
extern PANSTAMP panstamp;

#endif

