/*  MiniWebSvr - A small webserver
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
#ifndef OS_COMPAT_H
#define OS_COMPAT_H

// Define the 64-bit integer used for seeking
#define _FILE_OFFSET_BITS 64
#define _LARGE_FILES

#ifdef __WIN32__
	// Compile for Windows(R) Sockets
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <winsock2.h>

	#define fseeko fseeko64
	#define ftello ftello64

	#define socklen_t int

	#define SEND_FLAG 0	// Windows does not have a similar flag?

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

	#define SEND_FLAG MSG_NOSIGNAL

#endif // !__WINDOWS__

// Define the 64-bit integer used for seeking
#ifndef INT64
#ifdef __GNUC__
#define INT64 long long
#endif

#ifdef _MSC_VER
#if _MSC_VER >= 1100
#define INT64   __int64
#endif
#endif

#ifdef __BORLANDC__
#define INT64 __int64
#endif
#endif

#ifndef INT64
#warning "No 64-bit type found, 2GB limit on files will apply"
#define INT64 long int
#endif

#endif // OS_COMPAT_H
