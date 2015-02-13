/*
 * Copyright (c) 2002 Dmitry Dicky diwil@eis.ru
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
 * $Id$
 */

#ifndef __LIMITS_H
#define __LIMITS_H

#define MB_LEN_MAX	1		/* Longest multi-byte character */
#define CHAR_BIT	8		/* number of bits in a char */
#define CHAR_MAX	127		/* maximum char value */
#define CHAR_MIN	(-128)		/* mimimum char value */
#define SHRT_MAX	32767		/* maximum (signed) short value */
#define SHRT_MIN	(-32768)	/* minimum (signed) short value */

#define __LONG_MAX__ 2147483647L
#undef LONG_MIN
#define LONG_MIN (-LONG_MAX-1)
#undef LONG_MAX
#define LONG_MAX __LONG_MAX__


#define UCHAR_MAX	255		/* maximum unsigned char value */
#define UCHAR_MIN	0		/* minimum unsigned char value */
#define USHRT_MAX	0xffff		/* maximum unsigned short value */
#define USHRT_MIN	0		/* minimum unsigned short value */
#define ULONG_MAX	0xfffffffful	/* maximum unsigned long value */
#define ULONG_MIN	0		/* minimum unsigned long value */

#define SCHAR_MAX	127		/* maximum signed char value */
#define SCHAR_MIN	(-128)		/* minimum signed char value */
#define INT_MAX		32767		/* maximum (signed) int value */
#define INT_MIN		(-32768)	/* minimum (signed) int value */
#define UINT_MAX	0xffff		/* maximum unsigned int value */
#define UINT_MIN	0		/* minimum unsigned int value */

#define ULONG_LONG_MAX	0xffffffffffffffffull

#ifndef RAND_MAX
#define RAND_MAX	INT_MAX
#endif

#endif

