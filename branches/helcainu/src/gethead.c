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
#include <sys/types.h>
#include <time.h>

#include "config.h"

#ifdef LIB
#include "server.h"
#include "hooks.h"
#endif
#include "header.h"
#include "gethead.h"
#include "logging.h"

#include "strl.h"

extern server_config config;

void server_dirlist(struct server_struct *inst,int headeronly,char *dirname,int dirlen)
{
	DIR *dir;
	struct dirent *ent;
	int retval,bufpos;
	char Buffer[SEND_BUFFER_SIZE];
	char FBuffer[FILENAME_SIZE];
	char *cptr;
	struct stat statbuf;
	int isdir;

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
		printHeader(inst,headeronly,Buffer,SEND_BUFFER_SIZE,1); // No need to read return value as it will flush the buffer
		return;
	}
	else
	{
		setHeader_filename(inst,".html");
		setHeader_respval(inst,201);  // Created
		bufpos=printHeader(inst,headeronly,Buffer,SEND_BUFFER_SIZE,1);

		if (headeronly == 1) {
			send(inst->sock,Buffer,bufpos,SEND_FLAG);
			return;
		}

		bufpos+=snprintf(Buffer+bufpos,SEND_BUFFER_SIZE-bufpos,"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2//EN\">\n<HTML><HEAD><TITLE>Index of %s</TITLE></HEAD><BODY><H1>Index of %s</H1><PRE><HR>\n",dirname+2,dirname+2);
		send(inst->sock,Buffer,bufpos,SEND_FLAG);

		while ((ent = readdir(dir)) != NULL)
		if (strcmp(ent->d_name,".")!=0)
			if ((strcmp(dirname,"./")!=0) || (ent->d_name[0]!='.'))
			{
				isdir=0;
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
							isdir=1;
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
					isdir=1;
				}

				bufpos=snprintf(Buffer,SEND_BUFFER_SIZE,"<A href=\"%s\">%s%s</A>\n",FBuffer+2,ent->d_name,(isdir?"/":""));
				send(inst->sock,Buffer,bufpos,SEND_FLAG);
			}

		closedir(dir);

		bufpos=snprintf(Buffer,SEND_BUFFER_SIZE,"</PRE><HR><ADDRESS>%s Server Port %d</ADDRESS></BODY></HTML>",VERSION,config.port);
		send(inst->sock,Buffer,bufpos,SEND_FLAG);
	}
}

void GETHEAD(struct server_struct *inst,int headeronly,char *filename,int filebufsize)
{
	char Buffer[SEND_BUFFER_SIZE];
	char GHBuffer[SERVER_BUFFER_SIZE];
	char TMPBuffer[SERVER_BUFFER_SIZE];
	char TimeBuffer[SERVER_BUFFER_SIZE];
	int retval,ret,blen,range;
        long int contentlength,rangefrom,rangeto;
	FILE *in;
	struct stat statbuf;
	struct tm *loctime;
        int statret;

	if ((statret=stat(filename, &statbuf)) == 0)
	{
		// Supports filestats
                if (!((statbuf.st_mode & (S_IFREG | S_IFDIR)) && (statbuf.st_ctime != -1))) {
			// Not a regular file OR a directory OR has no creation time!!!!
			strlcat(inst->logbuffer," ;",SERVER_BUFFER_SIZE);
			setHeader_respval(inst,403);  // Forbidden
			printHeader(inst,headeronly,Buffer,SEND_BUFFER_SIZE,1); // No need to read return value as it will flush the buffer
			return;
		}
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
                                statret=stat(filename, &statbuf);
			}
		}
	}
        strlcat(inst->logbuffer," ;",SERVER_BUFFER_SIZE);
        if (in == NULL)
	{
		server_dirlist(inst,headeronly,filename,filebufsize);
	}
	else if(statbuf.st_mode & S_IFDIR)
	{
		fclose(in);
		server_dirlist(inst,headeronly,filename,filebufsize);
	}
#ifdef LIB
	else if(check_hook(filename, inst))
	{
		fclose(in);
	}
#endif
	else 
	{
		// Start Header parsing
		// Reset some internal variables
		TimeBuffer[0]=0;
                contentlength=0;
                rangefrom=0;
		range=0;
		GHBuffer[0]=0;

		// Read until blank line
		while ((retval = server_readln(inst,TMPBuffer,SERVER_BUFFER_SIZE)) != 0) {
                        DebugMSG("%s",TMPBuffer);
			if (0 == strncmp(TMPBuffer, "If-Modified-Since: ", 19))
			{
				// Comare date string to existing
				strlcpy(TimeBuffer,TMPBuffer+19,SERVER_BUFFER_SIZE-19);
			}

                        if (0 == strncmp(TMPBuffer, "Range: ", 7))
                        {
                                char *range2;
                                char *range1 = strstr(TMPBuffer,"bytes=");
                                if (range1)
                                {
                                        rangeto=0;
                                        sscanf(range1,"bytes=%ld-",&rangefrom);
                                        range2 = strstr(range1,"-");
                                        if (range2)
                                                sscanf(range2,"-%ld",&rangeto);
                                        DebugMSG("RANGE: %s (%ld - %ld)",range1,rangefrom,rangeto);
                                        range+=1;
                                }
                        }

                        if (0 == strncmp(TMPBuffer, "If-Range: ", 10))
                        {
                                range+=2;
                        }
		}

		// Check that If-Range was done with Range
		if (range == 2)
			range = 0; // If-Range without Range is invalid

		filename[filebufsize-2]=0;
		setHeader_filename(inst,filename);
		setHeader_respval(inst,200);  // OK
		blen=0;
                if (!statret) {
		        loctime = gmtime (&statbuf.st_mtime);
                        blen=strftime(GHBuffer,SERVER_BUFFER_SIZE,"Last-Modified: %a, %d %b %Y %I:%M:%S GMT\r\n",loctime);
		        if (strncmp(TimeBuffer,GHBuffer+15,strlen(GHBuffer+15)-2) == 0 )
		        {
			        // If-Modified-Since matches, therefore no update
			        setHeader_respval(inst,304);  // Not Modified
			        headeronly=1;
                        }
                }

		if (!fseek(in,0,SEEK_END))
		{
			// Supports seek
                        contentlength=ftell(in);
		}

                // Is a Partial request? Force a full DL, unless below changes it
                if (range)
                {
                        setHeader_respval(inst,200);  // OK
                        headeronly=0;
                }

                // Partial download may only happen when "If-Modified-Since" did NOT FAIL and is seekable
                if (range && contentlength && ((TimeBuffer[0] == 0) || (inst->respval == 304)))
                {
                        // OK, partial downloads MAY happen

                        if ((rangefrom >= contentlength) || (rangeto >= contentlength))
                        {
				// Cannot satisfy "Range" request
				if (range == 1) 
				{
                                        setHeader_respval(inst,416);  // Requested Range Not Satisfiable
                                        range=-1;
                                }
				else
				{
					range=0;  // Act as if no range request;
				}
			}
                        else
                        {
                        	// Range request IS valid
                                setHeader_respval(inst,206);  // Partial Content
                        }
                }
                else if ((range == 1) && (contentlength || (TimeBuffer[0] != 0)))
                {
                        // "Range" with failed conditional or unseekable returns 416
                        setHeader_respval(inst,416);  // Requested Range Not Satisfiable
                        headeronly=0;
                        range=-1;
                }

                if (contentlength > 0)
                {
			long int rangeto2 = rangeto;
			if (!rangeto2)
				rangeto2 = contentlength-1;
			blen+=snprintf(GHBuffer+blen,SERVER_BUFFER_SIZE-blen,"Content-Range: bytes %ld-%ld/%ld\r\n",rangefrom,rangeto2,contentlength);
		}

                if (range > 0)
                {
                        fseek(in,rangefrom,SEEK_SET);
                        if (rangeto)
                                contentlength = rangeto - rangefrom;
                        else
                                contentlength-=rangefrom;
                }

                if (contentlength > 0)
                {
			blen+=snprintf(GHBuffer+blen,SERVER_BUFFER_SIZE-blen,"Content-Length: %ld\r\n",contentlength);
			fseek(in,rangefrom,SEEK_SET);
                }

                // Test for no late error
                if (range != -1)
                {
              		GHBuffer[SERVER_BUFFER_SIZE-1] = 0; // strnprintf does not null-delimit when full
                        setHeader_generic(inst,GHBuffer);

		        blen=printHeader(inst,headeronly,Buffer,SEND_BUFFER_SIZE,1);
                }
                else
                {
                        // Late error
                        printHeader(inst,headeronly,Buffer,SEND_BUFFER_SIZE,1);
                        return;
                }

                if (headeronly == 1)
                {
			send(inst->sock,Buffer,blen,SEND_FLAG);
			fclose(in);
			return;
		}


                if (contentlength > 0)
                {
                        // I know exactly how much I need to read
                        contentlength += blen; // Add existing buffer, and then only keep track of data sent over socket
                        DebugMSG("c+b: %d",contentlength);
                        retval=fread(Buffer+blen, 1,SEND_BUFFER_SIZE-blen, in);
                        retval+=blen;
                        do
        		{
        			ret=0;
        			blen=0;
                                contentlength -= retval;

        			while ((ret < retval) && (retval > 0))
        			{
        				ret=send(inst->sock,Buffer+blen,retval,SEND_FLAG);

//                                        DebugMSG("%d - %d",contentlength,ret);
        				if (ret <= 0)
        				{
        					// Some transmission error
        					fclose(in);
                                                DebugMSG("Transmission ended prematurely");
        					return;
        				}
        				blen+=ret;
	        			retval-=ret;
        				ret=0;
		        	}
        			blen=0;
                                if (contentlength < SEND_BUFFER_SIZE)
                                        blen =  SEND_BUFFER_SIZE - contentlength; // To ensure that I only read as much as is left.
        		}
        		while ((contentlength > 0) && ((retval=fread(Buffer, 1,SEND_BUFFER_SIZE-blen, in)) > 0));
                }
                else
                {
                        // I DONT know how much I need to read
        		while ((retval=fread(Buffer+blen, 1,SEND_BUFFER_SIZE-blen, in)) > 0)
        		{
        			retval+=blen;
        			ret=0;
        			blen=0;

        			while ((ret < retval) && (retval > 0))
        			{
        				ret=send(inst->sock,Buffer+blen,retval,SEND_FLAG);

//                                        DebugMSG("%d",ret);
        				if (ret <= 0)
        				{
        					// Some transmission error
        					fclose(in);
                                                DebugMSG("Transmission ended prematurely");
        					return;
        				}
        				blen+=ret;
	        			retval-=ret;
        				ret=0;
		        	}
        			blen=0;
        		}
                }
		fclose(in);
	}
}
