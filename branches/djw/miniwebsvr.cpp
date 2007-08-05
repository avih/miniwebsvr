/*  MiniWebSvr - A small webserver
    Copyright (C) 2007  Daniel John Walker and Nickolas Antonie Grigoriadis
    E-Mail: djwalker at users.sourceforge.net and grigi_ at users.sourceforge.net

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
#include "miniwebsvr.h"
#include "os_compat.h"

#include <cstdio>
#include <dirent.h>
#include <sys/stat.h>
#include <malloc.h>
#include <windows.h>

// the following defines should be moved into a config.h file
#define MULTITHREADED
//#define THREAD_POOL

#define SERVER_BUFFER_SIZE 2048


#if defined(THREAD_POOL) && !defined(MULTITHREADED)
#warning "THREAD_POOL requires that MULTITHREADED is defined. THREAD_POOL will be disabled."
#undef THREAD_POOL
#endif
#if defined(THREAD_POOL) && defined(__WIN32__)
#warning "THREAD_POOL is not implemented for WIN32 targets. THREAD_POOL will be disabled."
#undef THREAD_POOL
#endif


namespace
{
	struct Server
	{
		miniwebsvr_connection_callback *connection_callback;
		void *connection_callback_param;
		miniwebsvr_logger *logger;
		void *logger_param;
		bool abort_when_no_connections;
		bool refuse_connections;
		int num_connections;
		SOCKET socket;
	};
	struct Connection
	{
		Server *server;
		SOCKET socket;
		u_short port;
		struct in_addr addr;

		char buffer[SERVER_BUFFER_SIZE];
		unsigned int buffer_pos;
		ssize_t buffer_size;
	};

	#ifdef __WIN32__
	bool wsa_started = false;
	#endif

	void handle_connection(Connection *connection)
	{
		connection->buffer_pos = 0;
		connection->buffer_size = 0;
		connection->server->connection_callback(connection->server->connection_callback_param, connection);
		closesocket(connection->socket);
		connection->server->num_connections -= 1;
#if defined(MULTITHREADED) && !defined(THREAD_POOL)
		free(connection);
#endif
	}
	int server_charpos(const struct Connection *connection, const char chr)
	{
		int i;

		for (i = connection->buffer_pos; i < connection->buffer_size; ++i)
		{
			if (connection->buffer[i] == chr)
				return i;
		}
		return -1;
	}
}

int miniwebsvr_listen(int port, const char *interface, miniwebsvr_connection_callback *connection_callback, void *connection_callback_param, miniwebsvr_logger *logger, void *logger_param)
{
	Server server;
	server.logger = logger;
	server.logger_param = logger_param;
	server.connection_callback = connection_callback;
	server.connection_callback_param = connection_callback_param;
	server.abort_when_no_connections = false;
	server.refuse_connections = false;
	server.num_connections = 0;

	struct timeval timeout;// = {1,0}; // 1 second poll state

#if defined(MULTITHREADED) && defined(THREAD_POOL) && !defined(__WIN32__)
	struct timespec tp; // stores absolute time for pthread_cond_timedwait
#endif

	int ret,on;
	socklen_t fromlen;
	struct sockaddr_in local, from;
	SOCKET listen_socket, msgsock;
	fd_set socket_set;

#ifdef __WIN32__
	if (!::wsa_started)
	{
		WSADATA wsaData;

		ret = WSAStartup(0x202,&wsaData); // Request WinSock API v2.2
		atexit((void(*)())WSACleanup);
		if (ret != 0)
		{
			char message[45];
			snprintf(message, 45, "Critical: WSAStartup failed with error %d",ret);
			server.logger(server.logger_param, message);
			return -1;
		}
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
		char message[45];
		snprintf(message, 45, "Critical: socket() failed with error %d", SOCKETS_ERRNO);
		server.logger(server.logger_param, message);
		return -1;
	}

	if (bind(listen_socket,(struct sockaddr*)&local,sizeof(local)) == SOCKET_ERROR)
	{
		char message[45];
		snprintf(message, 45, "Critical: bind() failed with error %d", SOCKETS_ERRNO);
		server.logger(server.logger_param, message);
		return -1;
	}

	if (listen(listen_socket,SOMAXCONN) == SOCKET_ERROR) // Listen with backlog set to max reasonable by OS
	{
		char message[45];
		snprintf(message, 45, "Critical: listen() failed with error %d", SOCKETS_ERRNO);
		server.logger(server.logger_param, message);
		return -1;
	}

#if defined(MULTITHREADED) && defined(THREAD_POOL) && !defined(__WIN32__)
	pool = malloc(THREAD_POOL_SIZE*sizeof(struct server_struct));
	thread_pool = malloc(THREAD_POOL_SIZE*sizeof(pthread_t));
	if((pool == NULL)||(thread_pool == NULL))
	{
		server.logger(server.logger_param, "Critical: failed to allocate memory for a thread pool");
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
#endif // defined(MULTITHREADED) && defined(THREAD_POOL) && !defined(__WIN32__)

	{
		char message[45];
		snprintf(message, 45, "--- Listening on port %d ---",port);
		server.logger(server.logger_param, message);
	}
	while(!server.abort_when_no_connections || server.num_connections > 0)
	{
		FD_ZERO(&socket_set);
		FD_SET(listen_socket,&socket_set);
		timeout.tv_sec=1;  // Select has 1 second timeout (to check on exit_status)
		timeout.tv_usec=0;

		ret=select(listen_socket+1,&socket_set,NULL,NULL,&timeout);
		if (ret > 0)
		{
			fromlen = sizeof(from);
			msgsock = accept(listen_socket,(struct sockaddr*)&from, &fromlen);
			if (msgsock == INVALID_SOCKET)
			{
				char message[45];
				snprintf(message, 45, "Warning: accept() failed with error %d", SOCKETS_ERRNO);
				server.logger(server.logger_param, message);
				continue;
			}

			if (server.refuse_connections)
			{
				closesocket(msgsock);
				continue;
			}

#ifdef MULTITHREADED
#if defined(THREAD_POOL) && !defined(__WIN32__)
			while(!push_request(sock) && loop) // try every second or when "thread_free" fires
			{
				clock_gettime(CLOCK_REALTIME, &tp);
				++tp.tv_sec;		// 1 sec
				pthread_mutex_lock(&thread_pool_mutex);
				pthread_cond_timedwait(&thread_free, &thread_pool_mutex, &tp);
				pthread_mutex_unlock(&thread_pool_mutex);
			}
#elif defined(__WIN32__)
			HANDLE hThread;
			DWORD dwThreadId;

			Connection *new_connection = (Connection*)malloc(sizeof(Connection));
			new_connection->server = &server;
			new_connection->socket = msgsock;
			new_connection->addr = from.sin_addr;
			new_connection->port = from.sin_port;
			server.num_connections += 1;

			hThread = CreateThread(
				NULL,                           // no security attributes
				0,                              // use default stack size
				(LPTHREAD_START_ROUTINE)&handle_connection,	// thread function
				new_connection,                 // argument to thread function
				0,                              // use default creation flags
				&dwThreadId);                   // returns the thread identifier
			CloseHandle(hThread);	// close the handle so the the thread will be removed once it has exited
#else // if !defined(THREAD_POOL) && !defined(__WIN32__)
			HANDLE hThread;
			pthread_attr_t attr;

			Connection *new_connection = (Connection*)malloc(sizeof(Connection));
			new_connection->server = &server;
			new_connection->socket = msgsock;
			new_connection->addr = from.sin_addr;
			new_connection->port = from.sin_port;
			server.num_connections += 1;

			pthread_attr_init(&attr);
			pthread_attr_setdetachstate(&attr,1);
			pthread_create(&hThread,&attr,(void*)handle_connection, new_connection);//!TODO
#endif // defined(THREAD_POOL) && !defined(__WIN32__)
#else // if !defined(MULTITHREADED)
			Connection new_connection;
			new_connection.server = &server;
			new_connection.socket = msgsock;
			new_connection.addr = from.sin_addr;
			new_connection.port = from.sin_port;
			server.num_connections += 1;
			handle_connection(&new_connection);
#endif // MULTITHREADED
		}
	}

	// Stop listening asap
	closesocket(listen_socket);

#if defined(MULTITHREADED) && defined(THREAD_POOL) && !defined(__WIN32__)
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

	return 0;
}


void miniwebsvr_close_server(miniwebsvr_connection connection)
{
	Server *server = ((Connection *)connection)->server;
	server->refuse_connections = true;
	server->abort_when_no_connections = true;
}


void miniwebsvr_write_log(miniwebsvr_connection connection, const char *message)
{
	Server *server = ((Connection *)connection)->server;
	server->logger(server->logger_param, message);
}

void miniwebsvr_send(miniwebsvr_connection a_connection, const char *message, int length)
{
	Connection *connection = (Connection*)a_connection;
	send(connection->socket, message, length, 0);
}

int miniwebsvr_read_line(miniwebsvr_connection a_connection, char *str, int strsize)
{
	Connection *connection = (Connection *)a_connection;
	int strpos;
	int retval;

	// Check if in the buffer
	if ((strpos = server_charpos(connection,'\n')) != -1)
	{
		// Whole string is in buffer
		strpos -= connection->buffer_pos+1;

		// Copy string to given buffer
		if (strpos > strsize)
			strpos=strsize;
		memcpy(str,connection->buffer+connection->buffer_pos,strpos);
		connection->buffer_pos+=strpos+2;
	}
	else
	{
		// Copy remaining data in buffer
		strpos=connection->buffer_size-connection->buffer_pos;
		if (strpos > strsize)
			strpos=strsize;
		memcpy(str,connection->buffer+connection->buffer_pos,strpos);

		// Assemble rest of string out of multiple buffers
		while (strpos < strsize)
		{
			// Get data from stream
			connection->buffer_pos=0;
			connection->buffer_size = recv(connection->socket,connection->buffer,SERVER_BUFFER_SIZE,0);

			if (connection->buffer_size == SOCKET_ERROR)
			{
				#ifdef __WIN32__
				retval = WSAGetLastError();
				#else
				retval = errno;
				#endif

				fprintf(stderr,"recv() failed: error %d\n",retval);
				//server_close(connection);
				return -1;
			}

			retval=server_charpos(connection,'\n');
			if ((retval == -1) && ((strpos+connection->buffer_size) > strsize))
				retval=strsize-strpos; // Limit check exceeded
			if (retval == -1)
			{
				// Consume whole buffer
				connection->buffer_pos=connection->buffer_size;
				memcpy(str+strpos,connection->buffer,connection->buffer_size);
				strpos+=connection->buffer_size;
			}
			else
			{
				// Partial buffer
				if (retval > (int)(strsize - strpos))
					retval = strsize - strpos; // Limit check exceeded
				connection->buffer_pos=retval+1; // +1 to skip the NULL terminator of the string
				--retval; // -1 to stop before the detected endline
				if (retval>0)
					memcpy(str+strpos,connection->buffer,retval);
				strpos+=retval;
				break;
			}

			// Don't expect a full buffer to allow interactive (non-buffering) connections.
			// No data indicates some connection error, so quit.
			if (connection->buffer_size == 0) break;
		}
	}

	// remove \r if present just before the \n
	if (str[strpos] == '\r')
		--strpos;

	// Set the NULL terminator for the string
	++strpos;
	if (strpos > strsize)
		strpos=strsize;
	str[strpos-1]=0;
	return strpos-1;
}

