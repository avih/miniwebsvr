/*  MiniWebSvr - A small webserver
    Copyright (c) 2009 Nickolas Antonie Grigoriadis
    E-Mail: grigi_ at users.sourceforge.net

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/
#include "os_compat.h"

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#ifdef __WIN32__
#include <direct.h>
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
		#ifdef USEWINMAIN
		MessageBox(NULL, "Cannot set exit function", "Critical", MB_ICONERROR);
		#else
		fprintf(stderr, "Critical: Cannot set exit function\n");
		#endif
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
	_chdir(ROOT);
	return listener(INTERFACE,PORT);
}
