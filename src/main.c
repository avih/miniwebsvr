/*  Miniweb - A small webserver
    Copyright (C) 2007  Nickolas Antonie Grigoriadis
    E-Mail: nagrigoriadis@gmail.com

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
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include "config.h"
#include "listener.h"
#include "logging.h"

void cleanup()
{
	WSACleanup();
	BIGMessage("--- %s terminated cleanly ---",VERSION);
	StopLogging();
}

void catch_term(int sig) {
	switch (sig) {
	case SIGINT   : BIGMessage("Caught signal SIGINT");break;
	case SIGTERM  : BIGMessage("Caught signal SIGTERM");break;
	case SIGBREAK : BIGMessage("Caught signal SIGBREAK");break;
	default       : BIGMessage("Caught signal %d",sig);
	}
	loop=0;
	signal(sig,SIG_ACK);
}

int main(int argc, char **argv) {
	getconfig();
	
	if (atexit(&cleanup)) {
		fprintf(stderr, "cannot set exit function\n");
		return EXIT_FAILURE;
	}
	signal (SIGINT, &catch_term);
	signal (SIGTERM, &catch_term);
	signal (SIGBREAK, &catch_term);
	
	StartLogging(LOGFILE);
	BIGMessage("--- %s starting ---",VERSION);
	BIGMessage("--- This software is distributed under the GNU Lesser General Public License");
	BIGMessage("--- Copyright (C) 2007  Nickolas Antonie Grigoriadis"); 
	BIGMessage("--- E-Mail: nagrigoriadis@gmail.com");
	return listener(INTERFACE,PORT);
}
