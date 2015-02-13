/*
 * Copyright (c) 2001 Dmitry Dicky diwil@eis.ru
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
 * $Id: setjmp.h,v 1.4 2006/11/15 14:34:57 coppice Exp $
 */

#if !defined(__SETJMP_H_)
#define __SETJMP_H_

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#if defined(__MSP430X__)  &&  defined(__MSP430X_LARGE__)
/* r3 does not have to be saved */
typedef struct
{
	uint32_t __j_pc; /* return address */
	uint32_t __j_sp; /* r1 stack pointer */
	uint32_t __j_sr; /* r2 status register */
	uint32_t __j_r4;
	uint32_t __j_r5;
	uint32_t __j_r6;
	uint32_t __j_r7;
	uint32_t __j_r8;
	uint32_t __j_r9;
	uint32_t __j_r10;
	uint32_t __j_r11;
} jmp_buf[1]; /* size = 20 bytes */
#else
/* r3 does not have to be saved */
typedef struct
{
	uint16_t __j_pc; /* return address */
	uint16_t __j_sp; /* r1 stack pointer */
	uint16_t __j_sr; /* r2 status register */
	uint16_t __j_r4;
	uint16_t __j_r5;
	uint16_t __j_r6;
	uint16_t __j_r7;
	uint16_t __j_r8;
	uint16_t __j_r9;
	uint16_t __j_r10;
	uint16_t __j_r11;
} jmp_buf[1]; /* size = 20 bytes */
#endif

#ifndef __ATTR_NORETURN__
#define __ATTR_NORETURN__ __attribute__((__noreturn__))
#endif

extern int setjmp(jmp_buf __jmpb);
extern void longjmp(jmp_buf __jmpb, int __ret) __ATTR_NORETURN__;

#ifdef __cplusplus
}
#endif //__cplusplus

#endif
