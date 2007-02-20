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

#ifndef LIBMINIWEBSVR_H
#define LIBMINIWEBSVR_H
#define LIB

typedef void (*HOOK_logger)(char*);


struct server_config_struct
{	
	int port;
	char* interface;
	char* logfile;
	char* root;

#ifdef MULTITHREADED
	int thread_pool_size;
	int thread_pool_adjust;
#endif
#ifdef LIB
	HOOK_logger logger_hook;
#endif
};

typedef struct server_config_struct server_config;


#endif

