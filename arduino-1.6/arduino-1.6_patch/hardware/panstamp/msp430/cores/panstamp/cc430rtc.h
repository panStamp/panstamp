/**
 * Copyright (c) 2015 panStamp <contact@panstamp.com>
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
 * Creation date: 06/21/2013
 */

#ifndef _CC430RTC_H
#define _CC430RTC_H

#include "wiring.h"

struct RTCDATA
{
  public:
    /**
     * Current year
     */
    uint16_t year;

    /**
     * Current month (1 to 12)
     */
    uint8_t mon;

    /**
     * Current month day (1 to 31)
     */
    uint8_t day;
    
    /**
     * Current week day (0 to 6)
     */
    uint8_t wday;
    
    /**
     * Current hour (0 to 23)
     */
    uint8_t hour;
    
    /**
     * Current minute (0 to 59)
     */
    uint8_t min;
    
    /**
     * Current second (0 to 59)
     */
    uint8_t sec;
};

/**
 * Macros
 */
#define RTCA_STOP()		                     (RTCCTL01 |=  RTCHOLD)
#define RTCA_START()		                   (RTCCTL01 &= ~RTCHOLD)

#define RTC_SET_YEAR(year)                 (RTCYEAR = year)
#define RTC_SET_MONTH(month)               (RTCMON = month)  // 1 to 12
#define RTC_SET_DAY(day)                   (RTCDAY = day)    // 1 to 31
#define RTC_SET_DOW(dow)                   (RTCDOW = dow)    // 0 to 6
#define RTC_SET_HOUR(hour)                 (RTCHOUR = hour)  // 0 to 23
#define RTC_SET_MIN(min)                   (RTCMIN = min)    // 0 to 59
#define RTC_SET_SEC(sec)                   (RTCSEC = sec)    // 0 to 59

#define RTC_SET_DAY_ALARM(day)             (RTCADAY = (day & 0x1F) | 0x80)
#define RTC_SET_DOW_ALARM(dow)             (RTCADOW = (dow & 0x07) | 0x80)
#define RTC_SET_HOUR_ALARM(hour)           (RTCAHOUR = (hour & 0x1F) | 0x80)
#define RTC_SET_MIN_ALARM(min)             (RTCAMIN = (min & 0x3F) | 0x80)

/**
 * RTC clock sources
 */
typedef enum RTCSRC
{
  RTCSRC_VLO = 0,
  RTCSRC_XT1
} RTCSRC;

/**
 * Class: CC430RTC
 * 
 * Description:
 * CC430RTC interface
 */
class CC430RTC
{
  public:
    /**
     * Calendar flag. Set to true when calendar is running
     */
     bool calendarIsRunning;

    /**
     * Class constructor
     */
    CC430RTC(void);

    /**
     * sleep
     * 
     * Put panStamp into Power-down state during "time".
     * This function uses RTC_D connected to an external 32.768KHz crystal
     * in order to exit (interrupt) from the power-down state
     * 
     * @param time Sleeping time in seconds
     * @param ACLK source (RTCSRC_XT1 or RTCSRC_VLO)
     */
    void sleep(uint16_t time, RTCSRC source=RTCSRC_XT1);

    /**
     * disableAlarm
     * 
     * Enable RTC alarm
     */
    inline void disableAlarm(void)
    {
      RTCADOW &= 0x7F;
      RTCADAY &= 0x7F;
      RTCAHOUR &= 0x7F;
      RTCAMIN  &= 0x7F;
      RTCCTL01 &= ~RTCAIE;
    }

    /**
     * startCalendar
     * 
     * Start RTC module in calendar mode
     * 
     * @param rtcData pointer to struct containing date, time and alarm information
     */
    void startCalendar(RTCDATA* rtcData);
    
    /**
     * stopCalendar
     * 
     * Stop RTC module in calendar mode
     */
    inline void stopCalendar(void)
    {
      calendarIsRunning = false;
      RTCA_STOP();
    }
    
    /**
     * setAlarmDayOfMonth
     * 
     * Set RTC alarm - Day of month
     * 
     * @param day Day of month (1 to 31)
     */
    inline void setAlarmDayOfMonth(uint8_t day) 
    {
      RTC_SET_DAY_ALARM(day);
    }
    
    /**
     * setAlarmDayOfWeek
     * 
     * Set RTC alarm - Day of week
     * 
     * @param dow Day of week (0 to 6)
     */
    inline void setAlarmDayOfWeek(uint8_t dow) 
    {
      RTC_SET_DOW_ALARM(dow);
    }
    
    /**
     * setAlarmHour
     * 
     * Set RTC alarm - Hour
     * 
     * @param hour Hour (0 to 23)
     */
    inline void setAlarmHour(uint8_t hour) 
    {
      RTC_SET_HOUR_ALARM(hour);
    }
    
    /**
     * setAlarmMinutes
     * 
     * Set RTC alarm - Minutes
     * 
     * @param min Minutes day (0 to 59)
     */
    inline void setAlarmMinutes(uint8_t min) 
    {
      RTC_SET_MIN_ALARM(min);
    }
};

#endif

