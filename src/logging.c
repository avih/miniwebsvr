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

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "logging.h"

// The log stream
FILE* LogStream=NULL;
// For Log file buffering
char outbuf[BUFSIZ];

void Log(char* txt)
{
	if (!DOLOG) return;

	if (LogStream==NULL)
		return;

	fprintf(LogStream,"%s\n",txt);
	fflush(LogStream);
}

void StartLogging(char* name)
{
	if (!DOLOG) return;

	if ((LogStream = fopen(name, "at")) == NULL)
	{
		#ifndef USEWINMAIN
		printf("Warning: Failed to open logfile \"%s\", printing logs to stdout.\n",name);
		#else
		char buf[256];
		snprintf(buf,255,"Failed to open logfile \"%s\", printing logs to stdout.\n",name);
		buf[255]=0;
		MessageBox(NULL, buf, "Warning", MB_ICONWARNING);
		#endif
		LogStream=stdout;
	}

	setbuf(LogStream, outbuf);  /// Optimization for speed, OS might not follow spec.
}

void StopLogging()
{
	if (!DOLOG) return;

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
	printf("%s\n",buffer);
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
	#ifndef USEWINMAIN
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

	buffer[MESSAGE_BUFFER-1]=0;
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

	buffer[MESSAGE_BUFFER-1]=0;
	Log(buffer);
#ifndef USEWINMAIN
	fprintf(stderr,"%s\n",buffer);
#else
	va_start(argptr, format);
	vsnprintf(buffer,MESSAGE_BUFFER, format, argptr);
	va_end(argptr);

	MessageBox(NULL, buffer, "Critical", MB_ICONERROR);
#endif
}
