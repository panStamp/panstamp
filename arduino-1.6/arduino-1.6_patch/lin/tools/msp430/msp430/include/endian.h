/* Copyright (c) 2010 People Power Co.
 * All rights reserved.
 *
 * This open source code was developed with funding from People Power Company
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 * - Neither the name of the People Power Corporation nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * PEOPLE POWER CO. OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE
 *
 */
#ifndef _ENDIAN_H_
#define _ENDIAN_H_

#define __LITTLE_ENDIAN 1234
#define __BIG_ENDIAN 4321

/* This is MSP430-libc.  The MSP430 is little-endian.
 * Unconditionally. */
#define __BYTE_ORDER __LITTLE_ENDIAN

#if _BSD_SOURCE

#include <bits/byteswap.h>

#define htobe16(_x) __bswap_16(_x)
#define htole16(_x) (_x)
#define be16toh(_x) __bswap_16(_x)
#define le16toh(_x) (_x)

#define htobe32(_x) __bswap_32(_x)
#define htole32(_x) (_x)
#define be32toh(_x) __bswap_32(_x)
#define le32toh(_x) (_x)

#define htobe64(_x) __bswap_64(_x)
#define htole64(_x) (_x)
#define be64toh(_x) __bswap_64(_x)
#define le64toh(_x) (_x)

#endif /* _BSD_SOURCE */

#endif /* _ENDIAN_H_ */
