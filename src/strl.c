/* strl(cat|cpy) implementation for systems that do not have it in libc */
/* strl.c - strlcpy/strlcat implementation
 * Time-stamp: <2004-03-14 njk>
 * (C) 2003-2004 Nicholas J. Kain <njk@aerifal.cx>
 */

// To find out about strl(cat/cpy) read:
// http://www.gratisoft.us/todd/papers/strlcpy.html
// Initial implementation of strl(cat/cpy) taken from mplayer project (GPL), need to ask them if it can be used in an LGPL project

#include <string.h>

#if !defined strlcpy
unsigned int strlcpy (char *dest, char *src, unsigned int size)
{
	register unsigned int i;

	for (i=0; size > 0 && src[i] != '\0'; ++i, size--)
		dest[i] = src[i];

	dest[i] = '\0';

	return i;
}
#endif

#if !defined strlcat
unsigned int strlcat (char *dest, char *src, unsigned int size)
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
size_t strnlen (const char *s, size_t maxlen)
{
  size_t len = 0;

  while (s[len] != '\0' && maxlen > 0)
    {
      if (s[++len] == '\0' || --maxlen == 0)
	return len;
      if (s[++len] == '\0' || --maxlen == 0)
	return len;
      if (s[++len] == '\0' || --maxlen == 0)
	return len;
      ++len;
      --maxlen;
    }

  return len;
}
#endif
