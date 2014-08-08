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

#include "panstamp.h"
#include "wiring.h"
#include "cc430rtc.h"
#include "cc430f5137.h"

/**
 * Macros
 */
/**
 * RTC_A TIMER
 */
#define RTC_SET_ACLK_XT1()                 UCSCTL4 = SELA__XT1CLK | SELS__DCOCLKDIV | SELM__DCOCLKDIV
#define RTC_SET_ACLK_VLO()                 UCSCTL4 = SELA__VLOCLK | SELS__DCOCLKDIV | SELM__DCOCLKDIV
#define RTC_RESET_COUNTER()                RTCNT1=0; RTCNT2=0; RTCNT3=0; RTCNT4=0
#define RTC_START_8BIT_COUNTER()           (RTCCTL1 = 0x00)
#define RTC_START_16BIT_COUNTER()          (RTCCTL1 = 0x01)
#define RTC_START_24BIT_COUNTER()          (RTCCTL1 = 0x02)
#define RTC_START_32BIT_COUNTER()          (RTCCTL1 = 0x03)
#define RTC_STOP_COUNTER()                 (RTCCTL1 = 0x40)
#define RTC_SET_TICKS(ticks)               \
  RTCNT4 = (ticks >> 24) & 0xFF;           \
  RTCNT3 = (ticks >> 16) & 0xFF;           \
  RTCNT2 = (ticks >> 8) & 0xFF;            \
  RTCNT1 = ticks & 0xFF;

#define RTC_ISR_ENABLE()                   (RTCCTL0 |= RTCTEVIE)
#define RTC_ACK_ISR()                      (RTCCTL0 &= ~RTCTEVIFG)
// 32.768 KHz cycles for 1 second
#define RTC_32K_CYCLES_1SEC                32768UL
// VLO cycles for 1 second
#define RTC_VLO_CYCLES_1SEC                10000UL


#define RTC_ENABLE_ALARM()                 (RTCCTL0 |= RTCAIE)
#define RTC_SET_CALENDAR_MODE()            (RTCCTL1 |= RTCMODE)

#define RTC_SET_YEAR(year)                 RTCYEARH = year >> 8 ; RTCYEARL = year & 0xFF
#define RTC_SET_MONTH(month)               (RTCMON = month)  // 1 to 12
#define RTC_SET_DAY(day)                   (RTCDAY = day)    // 1 to 31
#define RTC_SET_DOW(dow)                   (RTCDOW = dow)    // 0 to 6
#define RTC_SET_HOUR(hour)                 (RTCHOUR = hour)  // 0 to 23
#define RTC_SET_MIN(min)                   (RTCMIN = min)    // 0 to 59
#define RTC_SET_SEC(sec)                   (RTCSEC = sec)    // 0 to 59

#define RTC_SET_DAY_ALARM(day)             (RTCADAY |= (day & 0x1F))
#define RTC_SET_DOW_ALARM(dow)             (RTCADOW |= (dow & 0x07))
#define RTC_SET_HOUR_ALARM(hour)           (RTCAHOUR |= (hour & 0x1F))
#define RTC_SET_MIN_ALARM(min)             (RTCAMIN |= (min & 0x3F))

#define RTC_ENABLE_DAY_ALARM()             (RTCADAY |= 0x80)
#define RTC_ENABLE_DOW_ALARM()             (RTCADOW |= 0x80)
#define RTC_ENABLE_HOUR_ALARM()            (RTCAHOUR |= 0x80)
#define RTC_ENABLE_MIN_ALARM()             (RTCAMIN |= 0x80)


/**
 * rtcISR
 * 
 * RTC ISR function
 */
__attribute__((interrupt(RTC_VECTOR)))
void rtcISR(void)
{
  RTC_ACK_ISR();
  __bic_SR_register_on_exit(LPM3_bits);  // clears the bits corresponding to LPM3 and exits the low power mode
  RTC_STOP_COUNTER();
  RTC_RESET_COUNTER();
}

/**
 * sleep
 * 
 * Put panStamp into Power-down state during "time".
 * This function uses RTC connected to an external 32.768KHz crystal
 * in order to exit (interrupt) from the power-down state
 * 
 * @param time Sleeping time in seconds
 * @param ACLK source (RTCSRC_XT1 or RTCSRC_VLO)
 */
void CC430RTC::sleep(uint16_t time, RTCSRC source) 
{
  if (time == 0)
    return;

  uint32_t ticks = 0xFFFFFFFF;

  switch(source)
  {
    case RTCSRC_XT1:
      RTC_SET_ACLK_XT1();             // Connect ACLK to 32.768 KHz crystal
      ticks -= (RTC_32K_CYCLES_1SEC * time);
      break;
    case RTCSRC_VLO:
      RTC_SET_ACLK_VLO();             // Connect ACLK to VLO (Internal 10KHz oscillator)
      ticks -= (RTC_VLO_CYCLES_1SEC * time);
      break;
    default:
      break;
  }

  disableWatchDog();                  // Stop WDT

  RTC_SET_TICKS(ticks);               // Initialize 32-bit counter
  RTC_ISR_ENABLE();                   // Enable RTC interrupt
  RTC_START_32BIT_COUNTER();          // Start RTC counter with 32-bit overflow

  panstamp.core.setLowPowerMode();    // Enter low-power mode (LPM3)

  // ZZZZZZZ....

  panstamp.core.setNormalMode();      // Exit low-power mode and enable WDT

  //enableWatchDog();                   // Enable WDT again
}

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
void CC430RTC::sleepUntil(char day, char dow, char hour, char min) 
{
  if ((day < 0) && (dow < 0) && (hour < 0) && (min < 0))
    return;

  RTC_SET_ACLK_XT1();                 // Connect ACLK to 32.768 KHz crystal

  disableWatchDog();                  // Stop WDT

  if (day > 0)
  {
    RTC_SET_DAY_ALARM(day);
    RTC_ENABLE_DAY_ALARM();
  }
  else if (dow > 0)
  {
    RTC_SET_DOW_ALARM(dow);
    RTC_ENABLE_DOW_ALARM();
  }

  if (hour > 0)
  {
    RTC_SET_HOUR_ALARM(hour);
    RTC_ENABLE_HOUR_ALARM();
  }

  if (min > 0)
  {
    RTC_SET_MIN_ALARM(min);
    RTC_ENABLE_MIN_ALARM();
  }

  RTC_SET_CALENDAR_MODE();            // RTC set to calendar mode
  RTC_ENABLE_ALARM();                 // Enable RTC alarm
  RTC_ISR_ENABLE();                   // Enable RTC interrupt

  __bis_SR_register(LPM3_bits + GIE); // Enter LPM3 with interrupts

  enableWatchDog();                   // Enable WDT again
}

/**
 * setTime
 *
 * Set current date/time
 *
 * @param year Year (1 to 4095)
 * @param mon Month (1 to 12)
 * @param day Day of month (1 to 31)
 * @param dow Day of week (0 to 6)
 * @param hour Hour (0 to 23)
 * @param min Minutes day (0 to 59)
 * @param sec Seconds (0 to 59)
 */
