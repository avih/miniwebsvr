/*  MiniWebsvr - A small webserver
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
#include "os_compat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

#ifdef THREAD_POOL
int THREAD_POOL_SIZE;
int THREAD_POOL_ADJUST;
#endif

int PORT;
char* INTERFACE;
char* LOGFILE;
char* ROOT;

void getconfig(int argc, char **argv) 
{
	int i;
	enum { next_param, next_port, next_logfile, next_root, next_interface, next_threads, next_threads_adjust } next = next_param;

	PORT=DEFAULT_PORT;
	INTERFACE=DEFAULT_INTERFACE;
	LOGFILE=DEFAULT_LOGFILE;
	ROOT=DEFAULT_ROOT;
#ifdef THREAD_POOL
        THREAD_POOL_SIZE=DEFAULT_THREAD_POOL_SIZE;
	THREAD_POOL_ADJUST=DEFAULT_THREAD_POOL_ADJUST;
#endif

	for (i = 1; i < argc; i += 1)
	{
		if (next == next_param)
		{
			if (0 == strcmp(argv[i], "--help"))
			{
				printf("Usage: %s [options]\nOptions:\n", argv[0]);
				printf("  --help                   Display this information\n");
				printf("  --port <port>            Listen on port <port> (default %d)\n",DEFAULT_PORT);
				printf("  --log <file>             Save the log file as <file> (default: %s)\n",DEFAULT_LOGFILE);
				printf("  --root <path>            Specify the document root directory (default: %s)\n",DEFAULT_ROOT);
				printf("  --interface <ip>         Specify the interface the server listens on (default: ALL)\n");
#ifdef THREAD_POOL
                                printf("  --threads <thread_nos>   Specify number of threads in thread pool (default %d)\n",DEFAULT_THREAD_POOL_SIZE);
				printf("  --threads-adjust <num>   Specify number of threads that can be spawned under heavy load (default %d)\n",DEFAULT_THREAD_POOL_ADJUST);
#endif
				exit(0);
			}
			else if (0 == strcmp(argv[i], "--port"))
				next = next_port;
			else if (0 == strcmp(argv[i], "--log"))
				next = next_logfile;
			else if (0 == strcmp(argv[i], "--root"))
				next = next_root;
			else if (0 == strcmp(argv[i], "--interface"))
				next = next_interface;
#ifdef THREAD_POOL
                        else if (0 == strcmp(argv[i], "--threads"))
                                next = next_threads;
			else if (0 == strcmp(argv[i], "--threads-adjust"))
				next = next_threads_adjust;
#endif
			continue;
		}
		if (next == next_logfile)
			LOGFILE = argv[i];
		else if (next == next_port)
			PORT = atoi(argv[i]);
		else if (next == next_root)
			ROOT = argv[i];
		else if (next == next_interface)
			INTERFACE = argv[i];
#ifdef THREAD_POOL
		else if (next == next_threads)
			THREAD_POOL_SIZE = atoi(argv[i]);
		else if (next == next_threads_adjust)
			THREAD_POOL_ADJUST = atoi(argv[i]);
#endif
		next = next_param;
	}
}

