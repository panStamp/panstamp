/* Copyright (c) 2011, Peter A. Bigot
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in
 *   the documentation and/or other materials provided with the
 *   distribution.
 * - Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _MSP430_LIBC_H_
#define _MSP430_LIBC_H_

/** Date of this release of msp430-libc */
#define __MSP430_LIBC__ 20120224

/* Do not attempt to control int32 support by defining this macro.  It
 * doesn't work that way. */
#undef __MSP430LIBC_PRINTF_INT32__

/** Defined to non-zero iff printf(3c) supports 32-bit integers */
#define __MSP430LIBC_PRINTF_INT32__ 1

/* Do not attempt to control int64 support by defining this macro.  It
 * doesn't work that way. */
#undef __MSP430LIBC_PRINTF_INT64__

/** Defined to non-zero iff printf(3c) supports 64-bit integers */
#define __MSP430LIBC_PRINTF_INT64__ 1

/** Defined to non-zero iff mathsf functions should conform to IEEE754
 * error handling.  Exceptions:
 *
 * + NaNs are not signaled.  If a NaN is passed into the multiply
 *   function the same NaN will be returned.
 *
 * + In some cases denormalized results are rounded up twice,
 *   resulting in a number 1 bit higher than defined by the standards.
 */
#define __MSP430LIBC_IEEE754_ERRORS__ 0

#endif /* _MSP430_LIBC_H_ */
