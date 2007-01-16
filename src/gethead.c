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
#include "gethead.h"

#include "strl.h"

void server_dirlist(struct server_struct *inst,int headeronly,char *dirname,int dirlen)
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
		if (dirname[retval-1] == '/') 
			dirname[retval-1] = 0;

		setHeader_respval(inst,404);  // Not Found
		printHeader(inst,headeronly,Buffer,SEND_BUFFER_SIZE); // No need to read return value as it will flush the buffer
		return;
	}
	else
	{
		setHeader_filename(inst,".html");
		setHeader_respval(inst,201);  // Created
		bufpos=printHeader(inst,headeronly,Buffer,SEND_BUFFER_SIZE);

		if (headeronly == 1) {
			send(inst->sock,Buffer,bufpos,0);
			return;
		}

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
					if (FBuffer[retval-1] == '/') 
						FBuffer[retval-1] = 0;
					if ((cptr=strrchr(FBuffer,'/')) != NULL) 
						cptr[1] = 0;
					else 
						strlcpy(FBuffer,"/",FILENAME_SIZE);
				}

				bufpos=snprintf(Buffer,SEND_BUFFER_SIZE,"<A href=\"%s\">%s</A>\n",FBuffer+1,ent->d_name);
				send(inst->sock,Buffer,bufpos,0);
			}

		closedir(dir);

		bufpos=snprintf(Buffer,SEND_BUFFER_SIZE,"</PRE><HR><ADDRESS>%s Server Port %d</ADDRESS></BODY></HTML>",VERSION,PORT);
		send(inst->sock,Buffer,bufpos,0);
	}
}

void GETHEAD(struct server_struct *inst,int headeronly,char *filename,int filebufsize) 
{
	char Buffer[SEND_BUFFER_SIZE];
	char GHBuffer[SERVER_BUFFER_SIZE];
	int retval;
	FILE *in;
	struct stat statbuf;
	struct tm *loctime;

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
		server_dirlist(inst,headeronly,filename,filebufsize);
	}
	else 
	{
		int ret,blen;
		filename[filebufsize-1]=0;
		setHeader_filename(inst,filename);
		setHeader_respval(inst,200);  // OK
		blen=0;
		if (!fseek(in,0,SEEK_END))
		{
			// Supports seek
			blen=snprintf(GHBuffer,SERVER_BUFFER_SIZE,"Content-Length: %ld\r\n",ftell(in));
			fseek(in,0,SEEK_SET);
		}
		if (stat(filename, &statbuf) == 0) 
		{
			// Supports filestats
			loctime = gmtime (&statbuf.st_mtime);
			strftime(GHBuffer+blen,SERVER_BUFFER_SIZE-blen,"Last-Modified: %a, %d %b %Y %I:%M:%S GMT\r\n",loctime);
		}

		GHBuffer[SERVER_BUFFER_SIZE-1] = 0; // strnprintf does not null-delimit when full
		setHeader_generic(inst,GHBuffer);
		blen=printHeader(inst,headeronly,Buffer,SEND_BUFFER_SIZE);

		if (headeronly == 1) {
			send(inst->sock,Buffer,blen,0);
			fclose(in);
			return;
		}


		while ((retval=fread(Buffer+blen, 1,SEND_BUFFER_SIZE-blen, in)) > 0) 
		{
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
					return;
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
}
