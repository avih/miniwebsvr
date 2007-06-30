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
#include "os_compat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

#ifdef THREAD_POOL
unsigned int THREAD_POOL_SIZE;
#endif
int PORT;
int DOLOG;
int NODIRLIST;
int LISTSERVER;
char* INTERFACE;
char* LOGFILE;
char* ROOT;
char* DEFAULTFILE;

int sar(char *str,char chr)
{
	if (strlen(str)!=2) 
		return 0;
	if ((str[0] == '-') || (str[0] == '/'))
	{
		if (str[1] == chr)
			return 1;
	}
	return 0;
}

void getconfig(int argc, char **argv)
{
	int i;
	enum { next_param, next_port, next_logfile, next_root, next_interface, next_threads, next_defaultfile } next = next_param;

	PORT=DEFAULT_PORT;
	INTERFACE=DEFAULT_INTERFACE;
	LOGFILE=DEFAULT_LOGFILE;
	ROOT=DEFAULT_ROOT;
	DOLOG=DEFUALT_DOLOG;
	NODIRLIST=DEFAULT_NODIRLIST;
	DEFAULTFILE=DEFAULT_DEFAULTFILE;
	LISTSERVER=DEFAULT_LISTSERVER;
#ifdef THREAD_POOL
	THREAD_POOL_SIZE=DEFAULT_THREAD_POOL_SIZE;
#endif

	for (i = 1; i < argc; i += 1)
	{
		if (next == next_param)
		{
			if ((0 == strcmp(argv[i], "--port")) || (sar(argv[i],'p')))
				next = next_port;
			else if ((0 == strcmp(argv[i], "--log")) || (sar(argv[i],'l')))
				next = next_logfile;
			else if ((0 == strcmp(argv[i], "--root")) || (sar(argv[i],'r')))
				next = next_root;
			else if ((0 == strcmp(argv[i], "--interface")) || (sar(argv[i],'i')))
				next = next_interface;
			else if (0 == strcmp(argv[i], "--nolog"))
				DOLOG = 0;
			else if (0 == strcmp(argv[i], "--nodirlist"))
				NODIRLIST = 1;
			else if ((0 == strcmp(argv[i], "--default")) || (sar(argv[i],'d')))
				next = next_defaultfile;
			else if (0 == strcmp(argv[i], "--noname"))
				LISTSERVER = 0;
#ifdef THREAD_POOL
			else if (0 == strcmp(argv[i], "--threads"))
				next = next_threads;
#endif
			else if ((0 == strcmp(argv[i], "--help")) || (sar(argv[i],'?')) | (sar(argv[i],'h')))
			{
				printf("Usage: %s [options] (%s)\nOptions:\n", argv[0],VERSION);
				printf("  -h, /?, --help           Display this information\n");
				printf("  -p, --port <port>        Listen on port <port> (default %d)\n",DEFAULT_PORT);
				printf("  -i, --interface <ip>     Specify the interface the server listens on (default: ALL)\n");

				printf("  -l, --log <file>         Save the log file as <file> (default: %s)\n",DEFAULT_LOGFILE);
				printf("  --nolog                  Disables logging, overrides any '--log' setting\n");

				printf("  -r, --root <path>        Specify the document root directory (default: %s)\n",DEFAULT_ROOT);
				printf("  -d, --default <filename> Specify the default document filename in a directory (default: %s)\n",DEFAULT_DEFAULTFILE);
				printf("  --nodirlist              Do not do any directory listings, just return a '404 File not found'\n");
				printf("  --noname                 Do not specify servername in directory listings or HTTP headers\n");
#ifdef THREAD_POOL
				printf("  --threads <thread_nos>   Specify number of threads in thread pool (default %d)\n",DEFAULT_THREAD_POOL_SIZE);
#endif
				exit(0);
			}
			else
			{
				printf("Unknown parameter \"%s\". Type \"%s --help\" for more info.\n",argv[i],argv[0]);
				exit(0);
			}
			continue;
		}
		if (next == next_logfile)
			LOGFILE = argv[i];
		else if (next == next_port)
		{
			PORT = atoi(argv[i]);
			if ((PORT<1) || (PORT>65535)) {
				printf("Bad port \"%s\". Must be integer from 1 to 65535.\n",argv[i]);
				exit(0);
			}
		}
		else if (next == next_root)
			ROOT = argv[i];
		else if (next == next_interface)
			INTERFACE = argv[i];
		else if (next == next_defaultfile)
			DEFAULTFILE = argv[i];
#ifdef THREAD_POOL
		else if (next == next_threads)
		{
			THREAD_POOL_SIZE = atoi(argv[i]);
			if ((THREAD_POOL_SIZE<1) || (THREAD_POOL_SIZE>32)) {
				printf("Bad thread pool size \"%s\". Must be positive integer from 1 to 32.\n",argv[i]);
			}
		}
#endif
		next = next_param;
	}
}
