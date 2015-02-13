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

#if !defined(__INTTYPES_H_)
#define __INTTYPES_H_

/* Per
 * http://pubs.opengroup.org/onlinepubs/009695399/basedefs/inttypes.h.html,
 * this header should augment <stdint.h>.  msp430-libc does not
 * support the extensions, except for PRI format string macros. */

#include <stdint.h>

#include <msp430libc.h>

#define __PRI8_PREFIX ""
#define __PRI16_PREFIX ""
#if __MSP430LIBC_PRINTF_INT32__
#define __PRI32_PREFIX "l"
#endif /* __MSP430_LIBC_PRINTF_INT32__ */
#if __MSP430LIBC_PRINTF_INT64__
/* intmax_t is always int64_t, but 64-bit printf format support is
 * optional.  Only define the format macros if msp430-libc was
 * configured to support it. */
#define __PRI64_PREFIX "ll"
#define __PRIMAX_PREFIX __PRI64_PREFIX
#endif /* __MSP430_LIBC_PRINTF_INT64__ */

/* TODO: SF 3383737 */
#define __PRIPTR_PREFIX __PRI16_PREFIX

#define PRId8 __PRI8_PREFIX "d"
#define PRIi8 __PRI8_PREFIX "i"
#define PRIu8 __PRI8_PREFIX "u"
#define PRIo8 __PRI8_PREFIX "o"
#define PRIx8 __PRI8_PREFIX "x"
#define PRIX8 __PRI8_PREFIX "X"

#define PRIdLEAST8 __PRI8_PREFIX "d"
#define PRIiLEAST8 __PRI8_PREFIX "i"
#define PRIuLEAST8 __PRI8_PREFIX "u"
#define PRIoLEAST8 __PRI8_PREFIX "o"
#define PRIxLEAST8 __PRI8_PREFIX "x"
#define PRIXLEAST8 __PRI8_PREFIX "X"

#define PRIdFAST8 __PRI8_PREFIX "d"
#define PRIiFAST8 __PRI8_PREFIX "i"
#define PRIuFAST8 __PRI8_PREFIX "u"
#define PRIoFAST8 __PRI8_PREFIX "o"
#define PRIxFAST8 __PRI8_PREFIX "x"
#define PRIXFAST8 __PRI8_PREFIX "X"

#define PRId16 __PRI16_PREFIX "d"
#define PRIi16 __PRI16_PREFIX "i"
#define PRIu16 __PRI16_PREFIX "u"
#define PRIo16 __PRI16_PREFIX "o"
#define PRIx16 __PRI16_PREFIX "x"
#define PRIX16 __PRI16_PREFIX "X"

#define PRIdLEAST16 __PRI16_PREFIX "d"
#define PRIiLEAST16 __PRI16_PREFIX "i"
#define PRIuLEAST16 __PRI16_PREFIX "u"
#define PRIoLEAST16 __PRI16_PREFIX "o"
#define PRIxLEAST16 __PRI16_PREFIX "x"
#define PRIXLEAST16 __PRI16_PREFIX "X"

#define PRIdFAST16 __PRI16_PREFIX "d"
#define PRIiFAST16 __PRI16_PREFIX "i"
#define PRIuFAST16 __PRI16_PREFIX "u"
#define PRIoFAST16 __PRI16_PREFIX "o"
#define PRIxFAST16 __PRI16_PREFIX "x"
#define PRIXFAST16 __PRI16_PREFIX "X"

#ifdef __PRI32_PREFIX

#define PRId32 __PRI32_PREFIX "d"
#define PRIi32 __PRI32_PREFIX "i"
#define PRIu32 __PRI32_PREFIX "u"
#define PRIo32 __PRI32_PREFIX "o"
#define PRIx32 __PRI32_PREFIX "x"
#define PRIX32 __PRI32_PREFIX "X"

#define PRIdLEAST32 __PRI32_PREFIX "d"
#define PRIiLEAST32 __PRI32_PREFIX "i"
#define PRIuLEAST32 __PRI32_PREFIX "u"
#define PRIoLEAST32 __PRI32_PREFIX "o"
#define PRIxLEAST32 __PRI32_PREFIX "x"
#define PRIXLEAST32 __PRI32_PREFIX "X"

#define PRIdFAST32 __PRI32_PREFIX "d"
#define PRIiFAST32 __PRI32_PREFIX "i"
#define PRIuFAST32 __PRI32_PREFIX "u"
#define PRIoFAST32 __PRI32_PREFIX "o"
#define PRIxFAST32 __PRI32_PREFIX "x"
#define PRIXFAST32 __PRI32_PREFIX "X"

#endif /* __PRI32_PREFIX */

#ifdef __PRI64_PREFIX

#define PRId64 __PRI64_PREFIX "d"
#define PRIi64 __PRI64_PREFIX "i"
#define PRIu64 __PRI64_PREFIX "u"
#define PRIo64 __PRI64_PREFIX "o"
#define PRIx64 __PRI64_PREFIX "x"
#define PRIX64 __PRI64_PREFIX "X"

#define PRIdLEAST64 __PRI64_PREFIX "d"
#define PRIiLEAST64 __PRI64_PREFIX "i"
#define PRIuLEAST64 __PRI64_PREFIX "u"
#define PRIoLEAST64 __PRI64_PREFIX "o"
#define PRIxLEAST64 __PRI64_PREFIX "x"
#define PRIXLEAST64 __PRI64_PREFIX "X"

#define PRIdFAST64 __PRI64_PREFIX "d"
#define PRIiFAST64 __PRI64_PREFIX "i"
#define PRIuFAST64 __PRI64_PREFIX "u"
#define PRIoFAST64 __PRI64_PREFIX "o"
#define PRIxFAST64 __PRI64_PREFIX "x"
#define PRIXFAST64 __PRI64_PREFIX "X"

#endif /* __PRI64_PREFIX */

#ifdef __PRIMAX_PREFIX

#define PRIdMAX __PRIMAX_PREFIX "d"
#define PRIiMAX __PRIMAX_PREFIX "i"
#define PRIuMAX __PRIMAX_PREFIX "u"
#define PRIoMAX __PRIMAX_PREFIX "o"
#define PRIxMAX __PRIMAX_PREFIX "x"
#define PRIXMAX __PRIMAX_PREFIX "X"

#endif /* __PRIMAX_PREFIX */

#define PRIdPTR __PRIPTR_PREFIX "d"
#define PRIiPTR __PRIPTR_PREFIX "i"
#define PRIuPTR __PRIPTR_PREFIX "u"
#define PRIoPTR __PRIPTR_PREFIX "o"
#define PRIxPTR __PRIPTR_PREFIX "x"
#define PRIXPTR __PRIPTR_PREFIX "X"

#endif /* __INTTYPES_H_ */
