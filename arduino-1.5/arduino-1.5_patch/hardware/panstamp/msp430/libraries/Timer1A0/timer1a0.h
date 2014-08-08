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
 * Creation date: 02/21/2013
 */

#ifndef _TIMER1A0_H
#define _TIMER1A0_H

#include "Arduino.h"

#ifdef __MSP430_HAS_T1A3__

/**
 * Class: TIMER1
 */
class TIMER1A0
{
  public:
    /**
     * Class constructor
     */
    TIMER1A0(void);

    /**
     * TA1 overflow callback function
     */
    void (*timer1a0overflow)(void);

    /**
     * start
     *
     * Start T1A3 timer
     *
     * @param millis Milliseconds to be timed. Up to 2000 ms
     */
    void start(unsigned int millis);

    /**
     * stop
     *
     * Stop T1A3 timer
     */
    void stop(void);

    /**
     * attachInterrupt
     * 
     * Declare custom ISR, to be called whenever the timer overflows
     * 
     * @param funct pointer to the custom function
     */
     inline void attachInterrupt(void (*funct)(void))
     {
       timer1a0overflow = funct;
     }
};

extern TIMER1A0 timer1a0;

#endif  // __MSP430_HAS_T1A3__
#endif
