/*  MiniWebSvr - A small webserver
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
#include "os_compat.h"

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

#include "config.h"
#include "header.h"
#include "options.h"

#include "strl.h"

void OPTIONS(struct server_struct *inst)
{
	char Buffer[SEND_BUFFER_SIZE];
	char GHBuffer[SERVER_BUFFER_SIZE];
	int bufpos;

	snprintf(inst->logbuffer,SERVER_BUFFER_SIZE,"\"%s:%d\"",inet_ntoa(inst->sin_addr),htons(inst->sin_port));
	inst->logbuffer[SERVER_BUFFER_SIZE-1] = 0; // snprintf does not null-delimit when full
	inst->MIMEoverride = "OPTIONS"; // Override MIME type with OPTIONS (for log)
	setHeader_respval(inst,200); // OK
	snprintf(GHBuffer,SERVER_BUFFER_SIZE,"Allow: GET,HEAD\r\nPublic: OPTIONS,GET,HEAD\r\n");
	GHBuffer[SERVER_BUFFER_SIZE-1] = 0; // strnprintf does not null-delimit when full
	setHeader_generic(inst,GHBuffer);
	bufpos=printHeader(inst,1,Buffer,SEND_BUFFER_SIZE);
	send(inst->sock,Buffer,bufpos,SEND_FLAG);
}
