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
	{0,206,"Partial Content"},
	{1,301,"Moved Permanently"},
	{0,304,"Not Modified"},
	{0,307,"Tempory Redirect"},
	{1,400,"Bad Request"},
	{1,403,"Forbidden"},
	{1,404,"Not Found"},
	{1,416,"Requested Range Not Satisfiable"}};

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

	if (str == NULL)
		return;
	len=strlen(str);
	if (len>EXT_size)
		return;

	++str; --len;

	for (i=0;i<len;++i)
		ext[i]=tolower(str[i]);
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


int printHeader(struct server_struct *inst, int headeronly, char* Buffer, int bufsize)
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

	if (!inst->MIMEoverride)
		inst->MIMEoverride=inst->MIMEtype;
	Message("%s : %s %d %s : %s",Buffer,inst->logbuffer,headerResp[inst->respval].respval,headerResp[inst->respval].respstr,inst->MIMEoverride);

	// OK, start buffering!
	bufpos=snprintf(Buffer,bufsize,"HTTP/1.1 %d %s\r\n",headerResp[inst->respval].respval,headerResp[inst->respval].respstr);
	if (inst->MIMEtype[0] != 0)
	{
		bufpos+=snprintf(Buffer+bufpos,bufsize-bufpos,"Content-Type: %s\r\n",inst->MIMEtype);
	}

	if (inst->header_content[0] != 0)
	{
		tmp=strlen(inst->header_content);
		if (tmp<bufsize-bufpos)
		{
			memcpy(Buffer+bufpos,inst->header_content,tmp);
			bufpos+=tmp;
		}
	}

	if (LISTSERVER)
		bufpos+=snprintf(Buffer+bufpos,bufsize-bufpos,"Server: %s\r\n",VERSION);

	loctime = gmtime (&curtime);
	bufpos+=strftime(Buffer+bufpos,bufsize-bufpos,"Date: %a, %d %b %Y %I:%M:%S GMT\r\n\r\n",loctime);
	Buffer[bufsize-1]=0;
	DebugMSG("\n%s",Buffer);

	if (headerResp[inst->respval].autogen)
	{
		if (headeronly==0) {
			bufpos+=snprintf(Buffer+bufpos,bufsize-bufpos,"<HTML><HEAD><TITLE>HTTP %d - %s</TITLE></HEAD><BODY><H1>HTTP %d - %s<H1><H3>%s</H3></BODY></HTML>"
				,headerResp[inst->respval].respval,headerResp[inst->respval].respstr
				,headerResp[inst->respval].respval,headerResp[inst->respval].respstr
				,(LISTSERVER?VERSION:""));
		}
		send(inst->sock,Buffer,bufpos,SEND_FLAG);
		return 0; // Flushed buffer
	}

	return bufpos; // Bytes in buffer that is "unflushed"
}
