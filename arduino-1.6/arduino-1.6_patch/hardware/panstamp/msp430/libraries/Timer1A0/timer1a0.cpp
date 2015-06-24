/**
 * Copyright (c) 2013 panStamp <contact@panstamp.com>
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
 * Creation date: 02/21/2014
 */

#include "timer1a0.h"
#ifdef __MSP430_HAS_T1A3__

/**
 * TIMER1_A0_ISR
 * 
 * TimerA 1 ISR function
 */
__attribute__((interrupt(TIMER1_A0_VECTOR)))
void TIMER1_A0_ISR(void)
{
  if (timer1a0.timer1a0overflow != NULL)
    timer1a0.timer1a0overflow();            // Call user function
}

/**
 * Class constructor
 */
TIMER1A0::TIMER1A0(void)
{
  timer1a0overflow = NULL;
}

/**
 * start
 *
 * Start T1A3 timer
 *
 * @param millis Milliseconds to be timed. Up to 2000 ms
 */
void TIMER1A0::start(unsigned int millis)
{
  TA1CCTL0 = CCIE;                          // CCR0 interrupt enabled
  TA1CCR0 = 32.767 * millis;                // Max count
  TA1CTL = TASSEL_1 + MC_1 + TACLR;         // ACLK, upmode, clear TAR
}

/**
 * stop
 *
 * Stop T1A3 timer
 */
void TIMER1A0::stop(void)
{
  TA1CTL = MC_0;                            // Halt timer
}

TIMER1A0 timer1a0;
#endif // __MSP430_HAS_T1A3__

