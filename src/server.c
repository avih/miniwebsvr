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
#include "url.h"

#include "gethead.h"
#include "options.h"
#include "logging.h"

#include "strl.h"

void server_close(struct server_struct *inst)
{
#ifdef __WIN32__
	closesocket(inst->sock);
#else
	close(inst->sock);
#endif
	free(inst);
}

int server_charpos(const struct server_struct *inst, const char chr)
{
	int i;

	for (i=inst->buffer_pos;i<inst->buffer_size;++i)
	{
		if (inst->buffer[i] == chr)
			return i;
	}
	return -1;
}

int server_readln(struct server_struct *inst, char *str, const int strsize)
{
	int strpos;
	int retval;

	// Check if in the buffer
	if ((strpos = server_charpos(inst,'\n')) != -1)
	{
		// Whole string is in buffer
		strpos -= inst->buffer_pos+1;

		// Copy string to given buffer
		if (strpos > strsize)
			strpos=strsize;
		memcpy(str,inst->buffer+inst->buffer_pos,strpos);
		inst->buffer_pos+=strpos+2;
	}
	else
	{
		// Copy remaining data in buffer
		strpos=inst->buffer_size-inst->buffer_pos;
		if (strpos > strsize)
			strpos=strsize;
		memcpy(str,inst->buffer+inst->buffer_pos,strpos);

		// Assemble rest of string out of multiple buffers
		while (strpos < strsize)
		{
			// Get data from stream
			inst->buffer_pos=0;
			inst->buffer_size = recv(inst->sock,inst->buffer,SERVER_BUFFER_SIZE,0);

			if (inst->buffer_size == SOCKET_ERROR)
			{
				#ifdef __WIN32__
				retval = WSAGetLastError();
				#else
				retval = errno;
				#endif

				fprintf(stderr,"recv() failed: error %d\n",retval);
				//server_close(inst);
				return -1;
			}

			retval=server_charpos(inst,'\n');
			if ((retval == -1) && ((strpos+inst->buffer_size) > strsize))
				retval=strsize-strpos; // Limit check exceeded
			if (retval == -1)
			{
				// Consume whole buffer
				inst->buffer_pos=inst->buffer_size;
				memcpy(str+strpos,inst->buffer,inst->buffer_size);
				strpos+=inst->buffer_size;
			}
			else
			{
				// Partial buffer
				if (retval > (int)(strsize - strpos))
					retval = strsize - strpos; // Limit check exceeded
				inst->buffer_pos=retval+1; // +1 to skip the NULL terminator of the string
				--retval; // -1 to stop before the detected endline
				if (retval>0)
					memcpy(str+strpos,inst->buffer,retval);
				strpos+=retval;
				break;
			}

			// Don't expect a full buffer to allow interactive (non-buffering) connections.
			// No data indicates some connection error, so quit.
			if (inst->buffer_size == 0) break;
		}
	}

	// remove \r if present just before the \n
	if (strpos && (str[strpos-1] == '\r'))
		--strpos;

	// Set the NULL terminator for the string
	++strpos;
	if (strpos > strsize)
		strpos=strsize;
	str[strpos-1]=0;
	return strpos-1;
}

#ifdef __WIN32__
DWORD server(struct server_struct *inst)
#else
void* server(struct server_struct *inst)
#endif
{
	char Buffer[SEND_BUFFER_SIZE];
	char filename[FILENAME_SIZE];
	char GHBuffer[SERVER_BUFFER_SIZE];
	int retval;
	int headeronly;
	char *tstr;

	DebugMSG("--==|New Request|==--");

	// Initialize structure
	inst->buffer_pos=0;
	inst->buffer_size=0;

	filename[0]='.';
	filename[1]='/';
	filename[2]=0;
	clearHeader(inst);

	headeronly=1;	// Disable data by default

	// Parse header
	retval = server_readln(inst,GHBuffer,SERVER_BUFFER_SIZE);
	DebugMSG("%s",GHBuffer);
	// check for GET requests
	if ( retval >= 4 && 0 == strncmp(GHBuffer, "GET ", 4))
	{
		urldecode(GHBuffer+4, filename+2, FILENAME_SIZE-2);
		headeronly=0;
	}
	else if ( retval >= 5 && 0 == strncmp(GHBuffer, "HEAD ", 5))
	{
		urldecode(GHBuffer+5, filename+2, FILENAME_SIZE-2);
		//headeronly=1;
	}
	else if ( retval >= 8 && 0 == strncmp(GHBuffer, "OPTIONS ", 8))
	{
		OPTIONS(inst);
		goto serverquit;
	}
	else
	{
		// Unknown request type, also print request to logfile
		snprintf(inst->logbuffer,SERVER_BUFFER_SIZE,"\"%s:%d\"",inet_ntoa(inst->sin_addr),htons(inst->sin_port));
		inst->logbuffer[SERVER_BUFFER_SIZE-1] = 0; // snprintf does not null-delimit when full
		inst->MIMEoverride = GHBuffer; // Override MIME type with unknown request type
		setHeader_respval(inst,501); // Not Implemented
		printHeader(inst,headeronly,Buffer,SEND_BUFFER_SIZE); // No need to read return value as it will flush the buffer

		goto serverquit;
	}

	if (filename[2] == 0)
	{
		snprintf(inst->logbuffer,SERVER_BUFFER_SIZE,"\"%s:%d\"",inet_ntoa(inst->sin_addr),htons(inst->sin_port));
		inst->logbuffer[SERVER_BUFFER_SIZE-1] = 0; // snprintf does not null-delimit when full
		setHeader_respval(inst,400);  // Bad Request
		printHeader(inst,headeronly,Buffer,SEND_BUFFER_SIZE); // No need to read return value as it will flush the buffer

		goto serverquit;
	}

	snprintf(inst->logbuffer,SERVER_BUFFER_SIZE,"\"%s:%d\" %s",inet_ntoa(inst->sin_addr),htons(inst->sin_port),filename+2);
	inst->logbuffer[SERVER_BUFFER_SIZE-1] = 0; // snprintf does not null-delimit when full

	// Check for sub-root hacking, If found send a forbidden.
	tstr=strstr(filename,"..");
	if (tstr!=NULL)
	{
		strlcat(inst->logbuffer," ;",SERVER_BUFFER_SIZE);
		setHeader_respval(inst,403);  // Forbidden
		printHeader(inst,headeronly,Buffer,SEND_BUFFER_SIZE); // No need to read return value as it will flush the buffer

		goto serverquit;
	}

	GETHEAD(inst,headeronly,filename,FILENAME_SIZE);

serverquit:

	server_close(inst);
#ifndef __WIN32__
#ifndef THREAD_POOL
	pthread_exit(NULL);
#endif
	return NULL;
#else
	return 0;
#endif
}
