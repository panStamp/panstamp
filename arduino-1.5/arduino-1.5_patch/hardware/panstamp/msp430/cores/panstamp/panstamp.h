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

#include "cc430core.h"
#include "cc430radio.h"
#include "cc430rtc.h"
#include "wiring.h"

/**
 * Default carrier frequency
 */
#define DEFAULT_CARRIER_FREQ   CFREQ_868

/**
 * Alias
 */
#define cc1101 radio

/**
 * RTC definitions
 */
#define RTC_250MS    0x03   // Timer 2 prescaler = 32
#define RTC_500MS    0x04   // Timer 2 prescaler = 64
#define RTC_1S       0x05   // Timer 2 prescaler = 128
#define RTC_2S       0x06   // Timer 2 prescaler = 256
#define RTC_8S       0x07   // Timer 2 prescaler = 1024

/**
 * Macros
 */
#define setPacketRxCallback(ptrFunc)     ccPacketReceived = ptrFunc

#define setHighTxPower()    radio.setTxPowerAmp(PA_LongDistance)
#define setLowTxPower()     radio.setTxPowerAmp(PA_LowPower)

#define wakeUp()  core.setNormalMode();      // Exit low-power mode

#define getVcc()  core.getVcc()
/**
 * Class: PANSTAMP
 * 
 * Description:
 * panStamp main class
 */
class PANSTAMP
{
  public:
    /**
     * Radio interface
     */
    CC430RADIO radio;
  
    /**
     * MCU core
     */
    CC430CORE core;

    /**
     * RTC module
     */
    CC430RTC rtc;

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
     * sleep
     *
     * Enter LPM4
     */
    void sleep(void);

    /**
     * sleepSec
     *
     * put the MCU in sleep mode
     *
     * @param time Sleep time in seconds
     * @param source Source of interruption (RTCSRC_VLO or RTCSRC_XT1)
     */
    void sleepSec(uint16_t time, RTCSRC source=RTCSRC_XT1);
    
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
};

/**
 * Global PANSTAMP object
 */
extern PANSTAMP panstamp;

#endif

