/*  Miniweb - A small webserver
    Copyright (C) 2007  Nickolas Antonie Grigoriadis
    E-Mail: grigi_ at users.sourceforge.net

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef STRL_H
#define STRL_H

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

#endif // STRL_H
