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
 * Creation date: 07/03/2013
 */

#ifndef _AVRRTC_H
#define _AVRRTC_H

/**
 * RTC clock sources
 */
typedef enum RTCSRC
{
  RTCSRC_WDT = 0,
  RTCSRC_XT2
} RTCSRC;

/**
 * RTC definitions
 */
#define RTC_250MS    0x03   // Timer 2 prescaler = 32
#define RTC_500MS    0x04   // Timer 2 prescaler = 64
#define RTC_1S       0x05   // Timer 2 prescaler = 128
#define RTC_2S       0x06   // Timer 2 prescaler = 256
#define RTC_8S       0x07   // Timer 2 prescaler = 1024

/**
 * Class: RTC
 * 
 * Description:
 * RTC timer
 */
class AVRRTC
{
  public:
    /**
     * Event counter. This variable gets incremented whenever a timer 2
     * interrupt occurs
     */
    static unsigned long count;
    
    /**
     * setup_watchdog
     * 
     * 'time'	Watchdog timer value
     */
    void setup_watchdog(uint8_t time);

    /**
     * setup_rtc
     *
     * Setup software (Timer 2) RTC
     *
     * 'time'   Timer2 prescaler
     *
     *          RTC_1S = 128 for 1 sec
     *          RTC_2S = 256 for 2 sec
     *          RTC_8S = 1024 for 8 sec
     */
    void setup_rtc(uint8_t time);

    /**
     * sleepWd
     * 
     * Put panStamp into Power-down state during "time".
     * This function uses the internal watchdog timer in order to exit (interrupt)
     * from the power-down state
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
    void sleepWd(uint8_t time); 

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
    void sleepRtc(uint8_t time);

    /**
     * wakeUp
     *
     * Wake from sleep mode
     *
     * 'rxOn' Enter RX_ON state after waking up
     */
    void wakeUp(void);
};

#endif

