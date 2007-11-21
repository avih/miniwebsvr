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
#include <sys/stat.h>

#include "config.h"
#include "logging.h"

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

#define arg(x) (i==argc?"":argv[x])

int sar(char *str,char *chr)
{
	int in=0;

	if (str[0] == '/')
		in=1;
	if (str[0] == '-')
	{
		if (str[1] == '-')
			in=2;
		else
			in=1;
	}

	if (in && (0==strcmp(str+in,chr)))
		return 1;

	return 0;
}

void getconfig(int argc, char **argv)
{
	int i;
	struct structstat statbuf;
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
			if ((sar(arg(i), "port")) || (sar(arg(i),"p")))
				next = next_port;
			else if ((sar(arg(i), "log")) || (sar(arg(i),"l")))
				next = next_logfile;
			else if ((sar(arg(i), "root")) || (sar(arg(i),"r")))
				next = next_root;
			else if ((sar(arg(i), "interface")) || (sar(arg(i),"i")))
				next = next_interface;
			else if ((sar(arg(i), "nolog")) || (sar(arg(i),"nl")))
				DOLOG = 0;
			else if ((sar(arg(i), "nodirlist")) || (sar(arg(i),"nd")))
				NODIRLIST = 1;
			else if ((sar(arg(i), "default")) || (sar(arg(i),"d")))
				next = next_defaultfile;
			else if ((sar(arg(i), "noname")) || (sar(arg(i),"nn")))
				LISTSERVER = 0;
#ifdef THREAD_POOL
			else if (sar(arg(i), "threads"))
				next = next_threads;
#endif
			else if ((sar(arg(i), "help")) || (sar(arg(i),"?")) || (sar(arg(i),"h")))
			{
				#ifdef USEWINMAIN
				char buf[8192];
				snprintf(buf,8191,
				#else
				printf(
				#endif
					"Usage: %s [options] (%s)\nOptions:\n" 
					" -h, /?, --help           Display this information\n" 
					" -p, --port <port>        Listen on port <port> (default %d)\n" 
					" -i, --interface <ip>     Specify the interface the server listens on (default: ALL)\n" 
					" -l, --log <file>         Save the log file as <file> (default: %s)\n" 
					" -nl, --nolog             Disables logging, overrides any '--log' setting\n" 
					" -r, --root <path>        Specify the document root directory (default: %s)\n" 
					" -d, --default <filename> Specify the default document filename in a directory (default: %s)\n" 
					" -nd, --nodirlist         Do not do any directory listings, just return a '404 File not found'\n" 
					" -nn, --noname            Do not specify server name in directory listings or HTTP headers\n" 
#ifdef THREAD_POOL
					" --threads <thread_nos>   Specify number of threads in thread pool (default %d)\n" 
#endif
					"",argv[0],VERSION,DEFAULT_PORT,DEFAULT_LOGFILE,DEFAULT_ROOT,DEFAULT_DEFAULTFILE
#ifdef THREAD_POOL
					,DEFAULT_THREAD_POOL_SIZE
#endif
					);
				#ifdef USEWINMAIN
				buf[8191]=0;
				MessageBox(NULL, buf, "Usage", MB_ICONINFORMATION);
				#endif
				exit(0);
			}
			else
			{
				Critical("Unknown parameter \"%s\". Type \"%s --help\" for more info.\n",arg(i),argv[0]);
				exit(0);
			}

			if (next != next_param)
				i += 1;
		}
		
		if (next == next_logfile)
			LOGFILE = arg(i);
		else if (next == next_port)
		{
			PORT = atoi(arg(i));
			if ((PORT<1) || (PORT>65535)) {
				Critical("Bad port \"%s\". Must be integer from 1 to 65535.\n",arg(i));
				exit(0);
			}
		}
		else if (next == next_root)
		{
			ROOT = arg(i);
			if (!((0 == funcstat(ROOT, &statbuf)) && (statbuf.st_mode & S_IFDIR)))
			{
				Critical("Invalid root \"%s\". Must be a valid directory.\n",ROOT);
				exit(0);
			}
		}
		else if (next == next_interface)
			INTERFACE = arg(i);
		else if (next == next_defaultfile)
			DEFAULTFILE = arg(i);
#ifdef THREAD_POOL
		else if (next == next_threads)
		{
			THREAD_POOL_SIZE = atoi(arg(i));
			if ((THREAD_POOL_SIZE<1) || (THREAD_POOL_SIZE>32)) {
				Critical("Bad thread pool size \"%s\". Must be positive integer from 1 to 32.\n",arg(i));
				exit(0);
			}
		}
#endif
		next = next_param;
	}
}

