/* strl(cat|cpy) implementation for systems that do not have it in libc */
/* strl.c - strlcpy/strlcat implementation
 * Time-stamp: <2004-03-14 njk>
 * (C) 2003-2004 Nicholas J. Kain <njk@aerifal.cx>
 */

// To find out about strl(cat/cpy) read:
// http://www.gratisoft.us/todd/papers/strlcpy.html
// Initial implementation taken from mplayer project (GPL), need to ask them if it can be used in an LGPL project

#if !defined strlcpy
extern unsigned int strlcpy (char *dest, char *src, unsigned int size);
#endif

#if !defined strlcat
extern unsigned int strlcat (char *dest, char *src, unsigned int size);
#endif

#if !defined strnlen
extern size_t strnlen (const char *s, size_t maxlen);
#endif
