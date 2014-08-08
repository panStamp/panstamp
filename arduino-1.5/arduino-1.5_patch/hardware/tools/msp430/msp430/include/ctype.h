/*
 * Copyright 2001, 2002, 2005
 *     Chris Liechti, Dmitry Diky
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
 * $Id: ctype.h,v 1.7 2006/11/15 14:34:56 coppice Exp $
 */

#if !defined(__CTYPE_H_)
#define __CTYPE_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if !defined(__ATTR_CONST__)
#define __ATTR_CONST__ __attribute__((__const__))
#endif

extern int isalpha(int c) __ATTR_CONST__;
extern int iscntrl(int c) __ATTR_CONST__;
extern int isdigit(int c) __ATTR_CONST__;
extern int isalnum(int c) __ATTR_CONST__;
extern int isspace(int c) __ATTR_CONST__;
extern int isascii(int c) __ATTR_CONST__;
extern int isupper(int c) __ATTR_CONST__;
extern int islower(int c) __ATTR_CONST__;
extern int isprint(int c) __ATTR_CONST__;
extern int isblank(int c) __ATTR_CONST__;
extern int isxdigit(int c) __ATTR_CONST__;
extern int ispunct(int c) __ATTR_CONST__;
extern int toupper(int c) __ATTR_CONST__;
extern int tolower(int c) __ATTR_CONST__;
extern int toascii(int c) __ATTR_CONST__;

#ifdef __GNUC_GNU_INLINE__
extern __inline__ int isalpha(int c)		{ return ((c>='A'&&c<='Z')||(c>='a'&&c<='z')); }

extern __inline__ int iscntrl(int c)		{ return ((c>=0&&c<=27)||c==127);}

extern __inline__ int isdigit(int c)		{ return (((c)>='0')&&((c)<='9'));}

extern __inline__ int isalnum(int c)		{ return (isalpha(c)||isdigit(c));}

extern __inline__ int isspace(int c)		{ return ((c) == ' ' || ((c) >= '\t' && (c) <= '\r'));}

extern __inline__ int isascii(int c)		{ return (((c) & ~0x7f) == 0);}

extern __inline__ int isupper(int c)		{ return ((c) >= 'A' && (c) <= 'Z');}

extern __inline__ int islower(int c)		{ return ((c) >= 'a' && (c) <= 'z');}

extern __inline__ int isprint(int c)		{ return (c >= ' ' && c < 127);}

extern __inline__ int isblank(int c)		{ return (c == '\t' || c == ' ');}

extern __inline__ int isxdigit(int c)     
{
	return (isdigit(c) || ((c) >= 'A' && (c) <= 'F') || ((c) >= 'a' && (c) <= 'f'));
}

extern __inline__ int ispunct(int c)
{
	return (isprint (c) && !islower(c) && !isupper(c) && c != ' ' && !isdigit(c));
}

extern __inline__ int toupper(int c)		{ return ((c) - 0x20 * (((c) >= 'a') && ((c) <= 'z')));}

extern __inline__ int tolower(int c)		{ return ((c) + 0x20 * (((c) >= 'A') && ((c) <= 'Z'))) ;} 

extern __inline__ int toascii(int c)		{ return (c & 0x7f); }
#endif /* __USE_EXTERN_INLINES */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
