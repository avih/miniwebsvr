/*  MiniWebSvr - A small webserver
    Copyright (c) 2009 Nickolas Antonie Grigoriadis
    E-Mail: grigi_ at users.sourceforge.net

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/
#ifndef STRL_H
#define STRL_H

#if __BSD_VISIBLE
	#define STATIN

	#if !defined strnlen
	extern size_t strnlen (const char *s, size_t maxlen);
	#endif
#else

#include "config.h"

#if defined USE_INLINE
	#ifdef STRL_C
	#define STATIN __inline
	#else
	#define STATIN static __inline
	#include "strl.c"
	#endif // STRL_C
#else
	#define STATIN

	#if !defined strlcpy
	extern size_t strlcpy(char *dst, const char *src, size_t siz);
	#endif

	#if !defined strlcat
	extern size_t strlcat(char *dst, const char *src, size_t siz);size_t strlcat (char *dst, char *src, unsigned int size);
	#endif

	#if !defined strnlen
	extern size_t strnlen (const char *s, size_t maxlen);
	#endif

#endif // USE_INLINE

#endif // __BSD_VISIBLE

#endif // STRL_H
