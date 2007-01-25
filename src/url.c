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

#include <stdio.h>
#include <stdlib.h>

#include "url.h"

/*int urlencode(const char *source, char *dest, int dest_size)
{
	int i;
	int di = 0;
	for (i = 0; source[i]; i += 1)
	{
		if (((source[i] | ' ')  >= 'a' && (source[i] | ' ')  <= 'z') || (source[i] == '/'))
		{
			if (di + 1 >= dest_size)
			{
				dest[di] = 0;
				return 0;
			}
			dest[di] = source[i];
			di += 1;
		}
		else
		{
			if (di + 3 >= dest_size)
			{
				dest[di] = 0;
				return 0;
			}
			snprintf(dest+di,dest_size-di,"%%%2x", source[i]);
                        dest[dest_size-1]=0; // snprintf does not null-delimit when full
			di += 3;
		}
	}
	dest[di] = 0;
	return 1;
}*/

int urldecode(const char *source, char *dest, int dest_size)
{
	int i;
	int di = 0;
	int tmp1,tmp2;

	--dest_size;  // To allow for dest_size-1 chars and a NULL delimiter

	for (i = 0; source[i] && source[i] != '?' && source[i] != ' ' ; ++i)
	{
		if (source[i] == '%' && di < dest_size-2)
		{
			tmp1=source[i+1];
			if ((tmp1-'a') >= 0) 
				tmp1-='a'+10;
			else if ((tmp1-'A') >= 0) 
				tmp1-='A'+10;
			else if ((tmp1-'0') >= 0) 
				tmp1-='0';
			else 
				tmp1 = -1;

			tmp2=source[i+2];
			if ((tmp2-'a') > 0) 
				tmp2-='a'+10;
			else if ((tmp2-'A') >= 0) 
				tmp2-='A'+10;
			else if ((tmp2-'0') >= 0) 
				tmp2-='0';
			else 
				tmp2 = -1;

			if ((tmp1!=-1) && (tmp2!=-1))
			{
				dest[di]=(char)((tmp1<<4)+tmp2);
				i+=2;
			}
			else 
				dest[di] = '%';
			++di;
		}
		else
		{
			if (di >= dest_size)
			{
				dest[di] = 0;
				return 0;
			}
			dest[di] = source[i];
			di += 1;
		}
	}
	dest[di] = 0;
	return 1;
}
