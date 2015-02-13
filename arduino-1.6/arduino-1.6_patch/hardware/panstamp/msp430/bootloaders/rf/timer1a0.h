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
 * Creation date: 09/11/2014
 */

#ifndef _TIMER1A0_H
#define _TIMER1A0_H


class TIMER1A0
{ 
  public:
  
  /**
   * start
   *
   * Start T1A3 timer
   *
   * @param millis Milliseconds to be timed. Up to 2000 ms
   */
  ALWAYS_INLINE
  void start(unsigned int millis)
  {
    TA1CCR0 = 32.767 * millis;                // Max count
    TA1CTL = TASSEL_1 + MC_1 + TACLR;         // ACLK, upmode, clear TAR
  }
  
  /**
   * stop
   *
   * Stop T1A3 timer
   */
  ALWAYS_INLINE
  void stop(void)
  {
    TA1CTL = MC_0 + TACLR;                    // Halt timer
  }
  
  /**
   * timeout
   * 
   * Return end of timing
   * 
   * @return true if the timer ended.
   */
  ALWAYS_INLINE
  bool timeout(void)
  {
    return (TA1R == TA1CCR0);
  }
};

#endif
