/*  Miniweb - A small webserver
    Copyright (C) 2007  Nickolas Antonie Grigoriadis
    E-Mail: nagrigoriadis@gmail.com

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

// The log stream
FILE* LogStream=NULL;
// For Log file buffering
char outbuf[BUFSIZ];

/// TODO: make thread safe
/// TODO: Optimize
void Log(char* txt) {
	int i;
	if (LogStream==NULL) return;

	i=0;
	while (txt[i]) {
		fputc(txt[i],LogStream);
		i++;
	}
	fputc('\n',LogStream);
	fflush(LogStream);
}

void StartLogging(char* name) {
	if ((LogStream = fopen(name, "at")) == NULL) LogStream=stdout;

	setbuf(LogStream, outbuf);  /// Optimization for speed, OS might not follow spec.
}

void StopLogging() {
	fflush(LogStream);
	if (LogStream!=stdout) fclose(LogStream);
	LogStream=NULL;
}

void Message(const char *format, ...) {
	char buffer[1024];
	va_list argptr;
	va_start(argptr, format);
	vsprintf(buffer, format, argptr);
	va_end(argptr);

	buffer[1023]=0;
	Log(buffer);
}

#ifdef _DEBUG
void DebugMSG(const char *format, ...) {
	char buffer[1024];
	strcpy(buffer,"Debug: ");
	va_list argptr;
	va_start(argptr, format);
	vsprintf(buffer+7, format, argptr);
	va_end(argptr);

	buffer[1023]=0;
	Log(buffer);
	printf("%s\n",buffer);
}
#endif // _DEBUG

void BIGMessage(const char *format, ...) {
	char buffer[1024];
	va_list argptr;
	va_start(argptr, format);
	vsprintf(buffer, format, argptr);
	va_end(argptr);

	buffer[1023]=0;
	Log(buffer);
	printf("%s\n",buffer);
}

void Error(const char *format, ...) {
	char buffer[1024];
	strcpy(buffer,"Error: ");
	va_list argptr;
	va_start(argptr, format);
	vsprintf(buffer+7, format, argptr);
	va_end(argptr);

	buffer[1023]=0;
	Log(buffer);
}

void Critical(const char *format, ...) {
	char buffer[1024];
	strcpy(buffer,"Critical: ");
	va_list argptr;
	va_start(argptr, format);
	vsprintf(buffer+10, format, argptr);
	va_end(argptr);

	buffer[1023]=0;
	Log(buffer);
	fprintf(stderr,"%s\n",buffer);
}
