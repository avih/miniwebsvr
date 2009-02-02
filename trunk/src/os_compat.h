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
#ifndef OS_COMPAT_H
#define OS_COMPAT_H

// Define that I WANT 64-bit file oprations
#define _FILE_OFFSET_BITS 64
#define _LARGE_FILES

#ifdef __WIN32__
	// Compile for Windows(R) Sockets
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <winsock2.h>
	#include <sys/stat.h>

        #ifdef _OFF64_T_
	#ifdef __GNUC__
	#define LTYPE "%lld"
	#else
	#define LTYPE "%I64d"
	#endif //__GNUC__
	#define structstat _stati64
	#define fseeko fseeko64
	#define ftello ftello64
	#define funcstat _stati64
	#define fopen fopen64
	#define INT64 __int64
        #else
        #define LTYPE "%ld"
        #define structstat stat
        #define funcstat stat
        #define fseeko fseek
	#define ftello ftell
	#define INT64 long
        // Comment this error out if you think the 2GB limit is acceptable
        #error "No 64-bit type found, 2GB limit on files will apply"
        #endif

       	#define socklen_t int

	#define SEND_FLAG 0	// Windows does not have a similar flag?

	// Define that I want unicode support
	#define USE_UNICODE

#else // !_WINDOWS__
	// Compile for UNIX Sockets
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <sys/select.h>
	#include <sys/wait.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h>
	#include <arpa/inet.h>
	#include <errno.h>
	#include <string.h>
	#include <unistd.h>
	#include <pthread.h>

	#define SOCKET int
	#define DWORD unsigned int
	#define SOCKET_ERROR -1
	#define INVALID_SOCKET -1
	#define HANDLE pthread_t
	#define structstat stat
	#define funcstat stat

	#define SEND_FLAG MSG_NOSIGNAL

	// Define the 64-bit integer used for seeking if not defined yet
	#ifndef INT64
	#ifdef __int64
	#define INT64 __int64
	#else // __int64
	#ifdef __GNUC__
	#define INT64 long long
	#endif // __GNUC__
	#endif // __int64
	#endif // INT64

	#ifndef INT64
        // Comment this error out if you think the 2GB limit is acceptable
	#error "No 64-bit type found, 2GB limit on files will apply"
	#define INT64 long int
	#define LTYPE "%ld"
	#else
	#define LTYPE "%lld"
	#endif

#endif // !__WINDOWS__

#ifdef USE_UNICODE
	// Unicode layer (Currently only implemented for WIN32 target)
	#include <stdlib.h>
	#include "utf8_layer.h"
#endif

#endif // OS_COMPAT_H
