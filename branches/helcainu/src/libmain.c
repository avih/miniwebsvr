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

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>

#include "config.h"

#include "listener.h"
#include "logging.h"

void cleanup()
{
#ifdef __WIN32__
	WSACleanup();
#endif
	BIGMessage("--- %s terminated cleanly ---",VERSION);
	StopLogging();
}

server_config config;

pthread_t server_start()
{
//	getconfig(argc, argv);
	
/*	if (atexit(&cleanup)) 
	{
		fprintf(stderr, "cannot set exit function\n");
		return EXIT_FAILURE;
	}
	signal (SIGINT, &catch_int);
	signal (SIGTERM, &catch_term);
#ifdef __WIN32__
	signal (SIGBREAK, &catch_break);
#else
	signal (SIGHUP, &catch_hup);
	signal (SIGQUIT, &catch_quit);
#endif
	
	StartLogging(LOGFILE);
	BIGMessage("--- %s starting ---",VERSION);
	BIGMessage("--- This software is distributed under the GNU Lesser General Public License");
	BIGMessage("--- %s",COPYRIGHT); 
	BIGMessage("--- E-Mail: %s",EMAIL);

// chdir to the document root

	chdir(ROOT);*/
#ifdef __WIN32__
#else
	if(config.logger_hook == &mwb_Log)
	{
		StartLogging(config.logfile);
	}
	return pthread_create(NULL, NULL, (void*)listener(), NULL);
#endif
}
