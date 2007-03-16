/*  MiniWebsvr - A small webserver
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
#ifndef MULTITHREADED
	free(inst);
#endif
}

int server_charpos(const struct server_struct *inst, const char chr)
{
	unsigned int i;
	
	for (i=inst->buffer_pos;i<inst->buffer_size;++i) 
	{
		if (inst->buffer[i] == chr) 
			return i;
	}
	return -1;
}

int server_readln(struct server_struct *inst, char *str, const unsigned int strsize)
{
	unsigned int strpos;
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
				if (retval > (strsize- strpos))
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
	if (str[strpos] == '\r') 
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
		printHeader(inst,headeronly,Buffer,SEND_BUFFER_SIZE,1); // No need to read return value as it will flush the buffer

		goto serverquit;
	}
	if (filename[2] == 0)
	{
		snprintf(inst->logbuffer,SERVER_BUFFER_SIZE,"\"%s:%d\"",inet_ntoa(inst->sin_addr),htons(inst->sin_port));
		inst->logbuffer[SERVER_BUFFER_SIZE-1] = 0; // snprintf does not null-delimit when full
		setHeader_respval(inst,400);  // Bad Request
		printHeader(inst,headeronly,Buffer,SEND_BUFFER_SIZE,1); // No need to read return value as it will flush the buffer
		
		goto serverquit;
	}

	snprintf(inst->logbuffer,SERVER_BUFFER_SIZE,"\"%s:%d\" %s",inet_ntoa(inst->sin_addr),htons(inst->sin_port),filename+2);
	inst->logbuffer[SERVER_BUFFER_SIZE-1] = 0; // snprintf does not null-delimit when full

	// Check for sub-root hacking, If found send a forbidden.
	tstr=strstr(filename,"/..");
	if ((tstr!=NULL) && ((tstr[3] == 0) || (tstr[3] == '/')))
	{
		strlcat(inst->logbuffer," ;",SERVER_BUFFER_SIZE);
		setHeader_respval(inst,403);  // Forbidden
		printHeader(inst,headeronly,Buffer,SEND_BUFFER_SIZE,1); // No need to read return value as it will flush the buffer
		
		goto serverquit;
	}

	GETHEAD(inst,headeronly,filename,FILENAME_SIZE);

serverquit:

	server_close(inst);
#ifndef __WIN32__
#ifndef MULTITHREADED
	pthread_exit(NULL);
#endif
	return NULL;
#else
	return 0;
#endif
}
