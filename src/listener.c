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
#include "os_compat.h"

#include <stdio.h>
#include <malloc.h>

#include "config.h"
#include "server.h"
#include "listener.h"
#include "logging.h"

int loop;

#ifdef THREAD_POOL
#ifdef __WIN32__
#error "THREAD_POOL not implemented for WIN32 targets"
#else
struct server_struct** pool;
pthread_t* thread_pool;

pthread_cond_t new_request = PTHREAD_COND_INITIALIZER;
pthread_cond_t thread_free = PTHREAD_COND_INITIALIZER;

pthread_mutex_t pool_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t thread_pool_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif
#endif

int listener(char *interface, unsigned short port)
{
	struct timeval timeout;// = {1,0}; // 1 second poll state
#ifdef THREAD_POOL
#ifdef __WIN32__
#error "THREAD_POOL not implemented for WIN32 targets"
#else
	struct timespec tp; // stores absolute time for pthread_cond_timedwait
#endif
#endif

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
	setsockopt( listen_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on) );
	// Disable Nagle
	// Since we are implementing our own buffering Nagle just gets in the way.
	setsockopt( listen_socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&on, sizeof(on) );

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
	loop = 1;
#ifdef THREAD_POOL
#ifdef __WIN32__
#error "THREAD_POOL not implemented for WIN32 targets"
#else
	pool = malloc(THREAD_POOL_SIZE*sizeof(struct server_struct));
	thread_pool = malloc(THREAD_POOL_SIZE*sizeof(pthread_t));
	if((pool == NULL)||(thread_pool == NULL))
	{
		DebugMSG("Can't allocate memory for thread pool need");
		return -1;
	}

	memset(pool, 0, sizeof(struct server_struct)*THREAD_POOL_SIZE);
	memset(thread_pool, 0, sizeof(pthread_t)*THREAD_POOL_SIZE);

	size_t i = 0;

	for(i = 0; i < THREAD_POOL_SIZE; i++)
	{
		pthread_create(&thread_pool[i], NULL, (void *(*)(void*))&worker, (void*)i);
		pthread_detach(thread_pool[i]);
	}
#endif

#endif
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
#ifndef THREAD_POOL
			HANDLE hThread;
#ifdef __WIN32__
			DWORD dwThreadId;
#else
			pthread_attr_t attr;
#endif // __WIN32__
#endif // THREAD_POOL
#endif // MULTITHREAD
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

			sock = (struct server_struct*)malloc(sizeof(struct server_struct));
			sock->sock=msgsock;
			sock->sin_addr=from.sin_addr;
			sock->sin_port=from.sin_port;
			sock->logbuffer[0]=0;
#ifdef THREAD_POOL
#ifdef __WIN32__
#error "THREAD_POOL not implemented for WIN32 targets"
#else
			while(!push_request(sock) && loop) // try every second or when "thread_free" fires
			{
				clock_gettime(CLOCK_REALTIME, &tp);
				++tp.tv_sec;		// 1 sec
				pthread_mutex_lock(&thread_pool_mutex);
				pthread_cond_timedwait(&thread_free, &thread_pool_mutex, &tp);
				pthread_mutex_unlock(&thread_pool_mutex);
			}
#endif // __WIN32__
#else
#ifdef MULTITHREADED
#ifdef __WIN32__
			hThread = CreateThread(
				NULL,                           // no security attributes
				0,                              // use default stack size
				(LPTHREAD_START_ROUTINE)server,	// thread function
				sock,                           // argument to thread function
				0,                              // use default creation flags
				&dwThreadId);                   // returns the thread identifier
#else // __WIN32__
			pthread_attr_init(&attr);
			pthread_attr_setdetachstate(&attr,1);
			pthread_create(&hThread,&attr,(void*)server,sock);
#endif // __WIN32__
#else // MULTITHREAD
			server(sock);
#endif // MULTITHREAD
#endif // THREAD_POOL
		}
	}

	// Stop listening asap

#ifdef __WIN32__
	closesocket(listen_socket);
#else
	close(listen_socket);
#endif

#ifdef THREAD_POOL
#ifdef __WIN32__
#error "THREAD_POOL not implemented for WIN32 targets"
#else
	// Send broadcast signal so that threads unblock.
//	pthread_cond_broadcast(&new_request); // you did'nt need it. 2 lines forward you use pthread_cancel, so it stop thread, because pthread_cond_wait is cancellation point.

	// OK, Cancel all threads
	for(i = 0; i < THREAD_POOL_SIZE; i++)
	{
		pthread_cancel(thread_pool[i]);
	}

	// OK, now wait for threads to complete...
	for(i = 0; i < THREAD_POOL_SIZE; i++)
	{
		void* blah;
		pthread_join(thread_pool[i],&blah);
	}

	// No need to sleep, since all threads are done by this time. (pthread_join guarentees it)
	//sleep(2); // 2 seconds are enough to complete request.. or NOT??

	free(pool); // We have to free what we allocate
#endif
#endif

#ifdef __WIN32__
	// Turn off WinSock
	WSACleanup();
#endif

	return 0;
}

#ifdef THREAD_POOL
#ifndef __WIN32__
void* worker(int n)
{
	struct server_struct *request;
	DebugMSG("Worker %d started!", n);
	while(loop) // Global variable of "loop" indicates that server is running, if not set, quit
	{
		pthread_mutex_lock(&pool_mutex); // You should lock mutex before pthread_cond_wait call..
		request = pop_request();
		if (request == NULL) {
			// No more jobs, go to sleep now
			pthread_cond_wait(&new_request, &pool_mutex); // On pthread_cond_signal event pthread_cond_wait lock's mutex via pthread_mutex_lock
			request = pop_request();
		}
		pthread_mutex_unlock(&pool_mutex); // so, you must unlock it.

		if(request != NULL)
			server(request);
		pthread_cond_signal(&thread_free);
	}

	pthread_exit(NULL);
	return NULL;
}

int push_request(struct server_struct* request)
{
	unsigned int i;
	int added = 0;

	pthread_mutex_lock(&pool_mutex);
	for(i = 0; (i < THREAD_POOL_SIZE)&&(!added); i++)
	{
		if(pool[i] == NULL)
		{
			pool[i] = request;
			added = 1;
		}
	}
	pthread_mutex_unlock(&pool_mutex);
	if (added)
		pthread_cond_signal(&new_request);
	return added;
}

struct server_struct* pop_request()
{
	unsigned int i;
	struct server_struct *request = NULL;
	for(i = 0; (i < THREAD_POOL_SIZE)&&(request == NULL); i++)
	{
		if(pool[i] != NULL)
		{
			request = pool[i];
			pool[i] = NULL;
		}
	}
	return request;
}
#endif
#endif
