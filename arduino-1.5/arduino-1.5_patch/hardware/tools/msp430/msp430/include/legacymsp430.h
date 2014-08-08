/* Syntactic sugar for certain operations
 *
 * Copyright (c) 2001,2002 Dmitry Dicky diwil@eis.ru
 *                         Chris Liechti cliechti@users.sourceforge.net
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS `AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#if !defined(__LEGACYMSP430_H_)
#define __LEGACYMSP430_H_

#include <iomacros.h>
#include <msp430.h>
#define INTERRUPT_VECTOR_SLOTS (1+(RESET_VECTOR / 2))

#if !defined(__ASSEMBLER__)

#ifndef BV
#define BV(x) (1 << (x))
#endif

#define Interrupt(x) void __attribute__((interrupt (x)))
#define INTERRUPT(x) void __attribute__((interrupt (x)))
#define interrupt(x) void __attribute__((interrupt (x)))

#define wakeup  __attribute__((wakeup))
#define Wakeup  __attribute__((wakeup))
#define WAKEUP  __attribute__((wakeup))

#define enablenested    __attribute__((signal))
#define EnableNested    __attribute__((signal))
#define ENABLENESTED    __attribute__((signal))

#include <in430.h>

/* Enable/Disable interrupts */
#define eint()  __eint()
#define dint()  __dint()

/* IAR compatibility functions */
#define _EINT()	                eint()
#define _DINT()                 dint()

#define INTERRUPT_VECTORS \
	void *InterruptVectors[INTERRUPT_VECTOR_SLOTS] __attribute__ ((section(".vectors")))

/* Declare interrupt service routine with no interrupt vector assigned.
 *  NB: Preferred way is to use __attribute__ ((interrupt)). */
#define NOVECTOR	    0xFF

#define _RESET() void __attribute__ ((naked)) _reset_vector__(void)

#define UNEXPECTED() interrupt (NOVECTOR) _unexpected_(void)

#else   /*__ASSEMBLER__ / assember definitions*/

/* Double macro trick to achieve that the x gets expanded*/
#define interrupt(x) xinterrupt(x)
#define xinterrupt(x) vector_##x: .global vector_##x

#endif /* __ASSEMBLER__ */

#endif /* __LEGACYMSP430_H_ */
