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
#include <malloc.h>

#include "config.h"
#include "listener.h"
#include "server.h"
#include "logging.h"

int loop;

int listener(char *interface, unsigned short port) 
{
	const struct timeval timeout = {1,0}; // 1 second poll state

	int ret,fromlen;
	struct sockaddr_in local, from;
	WSADATA wsaData;
	SOCKET listen_socket, msgsock;
	fd_set socket_set;

	if ((ret=WSAStartup(0x202,&wsaData)) != 0) // Request WinSock API v2.2
	{
		Critical("WSAStartup failed with error %d",ret);
		WSACleanup();
		return -1;
	}
	
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = (!interface)?INADDR_ANY:inet_addr(interface); 

	local.sin_port = htons(port);

	listen_socket = socket(AF_INET, SOCK_STREAM,0); // TCP socket
	
	if (listen_socket == INVALID_SOCKET)
	{
		Critical("socket() failed with error %d",WSAGetLastError());
		WSACleanup();
		return -1;
	}

	if (bind(listen_socket,(struct sockaddr*)&local,sizeof(local)) == SOCKET_ERROR) 
	{
		Critical("bind() failed with error %d",WSAGetLastError());
		WSACleanup();
		return -1;
	}

	if (listen(listen_socket,SOMAXCONN) == SOCKET_ERROR) // Listen with backlog set to max reasonable by OS
	{
		
		Critical("listen() failed with error %d",WSAGetLastError());
		WSACleanup();
		return -1;
	}

	loop=1;
	BIGMessage("--- Listening on port %d ---",port);
	while(loop) 
	{
		socket_set.fd_count=1;
		socket_set.fd_array[0]=listen_socket;
		ret=select(0,&socket_set,NULL,NULL,&timeout);
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
				Critical("accept() error %d",WSAGetLastError());
				WSACleanup();
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
	
	closesocket(listen_socket);
	WSACleanup();
	return 0;
}
