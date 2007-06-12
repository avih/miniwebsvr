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

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#ifdef __WIN32__
#include <dir.h>
#endif

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

// As documented at http://www.bindview.com/Services/Razor/Papers/2001/signals.cfm
// There exists the possibility of a race condition if 2 or more different
// signals are sent to the process nearly the same time when using the same
// signal handler for different signals.
void catch_int(int sig)
{
	BIGMessage("Caught signal SIGINT");
	loop=0;
	signal(sig,SIG_IGN);
}

void catch_term(int sig)
{
	BIGMessage("Caught signal SIGTERM");
	loop=0;
	signal(sig,SIG_IGN);
}

#ifdef __WIN32__
void catch_break(int sig)
{
	BIGMessage("Caught signal SIGBREAK");
	loop=0;
	signal(sig,SIG_IGN);
}
#else
void catch_hup(int sig)
{
	BIGMessage("Caught signal SIGHUP");
	loop=0;
	signal(sig,SIG_IGN);
}

void catch_quit(int sig)
{
	BIGMessage("Caught signal SIGQUIT");
	loop=0;
	signal(sig,SIG_IGN);
}
#endif

#ifdef USEWINMAIN
#include <shellapi.h>

char **argv = NULL;
	
/*******************************************************
WIN32 command line parser function
********************************************************/
int ParseCommandline()
{
	int    argc, BuffSize, i;
	WCHAR  *wcCommandLine;
	LPWSTR *argw;
	
	// Get a WCHAR version of the parsed commande line
	wcCommandLine = GetCommandLineW();	
	argw = CommandLineToArgvW( wcCommandLine, &argc);

	// Create the first dimension of the double array
	argv = (char **)GlobalAlloc( LPTR, argc + 1 );
	
	// convert each line of wcCommandeLine to MultiByte and place them
	// to the argv[] array
	for( i=0; i < argc; i++)
	{
		BuffSize = WideCharToMultiByte( CP_ACP, WC_COMPOSITECHECK, argw[i], -1, NULL, 0, NULL, NULL );
		argv[i] = (char *)GlobalAlloc( LPTR, BuffSize );		
		WideCharToMultiByte( CP_ACP, WC_COMPOSITECHECK, argw[i], BuffSize * sizeof( WCHAR ) ,argv[i], BuffSize, NULL, NULL );
	}

	LocalFree(argw);

	// return the number of argument
	return argc;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int argc = ParseCommandline( );
#else
int main(int argc, char **argv)
{
#endif

	getconfig(argc, argv);

	if (atexit(&cleanup))
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
	chdir(ROOT);
	return listener(INTERFACE,PORT);
}
