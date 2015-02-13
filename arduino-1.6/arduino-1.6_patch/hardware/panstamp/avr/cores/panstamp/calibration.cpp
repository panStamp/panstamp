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
 * Creation date: 11/13/2012
 */

#include "calibration.h"
#include "Arduino.h"

/**
 * rcOscCalibrate
 * 
 * Calibrate internal RC oscillator
 *
 * Return:
 *   true  : calibration successfully done
 *   false : unable to complete calibration
 */
bool rcOscCalibrate(void)
{
  uint8_t loopCount = (0x7F / 2);
  uint32_t checkCount = 100;

  // Save current OSCCAL value
  uint8_t oldOsccal = OSCCAL;

  // Save Timer 1 settings
  uint8_t oldTCCR1A = TCCR1A;
  uint8_t oldTCCR1B = TCCR1B;

  // Inital OSCCAL of half its maximum
  OSCCAL = (0x7F / 2);

  // Disable timer interrupts
  TIMSK1 = 0;
  TIMSK2 = 0;

  // Set timer 2 to asyncronous mode (32.768KHz crystal)
  ASSR = (1 << AS2);

  // Ensure timer 1 control register A is cleared
  TCCR1A = 0;

  // Timer 2 normal operation
  TCCR2A = 0x00;

  // Start Timer 2 with no prescaling
  TCCR2B = (1 << CS20);

  // Start Timer 1 with no prescaling
  TCCR1B = (1 << CS10);
  TIFR1 |= (1 << TOV1);          // Clear timer 1 overflow flag   
  TCNT1 = 0;                     // Reset Timer 1 count

  // Enter calibration loop
  while (loopCount--)
  {  
    TIFR2 |= (1 << TOV2);          // Clear timer 2 overflow flag   
    TCNT2 = 0;                     // Reset Timer 2 count
    
    // Wait for the registers to be updated for Timer 2
    while (ASSR & (_BV(TCN2UB) | _BV(TCR2AUB) | _BV(TCR2BUB)))
    {
      // Use timer 1 to detect presence or lack of external crystal
      if (TIFR1 & (1 << TOV1))
      {
        TIFR1 |= (1 << TOV1);        // Clear timer 1 overflow flag   
        TCNT1 = 0;                   // Reset Timer 1 count
        if (--checkCount == 0)
        {
          ASSR = 0;                  // Disable Timer 2
          OSCCAL = oldOsccal;        // Set factory default OSCCAL
          return false;              // 32.768 KHz crystal is not present
        }
      }
    }

    TCCR1B = (1 << CS10);          // Start Timer 1 with no prescaling
    TCNT1 = 0;                     // Reset Timer 1 count
    
    // Wait until timer 2 overflows
    while (!(TIFR2 & (1 << TOV2)));

    // Stop timer 1 so it can be read
    TCCR1B = 0x00;

    // Check timer value against ideal constant
    if (TCNT1 > TARGETCOUNT_MAX)       // Clock is running too fast
      OSCCAL--;
    else if (TCNT1 < TARGETCOUNT_MIN)  // Clock is running too slow
      OSCCAL++;
    else                               // Clock is OK
      break;
  }

  ASSR = 0;                        // Disable Timer 2
  TIFR2 |= (1 << TOV2);            // Clear timer 2 overflow flag   
  TCNT2 = 0;                       // Reset Timer 2 count

  //Revert to original Timer 1 settings
  TCCR1A = oldTCCR1A;
  TCCR1B = oldTCCR1B;

  /*
  // Read OSCCAL from EEPROM
  uint8_t val = EEPROM.read(0x3FF);
  if (val != OSCCAL)
  {
    // Store OSCCAL in the last location in EEPROM
    EEPROM.write(0x3FF, OSCCAL);
  }
  */

  return true;
}

