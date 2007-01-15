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
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

#include "config.h"
#include "header.h"
#include "url.h"

#include "strl.h"

void server_close(struct server_struct *inst)
{
	closesocket(inst->sock);
	free(inst);
}

int server_charpos(const struct server_struct *inst, const char chr)
{
	unsigned int i;
	
	for (i=inst->buffer_pos;i<inst->buffer_size;++i) 
	{
		if (inst->buffer[i] == chr) return i;
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
		if (strpos > strsize) strpos=strsize;
		memcpy(str,inst->buffer+inst->buffer_pos,strpos);
		inst->buffer_pos+=strpos+2; 
	} 
	else 
	{
		// Copy remaining data in buffer
		strpos=inst->buffer_size-inst->buffer_pos;
		if (strpos > strsize) strpos=strsize;
		memcpy(str,inst->buffer+inst->buffer_pos,strpos);
	
		// Assemble rest of string out of multiple buffers
		while (strpos < strsize) 
		{
			// Get data from stream
			inst->buffer_pos=0;
			inst->buffer_size = recv(inst->sock,inst->buffer,SERVER_BUFFER_SIZE,0);

			if (inst->buffer_size == SOCKET_ERROR) 
			{
				fprintf(stderr,"recv() failed: error %d\n",WSAGetLastError());
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
				if (retval>0) memcpy(str+strpos,inst->buffer,retval);
				strpos+=retval;
				break;
			}
			
			// Don't expect a full buffer to allow interactive (non-buffering) connections.
			// No data indicates some connection error, so quit.
			if (inst->buffer_size == 0) break;
		}
	}
	
	// remove \r if present just before the \n
	if (str[strpos] == '\r') --strpos;
	
	// Set the NULL terminator for the string
	++strpos;
	if (strpos > strsize) strpos=strsize;
	str[strpos]=0;
	return strpos-1;	
}

void server_dirlist(struct server_struct *inst,char *dirname,int dirlen)
{
	DIR *dir;
	struct dirent *ent;
	int retval,bufpos;
	char Buffer[SEND_BUFFER_SIZE];
	char FBuffer[FILENAME_SIZE];
	char *cptr;
	struct stat statbuf;

	if (dirlen>FILENAME_SIZE) dirlen=FILENAME_SIZE;

	retval=strnlen(dirname,dirlen);
	if (dirname[retval-1] != '/')
	{
		dirname[retval] = '/';
		dirname[retval+1] = 0;
	}
	if ((dir = opendir(dirname)) == NULL)
	{
		retval=strnlen(dirname,dirlen);
		if (dirname[retval-1] == '/') dirname[retval-1] = 0;

		setHeader_respval(inst,404);  // Not Found
		printHeader(inst,Buffer,SEND_BUFFER_SIZE); // No need to read return value as it will flush the buffer
		return;
	}
	else
	{
		setHeader_filename(inst,".html");
		setHeader_respval(inst,201);  // Created
		bufpos=printHeader(inst,Buffer,SEND_BUFFER_SIZE);

		bufpos+=snprintf(Buffer+bufpos,SEND_BUFFER_SIZE-bufpos,"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2//EN\">\n<HTML><HEAD><TITLE>Index of %s</TITLE></HEAD><BODY><H1>Index of %s</H1><PRE><HR>\n",dirname+1,dirname+1);
		send(inst->sock,Buffer,bufpos,0);

		while ((ent = readdir(dir)) != NULL)
		if (strcmp(ent->d_name,".")!=0)
		if ((strcmp(dirname,"./")!=0) || (ent->d_name[0]!='.'))
		{
			strlcpy(FBuffer,dirname,FILENAME_SIZE);
			if (strcmp(ent->d_name,"..")!=0)
			{
				strlcat(FBuffer,ent->d_name,FILENAME_SIZE);
				stat(FBuffer, &statbuf);

				if (statbuf.st_mode & S_IFDIR)
				{
					retval=strnlen(FBuffer,FILENAME_SIZE);
					if (FBuffer[retval-1] != '/')
					{
						FBuffer[retval] = '/';
						FBuffer[retval+1] = 0;
					}
				}
			}
			else
			{
				retval=strnlen(dirname,FILENAME_SIZE);
				if (FBuffer[retval-1] == '/') FBuffer[retval-1] = 0;
				if ((cptr=strrchr(FBuffer,'/')) != NULL) cptr[1] = 0;
				else strlcpy(FBuffer,"/",FILENAME_SIZE);

			}

			bufpos=snprintf(Buffer,SEND_BUFFER_SIZE,"<A href=\"%s\">%s</A>\n",FBuffer+1,ent->d_name);
			send(inst->sock,Buffer,bufpos,0);
		}

		closedir(dir);

		bufpos=snprintf(Buffer,SEND_BUFFER_SIZE,"</PRE><HR><ADDRESS>%s Server Port %d</ADDRESS></BODY></HTML>",VERSION,PORT);
		send(inst->sock,Buffer,bufpos,0);
	}
}

DWORD WINAPI server(struct server_struct *inst)
{
	char Buffer[SEND_BUFFER_SIZE];
	char filename[FILENAME_SIZE];
	char GHBuffer[SERVER_BUFFER_SIZE];
	int retval;
	FILE *in;
	char *tstr;
        struct stat statbuf;
        struct tm *loctime;

	// Initialize structure
	inst->buffer_pos=0;
	inst->buffer_size=0;

	filename[0]='.';
	filename[1]=0;
	clearHeader(inst);

//CHANGE replaced the parsing
	// Parse header
	retval = server_readln(inst,GHBuffer,SERVER_BUFFER_SIZE);
	// check for GET requests
	if ( retval > 4 && 0 == strncmp(GHBuffer, "GET ", 4))
		urldecode(GHBuffer+4, filename+1, FILENAME_SIZE-1);
	else
        {
                // Unknown request type, also print request to logfile
                snprintf(inst->logbuffer,SERVER_BUFFER_SIZE,"\"%s:%d\"",inet_ntoa(inst->sin_addr),htons(inst->sin_port));
                inst->logbuffer[SERVER_BUFFER_SIZE-1] = 0; // snprintf does not null-delimit when full
                inst->MIMEoverride = GHBuffer; // Override MIME type with unknown request type
                setHeader_respval(inst,501); // Not Implemented
                printHeader(inst,Buffer,SEND_BUFFER_SIZE); // No need to read return value as it will flush the buffer

                goto serverquit;
        }
	if (filename[1] == 0)
	{
		snprintf(inst->logbuffer,SERVER_BUFFER_SIZE,"\"%s:%d\"",inet_ntoa(inst->sin_addr),htons(inst->sin_port));
                inst->logbuffer[SERVER_BUFFER_SIZE-1] = 0; // snprintf does not null-delimit when full
		setHeader_respval(inst,400);  // Bad Request
		printHeader(inst,Buffer,SEND_BUFFER_SIZE); // No need to read return value as it will flush the buffer
		
		goto serverquit;
	}

	snprintf(inst->logbuffer,SERVER_BUFFER_SIZE,"\"%s:%d\" %s",inet_ntoa(inst->sin_addr),htons(inst->sin_port),filename);
        inst->logbuffer[SERVER_BUFFER_SIZE-1] = 0; // snprintf does not null-delimit when full

	// Check for special "device" called "nul"
	tstr=strstr(filename,"/nul");
	if (tstr!=NULL) 
	{
		if ((tstr[4] == '.') || (tstr[4] == 0))
			strcpy(filename,"../");  // nul device
	}

	// Check for sub-root hacking, If found send a forbidden.
	if (strstr(filename,"../")!=NULL) 
	{
		strlcat(inst->logbuffer," ;",SERVER_BUFFER_SIZE);
		setHeader_respval(inst,403);  // Forbidden
		printHeader(inst,Buffer,SEND_BUFFER_SIZE); // No need to read return value as it will flush the buffer
		
		goto serverquit;
	}
	if ((in = fopen(filename, "rb")) == NULL)
	{
		retval=strlcpy(GHBuffer,filename,SERVER_BUFFER_SIZE);
		if (GHBuffer[retval-1] == '/') {
			strlcat(GHBuffer,"index.html",SERVER_BUFFER_SIZE);
			in = fopen(GHBuffer, "rb");
			if (in != NULL) 
			{
				strlcat(inst->logbuffer,"[index.html]",SERVER_BUFFER_SIZE);
				strlcat(filename,"index.html",FILENAME_SIZE);
			}
		}
	}
	
	strlcat(inst->logbuffer," ;",SERVER_BUFFER_SIZE);
	if (in == NULL)
	{
		server_dirlist(inst,filename,FILENAME_SIZE);
	}
	else 
	{
		int ret,blen;
		filename[FILENAME_SIZE-1]=0;
		setHeader_filename(inst,filename);
		setHeader_respval(inst,200);  // OK
		blen=0;
		if (!fseek(in,0,SEEK_END))
		{
			// Supports seek
			blen=snprintf(GHBuffer,SERVER_BUFFER_SIZE,"Content-Length: %ld\r\n",ftell(in));
			fseek(in,0,SEEK_SET);
		}
                if (stat(filename, &statbuf) == 0) {
                        // Supports filestats
                        loctime = gmtime (&statbuf.st_mtime);
                        strftime(GHBuffer+blen,SERVER_BUFFER_SIZE-blen,"Last-Modified: %a, %d %b %Y %I:%M:%S GMT\r\n",loctime);
                }

		GHBuffer[SERVER_BUFFER_SIZE-1] = 0; // strnprintf does not null-delimit when full
		setHeader_generic(inst,GHBuffer);
		blen=printHeader(inst,Buffer,SEND_BUFFER_SIZE);

		while ((retval=fread(Buffer+blen, 1,SEND_BUFFER_SIZE-blen, in)) > 0) {
			retval+=blen;
			ret=0;
			blen=0;
//			printf("1: bpos=%d\tret=%d\tretval=%d\n",blen,ret,retval);

			while ((ret < retval) && (retval > 0)) 
			{
				ret=send(inst->sock,Buffer+blen,retval,0);
//				printf("2: bpos=%d\tret=%d\tretval=%d\n",blen,ret,retval);

				if (ret == 0) 
				{
//					printf("transmission error\n");
					fclose(in);
					goto serverquit;
				}
				blen+=ret;
				retval-=ret;
				ret=0;
//				printf("3: bpos=%d\tret=%d\tretval=%d\n",blen,ret,retval);
			}
			blen=0;
		}
//		printf("While quit\nbpos=%d\tret=%d\tretval=%d\n\n",blen,ret,retval);
		fclose(in);
	}

serverquit:

	server_close(inst);
	return 0;
}
