/*  MiniWebSvr - A small webserver
    Copyright (c) 2009 Daniel John Walker and Nickolas Antonie Grigoriadis
    E-Mail: djwalker at users.sourceforge.net and grigi_ at users.sourceforge.net

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
#include <stdio.h>
#include <stdlib.h>

#include "url.h"

int urldecode(const char *source, char *dest, int dest_size)
{
	int i;
	int di = 0;
	int tmp1,tmp2;

	--dest_size;  // To allow for dest_size-1 chars and a NULL delimiter

	for (i = 0; source[i] && source[i] != '?' && source[i] != ' ' ; ++i)
	{
		if (di >= dest_size)
		{
			if (di == dest_size)
				dest[di] = 0;
			return 1;
		}
		if (source[i] == '%' && source[i+1] && source[i+2])
		{
			tmp1=source[i+1];
			if ((tmp1-'a') >= 0)
				tmp1-='a'-10;
			else if ((tmp1-'A') >= 0)
				tmp1-='A'-10;
			else if ((tmp1-'0') >= 0)
				tmp1-='0';
			else
				tmp1 = 16;

			tmp2=source[i+2];
			if ((tmp2-'a') >= 0)
				tmp2-='a'-10;
			else if ((tmp2-'A') >= 0)
				tmp2-='A'-10;
			else if ((tmp2-'0') >= 0)
				tmp2-='0';
			else
				tmp2 = 16;

			if ((tmp1<16) && (tmp2<16))
			{
				dest[di]=(char)((tmp1<<4)+tmp2);
				i+=2;
			}
			else
				dest[di] = '%';
			di += 1;
		}
		else
		{
			dest[di] = source[i];
			di += 1;
		}
	}
	dest[di] = 0;
	return 1;
}
