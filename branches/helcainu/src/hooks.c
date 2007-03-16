/*  MiniWebsvr - A small webserver
    Copyright (C) 2007  Daniel John Walker and Nickolas Antonie Grigoriadis
    E-Mail: djwalker at users.sourceforge.net and grigi_ at users.sourceforge.net

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
#include "os_compat.h"

#include <sys/types.h>

#include "server.h"
#include "config.h"
#include "logging.h"

extern server_config config;

int check_hook(char* filename, struct server_struct* inst)
{
	char *ext;
	hook_list* list;
	int len, i;

	DebugMSG("Checking hook!");

	if(config.filehooks)
	{
		ext = strrchr(filename, '.');
		if(ext)
		{
			len = strlen(ext);
			++ext;
			--len;

			ext[len]=0;

			DebugMSG(ext);

			list = config.filehooks;	
			while(list)
			{
				if((strcmp(list->fileext, ext)) == 0)
				{
					DebugMSG("Calling a hook");
					(list->hook)((void*)inst, filename, NULL, 0);
					return 1;
				}
				list = list->next;
			}
		}
	}
	return 0;
}



