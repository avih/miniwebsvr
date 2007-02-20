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
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "logging.h"
#include "config.h"

// The log stream
FILE* LogStream=NULL;
// For Log file buffering
char outbuf[BUFSIZ];

#ifdef LIB
extern server_config config;

void inline Log(char* txt)
{
	if(config.logger_hook != NULL)
		config.logger_hook(txt);
}

void mwb_Log(char* txt) 
#else
void Log(char* txt)
#endif
{
	if (LogStream==NULL) 
		return;

	fprintf(LogStream,"%s\n",txt);
	fflush(LogStream);
}
void StartLogging(char* name) 
{
	if ((LogStream = fopen(name, "at")) == NULL) 
		LogStream=stdout;

	setbuf(LogStream, outbuf);  /// Optimization for speed, OS might not follow spec.
}

void StopLogging() 
{
	fflush(LogStream);
	if (LogStream!=stdout)
		fclose(LogStream);
	LogStream=NULL;
}

void Message(const char *format, ...) 
{
	char buffer[MESSAGE_BUFFER];
	va_list argptr;

	va_start(argptr, format);
	vsnprintf(buffer, MESSAGE_BUFFER, format, argptr);
	va_end(argptr);

	buffer[MESSAGE_BUFFER-1]=0;
	Log(buffer);
}

#ifdef _DEBUG
void DebugMSG(const char *format, ...) 
{
	char buffer[MESSAGE_BUFFER];
	va_list argptr;

	va_start(argptr, format);
	vsnprintf(buffer,MESSAGE_BUFFER, format, argptr);
	va_end(argptr);

	buffer[MESSAGE_BUFFER-1]=0;
	Log(buffer);
#ifndef LIB
	printf("%s\n",buffer);
#endif
}
#endif // _DEBUG

void BIGMessage(const char *format, ...) 
{
	char buffer[MESSAGE_BUFFER];
	va_list argptr;

	va_start(argptr, format);
	vsnprintf(buffer,MESSAGE_BUFFER, format, argptr);
	va_end(argptr);

	buffer[MESSAGE_BUFFER-1]=0;
	Log(buffer);
#ifndef LIB
	printf("%s\n",buffer);
#endif
}

void Error(const char *format, ...) 
{
	char buffer[MESSAGE_BUFFER];
	va_list argptr;

	memcpy(buffer,"Error: ",7);
	va_start(argptr, format);
	vsnprintf(buffer+7,MESSAGE_BUFFER-7, format, argptr);
	va_end(argptr);

	buffer[MESSAGE_BUFFER]=0;
	Log(buffer);
}

void Critical(const char *format, ...) 
{
	char buffer[MESSAGE_BUFFER];
	va_list argptr;

	memcpy(buffer,"Critical: ",10);
	va_start(argptr, format);
	vsnprintf(buffer+10,MESSAGE_BUFFER-10, format, argptr);
	va_end(argptr);

	buffer[MESSAGE_BUFFER]=0;
	Log(buffer);
	fprintf(stderr,"%s\n",buffer);
}
