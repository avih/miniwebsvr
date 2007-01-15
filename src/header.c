/*  Miniweb - A small webserver
    Copyright (C) 2007  Nickolas Antonie Grigoriadis
    E-Mail: grigi_ at users.sourceforge.net

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

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
#include <ctype.h>
#include <time.h>

#include "header.h"
#include "logging.h"
#include "config.h"

#include "strl.h"

const struct headerResp_struct headerResp[headerResp_size] =
	{{1,501,"Not Implemented"}, // Default
	{0,200,"OK"},
	{0,201,"Created"},
	{0,304,"Not Modified"},
	{0,307,"Tempory Redirect"},
	{1,400,"Bad Request"},
	{1,403,"Forbidden"},
	{1,404,"Not Found"}};

const struct headerExt_struct headerExt[headerExt_size] = 
	{{"",""}, // Default type
	{"html","text/html"},
	{"htm","text/html"},
	
	{"xml","text/xml"},
	
	{"txt","text/plain"},
	{"c","text/plain"},
	{"h","text/plain"},
	{"ini","text/plain"},
	{"cpp","text/plain"},
	{"log","text/plain"},
	
	{"jpeg","image/jpeg"},
	{"jpg","image/jpeg"},
	{"png","image/png"},
	{"bmp","image/bmp"},
	{"gif","image/gif"},
	{"ico","image/x-icon"}};	

void setHeader_generic(struct server_struct *inst,char* genstr)
{
	strlcat(inst->header_content,genstr,SERVER_BUFFER_SIZE);
}

void setHeader_filename(struct server_struct *inst,char* filename) 
{
	unsigned int i,len;
	char ext[EXT_size];
	char* str;
	
	inst->MIMEtype[0]=0;
	str=strrchr(filename,'.');
	
	if (str == NULL) return;
	len=strlen(str);
	if (len>EXT_size) return;

	++str; --len;

	for (i=0;i<len;++i) ext[i]=tolower(str[i]);
	ext[len]=0;
	
	for (i=1;i<headerExt_size;++i) 
	{
		if (strncmp(ext,headerExt[i].ext,len)==0)
		{
			strlcpy(inst->MIMEtype,headerExt[i].type,MIME_size);
			return;
		}
	}
}

void setHeader_respval(struct server_struct *inst,int respval) 
{
	int i;
	
	inst->respval=0;
	for (i=1;i<headerResp_size;++i)
	{
		if (headerResp[i].respval == respval) 
		{
			inst->respval=i;
			break;
		}
	}
}

void clearHeader(struct server_struct *inst)
{
	inst->respval=0;
	inst->MIMEtype[0]=0;
	inst->header_content[0]=0;
        inst->MIMEoverride=0;
}


int printHeader(struct server_struct *inst, char* Buffer, int bufsize)
{
	int bufpos,tmp;
	time_t curtime;
	struct tm *loctime;

	// Get the current time.
	curtime = time (NULL);
	// Convert it to local time representation.
	loctime = localtime (&curtime);
	strftime(Buffer,bufsize,"%a %b %d %I:%M:%S %Y",loctime);
	Buffer[bufsize-1]=0;  // Make sure string is null-terminated

        if (!inst->MIMEoverride) inst->MIMEoverride=inst->MIMEtype;
	Message("%s : %s %d %s : %s",Buffer,inst->logbuffer,headerResp[inst->respval].respval,headerResp[inst->respval].respstr,inst->MIMEoverride);

	// OK, start buffering!
	bufpos=snprintf(Buffer,bufsize,"HTTP/1.1 %d %s\r\n",headerResp[inst->respval].respval,headerResp[inst->respval].respstr);
	//send(inst->sock,Buffer,strnlen(Buffer,bufsize),0);
	if (inst->MIMEtype[0] != 0) 
	{
		bufpos+=snprintf(Buffer+bufpos,bufsize-bufpos,"Content-Type: %s\r\n",inst->MIMEtype);
		//send(inst->sock,Buffer,strlen(Buffer),0);
	}
	if (inst->header_content[0] != 0)
	{
		tmp=strlen(inst->header_content);
		if (tmp<bufsize-bufpos)
		{
			memcpy(Buffer+bufpos,inst->header_content,tmp);
			bufpos+=tmp;
		}
		//send(inst->sock,inst->header_content,strlen(inst->header_content),0);
	}
	bufpos+=snprintf(Buffer+bufpos,bufsize-bufpos,"Server: %s\r\n",VERSION);
	//send(inst->sock,Buffer,strlen(Buffer),0);
        loctime = gmtime (&curtime);
	bufpos+=strftime(Buffer+bufpos,bufsize-bufpos,"Date: %a, %d %b %Y %I:%M:%S GMT\r\n\r\n",loctime);
	Buffer[bufsize-1]=0;
	//send(inst->sock,Buffer,strlen(Buffer),0);

	if (headerResp[inst->respval].autogen) 
	{
		bufpos+=snprintf(Buffer+bufpos,bufsize-bufpos,"<HTML><HEAD><TITLE>HTTP %d - %s</TITLE></HEAD><BODY><H1>HTTP %d - %s<H1><H3>MiniWeb web server</H3></BODY></HTML>"
			,headerResp[inst->respval].respval,headerResp[inst->respval].respstr
			,headerResp[inst->respval].respval,headerResp[inst->respval].respstr);
		send(inst->sock,Buffer,bufpos,0);	
		return 0; // Flushed buffer	
	}

	return bufpos; // Bytes in buffer that is "unflushed"
}
