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
 * Creation date: 06/21/2013
 */

#ifndef _CC430RTC_H
#define _CC430RTC_H

#include "wiring.h"

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
 * CC430RADIO interface
 */
class CC430RTC
{
  public:
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
     * sleepUntil
     * 
     * Put panStamp into Power-down state until the RTC alarm is triggered
     * Here the RTC module is used in calendar mode and hexadecimal format
     * This function uses RTC connected to an external 32.768KHz crystal
     * in order to exit (interrupt) from the power-down state
     * 
     * @param day Day of month (1 to 31)
     * @param dow Day of week (0 to 6)
     * @param hour Hour (0 to 23)
     * @param min Minutes day (0 to 59)
     */
    void sleepUntil(char day=-1, char dow=-1, char hour=-1, char min=-1);
};

#endif

