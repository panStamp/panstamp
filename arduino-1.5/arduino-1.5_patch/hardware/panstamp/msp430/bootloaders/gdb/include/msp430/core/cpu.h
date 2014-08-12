/*
 * cpu.h - abstract CPU clock initialization, timing and sleeping
 *
 * Created: Nov-12-2012
 *  Author: rick@kimballsoftware.com
 *    Date: 03-02-2013
 * Version: 1.0.2
 *
 * =========================================================================
 *  Copyright © 2013 Rick Kimball
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef CPU_H_
#define CPU_H_
#include <msp430.h>

/*
 * cpu_base_t - provide CPU related functions
 *
 * uint32_t FREQ - cpu frequency in Hz
 */
template<uint32_t MCLK_FREQ>
struct cpu_base_t {
    static const uint32_t frequency = MCLK_FREQ;
    static const uint32_t msec_cycles = frequency/1000;
    static const uint32_t usec_cycles = frequency/1000000;
};

#if defined(__MSP430_HAS_BC2__)
#include "drivers/cpu430_bc2.h"
#elif defined(__MSP430_HAS_CS__)
#include "drivers/cpu430_cs.h"
#elif defined(__MSP430_HAS_UCS_RF__)
#include "drivers/cpu430_ucs_rf.h"
#else
#error Unsupport CPU detected!
#endif

#define delay_msecs(msec) __delay_cycles(CPU::msec_cycles * msec)
#define delay_usecs(usec) __delay_cycles(CPU::usec_cycles * usec)

// arduino style
#define delay(msec) __delay_cycles(CPU::msec_cycles * msec)
#define delayMicroseconds(usec) __delay_cycles(CPU::usec_cycles * usec)

#endif /* CPU_H_ */
