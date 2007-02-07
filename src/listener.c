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
#include <malloc.h>

#include "config.h"
#include "listener.h"
#include "server.h"
#include "logging.h"

int loop;

int listener(char *interface, unsigned short port) 
{
	struct timeval timeout;// = {1,0}; // 1 second poll state

	int ret,on;
	socklen_t fromlen;
	struct sockaddr_in local, from;
	SOCKET listen_socket, msgsock;
	fd_set socket_set;

#ifdef __WIN32__
	WSADATA wsaData;

	if ((ret=WSAStartup(0x202,&wsaData)) != 0) // Request WinSock API v2.2
	{
		Critical("WSAStartup failed with error %d",ret);
		WSACleanup();
		return -1;
	}
#endif
	
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = (!interface)?INADDR_ANY:inet_addr(interface); 

	local.sin_port = htons(port);

	listen_socket = socket(AF_INET, SOCK_STREAM,0); // TCP socket
	
	// Enable address reuse
	on = 1;
	setsockopt( listen_socket, SOL_SOCKET, SO_REUSEADDR, (void*)&on, sizeof(on) );
	// Disable Nagle 
	// Since we are implementing our own buffering Nagle just gets in the way.
	setsockopt( listen_socket, IPPROTO_TCP, TCP_NODELAY, (void*)&on, sizeof(on) );

	if (listen_socket == INVALID_SOCKET)
	{
		#ifdef __WIN32__
		ret = WSAGetLastError();
		#else
		ret = errno;
		#endif

		Critical("socket() failed with error %d",ret);
		#ifdef __WIN32__
		WSACleanup();
		#endif
		return -1;
	}

	if (bind(listen_socket,(struct sockaddr*)&local,sizeof(local)) == SOCKET_ERROR) 
	{
		#ifdef __WIN32__
		ret = WSAGetLastError();
		#else
		ret = errno;
		#endif

		Critical("bind() failed with error %d",ret);
		#ifdef __WIN32__
		WSACleanup();
		#endif
		return -1;
	}

	if (listen(listen_socket,SOMAXCONN) == SOCKET_ERROR) // Listen with backlog set to max reasonable by OS
	{
		#ifdef __WIN32__
		ret = WSAGetLastError();
		#else
		ret = errno;
		#endif
		
		Critical("listen() failed with error %d",ret);
		#ifdef __WIN32__
		WSACleanup();
		#endif
		return -1;
	}

	loop=1;
	BIGMessage("--- Listening on port %d ---",port);
	while(loop) 
	{
		FD_ZERO(&socket_set);
		FD_SET(listen_socket,&socket_set);
		timeout.tv_sec=1;  // Select has 1 second timeout (to check on exit_status)
		timeout.tv_usec=0;
		ret=select(listen_socket+1,&socket_set,NULL,NULL,&timeout);
		if (ret > 0) 
		{
#ifdef MULTITHREADED			
			DWORD dwThreadId; 
			HANDLE hThread; 
#endif
			struct server_struct *sock;
			fromlen =sizeof(from);
			msgsock = accept(listen_socket,(struct sockaddr*)&from, &fromlen);
			if (msgsock == INVALID_SOCKET) 
			{
				#ifdef __WIN32__
				ret = WSAGetLastError();
				#else
				ret = errno;
				#endif

				Critical("accept() error %d",ret);
				#ifdef __WIN32__
				WSACleanup();
				#endif
				return -1;
			}
			
			sock = malloc(sizeof(struct server_struct));
			sock->sock=msgsock;
			sock->sin_addr=from.sin_addr;
			sock->sin_port=from.sin_port;
			sock->logbuffer[0]=0;

#ifdef MULTITHREADED
			hThread = CreateThread( 
				NULL,                           // no security attributes 
				0,                              // use default stack size  
				(LPTHREAD_START_ROUTINE)server,	// thread function 
				sock,                           // argument to thread function
				0,                              // use default creation flags 
				&dwThreadId);                   // returns the thread identifier 
#else
			server(sock);
#endif
		}
	}

        // Stop listening asap

#ifdef __WIN32__
	closesocket(listen_socket);
#else
	close(listen_socket);
#endif

#ifdef MULTITHREADED
        // OK, now wait for threads to complete...
/*        while ()
        {
        }*/
#endif

#ifdef __WIN32__
        // Turn off WinSock
	WSACleanup();
#endif

	return 0;
}
