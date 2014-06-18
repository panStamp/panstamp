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

#ifndef _PANSTAMP_H
#define _PANSTAMP_H

#include "Arduino.h"
#include "datatypes.h"
#include "EEPROM.h"
#include "cc1101.h"
#include "nvolat.h"
#include "register.h"
#include "swpacket.h"
#include "config.h"
#include "repeater.h"
#include "avrrtc.h"
/*
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>
*/
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
#define setSwapStatusCallBack(ptrFunc)   statusReceived = ptrFunc

#define eepromToFactoryDefaults()                             \
  EEPROM.write(EEPROM_SYNC_WORD, CC1101_DEFVAL_SYNC1);        \
  EEPROM.write(EEPROM_SYNC_WORD + 1, CC1101_DEFVAL_SYNC0);    \
  EEPROM.write(EEPROM_DEVICE_ADDR, CC1101_DEFVAL_ADDR);       \
  EEPROM.write(EEPROM_FREQ_CHANNEL, CC1101_DEFVAL_CHANNR);    \
  EEPROM.write(EEPROM_TX_INTERVAL, 0xFF);                     \
  EEPROM.write(EEPROM_TX_INTERVAL + 1, 0xFF)

#define setHighTxPower()    cc1101.setTxPowerAmp(PA_LongDistance)
#define setLowTxPower()     cc1101.setTxPowerAmp(PA_LowPower)

#define enableAntiPlayback()    security |= 0x01

#define disableRfRx()       enableRfRx(false)

#ifndef SWAP_EXTENDED_ADDRESS
#define swapAddress         cc1101.devAddress
#endif


/**
 * System states
 */
enum SYSTATE
{
  SYSTATE_RESTART = 0,
  SYSTATE_RXON,
  SYSTATE_RXOFF,
  SYSTATE_SYNC,
  SYSTATE_LOWBAT,
  SYSTATE_UPGRADE
};


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
     * RTC timer object (supports WDT and Timer2)
     */
    AVRRTC rtc;

    /**
     * repeater
     *
     * Pointer to repeater object
     */
    REPEATER *repeater;

    /**
     * True if the external 32.768 KHz crystal is enabled
     */
    bool rtcCrystal;

    /**
     * CC1101 radio interface
     */
    CC1101 cc1101;
    
    #ifdef SWAP_EXTENDED_ADDRESS
    /**
     * Device address
     */
    SWADDR swapAddress;
    #endif

    /**
     * Security options
     */
    byte security;

    /**
     * Security cyclic nonce
     */
    byte nonce;
    
    /**
     * System state
     */
    byte systemState;

    /**
     * Interval between periodic transmissions. 0 for asynchronous transmissions
     */
    unsigned int txInterval;

    /**
     * Smart encryption password
     */
    byte encryptPwd[12];

    /**
     * enableRepeater
     *
     * Enable repeater mode
     */
    void enableRepeater(void);

    /**
     * enableRepeater
     *
     * Enable repeater mode
     *
     * 'maxHop'  MAximum repeater count. Zero if omitted
     */
    void enableRepeater(byte maxHop=0);

    /**
     * SWAP status packet received. Callaback function
     */
    void (*statusReceived)(SWPACKET *status);

    /**
     * PANSTAMP
     *
     * Class constructor
     */
    PANSTAMP(void);

    /**
     * init
     * 
     * Initialize panStamp board
     * 
     * 'freq'	New carrier frequency
     */
    void init(byte freq=CFREQ_868);

    /**
     * reset
     * 
     * Reset panStamp
     */
    void reset(void);

    /**
     * sleepWd
     * 
     * Put panStamp into Power-down state during "time".
     * This function uses the internal watchdog timer in order to exit (interrupt)
     * from the power-doen state
     * 
     * 'time'	Sleeping time:
     *  WDTO_15MS  = 15 ms
     *  WDTO_30MS  = 30 ms
     *  WDTO_60MS  = 60 ms
     *  WDTO_120MS  = 120 ms
     *  WDTO_250MS  = 250 ms
     *  WDTO_500MS  = 500 ms
     *  WDTO_1S = 1 s
     *  WDTO_2S = 2 s
     *  WDTO_4S = 4 s
     *  WDTO_8S = 8 s
     */
    void sleepWd(byte time);

    /**
     * sleepRtc
     * 
     * Put panStamp into Power-down state during "time".
     * This function uses Timer 2 connected to an external 32.768KHz crystal
     * in order to exit (interrupt) from the power-down state
     * 
     * 'time'	Sleeping time:
     *  RTC_250MS  = 250 ms
     *  RTC_500MS  = 500 ms
     *  RTC_1S = 1 s
     *  RTC_2S = 2 s
     *  RTC_8S = 8 s
     */
    void sleepRtc(byte time);

    /**
     * wakeUp
     *
     * Wake from sleep mode
     */
    void wakeUp(void);

    /**
     * goToSleep
     *
     * Sleep whilst in power-down mode. This function currently uses sleepWd in a loop
     *
     */
    void goToSleep(void);

    /**
     * enterSystemState
     *
     * Enter system state
     *
     * 'state'  New system state
     */
    void enterSystemState(SYSTATE state);

    /**
     * getInternalTemp
     * 
     * Read internal (ATMEGA328 only) temperature sensor
     * Reference: http://playground.arduino.cc/Main/InternalTemperatureSensor
     * 
     * Return:
     * 	Temperature in degrees Celsius
     */
    long getInternalTemp(void);

    /**
     * setSmartPassword
     * 
     * Set Smart Encryption password
     * 
     * 'password'	Encryption password
     */
    void setSmartPassword(byte* password);

    /**
     * enableRfRx
     * 
     * Enable or disable RF reception
     *
     * @param ena Enable if true. Disable otherwise
     */
    void enableRfRx(bool ena = true);
};

/**
 * Global PANSTAMP object
 */
extern PANSTAMP panstamp;

/**
 * getRegister
 *
 * Return pointer to register with ID = regId
 *
 * 'regId'  Register ID
 */
REGISTER * getRegister(byte regId);

#endif

