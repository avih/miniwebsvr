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

// strlcpy and strlcpy implementation from OpenBSD 
// To find out about strl(cat/cpy) read:
// http://www.gratisoft.us/todd/papers/strlcpy.html

#include <string.h>
#define STRL_C
#include "strl.h"

#if !defined strlcpy
STATIN unsigned int strlcpy (char *dest, char *src, unsigned int size)
{
	register unsigned int i;

	for (i=0; size > 0 && src[i] != '\0'; ++i, size--)
		dest[i] = src[i];

	dest[i] = '\0';

	return i;
}
#endif

#if !defined strlcat
STATIN unsigned int strlcat (char *dest, char *src, unsigned int size)
{
	register char *d = dest, *s = src;

	for (; size > 0 && *d != '\0'; size--, d++);
	for (; size > 0 && *s != '\0'; size--, d++, s++)
		*d = *s;

	*d = '\0';
	return (d - dest) + (s - src);
}
#endif

#if !defined strnlen
STATIN size_t strnlen (const char *s, size_t maxlen)
{
  size_t len = 0;

  if (s[0] == '\0' || maxlen == 0) return 0;

  while (s[++len] != '\0' && --maxlen > 0) {}

  return len;
}
#endif
