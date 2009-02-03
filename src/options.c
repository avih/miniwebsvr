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
#include "os_compat.h"

#include <stdio.h>
#include <stdlib.h>
//#include <dirent.h>
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
