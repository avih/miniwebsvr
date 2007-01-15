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
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"

int PORT;
char* INTERFACE;
char* LOGFILE;
char* ROOT;

/// TODO: read configuration from registry
void getconfig(int argc, char **argv) {
	int i;
	enum { next_param, next_port, next_logfile, next_root } next = next_param;

	PORT=DEFAULT_PORT;
	INTERFACE=DEFAULT_INTERFACE;
	LOGFILE=DEFAULT_LOGFILE;
	ROOT=DEFAULT_ROOT;

//CHANGE now handles --root parameter
	for (i = 1; i < argc; i += 1)
	{
		if (next == next_param)
		{
			if (0 == strcmp(argv[i], "--help"))
			{
				printf("Usage: %s [options]\nOptions:\n", argv[0]);
				printf("  --help                   Display this information\n");
				printf("  --port <port>            Listen on port <port> (default 8080)\n");
				printf("  --log <file>             Save the log file as <file> (default: miniweb.log)\n");
				printf("  --root <path>            Specify the document root directory\n");
				exit(0);
			}
			else if (0 == strcmp(argv[i], "--port"))
				next = next_port;
			else if (0 == strcmp(argv[i], "--log"))
				next = next_logfile;
			else if (0 == strcmp(argv[i], "--root"))
				next = next_root;
			continue;
		}
		if (next == next_logfile)
			LOGFILE = argv[i];
		else if (next == next_port)
			PORT = atoi(argv[i]);
		else if (next == next_root)
			ROOT = argv[i];
		next = next_param;
	}
}

