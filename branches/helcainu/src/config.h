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
#ifndef CONFIG_H
#define CONFIG_H

/************************* Compile time Limits/Options ***********************/

#define VERSION "MiniWebsvr/0.0.7svn"
#define COPYRIGHT "Copyright (C) 2007  Nickolas Antonie Grigoriadis"
#define EMAIL "grigi_ at users.sourceforge.net"

/// Defines whether the server must run in multiple threads (recommended)
/// MULTITHREADED is synonym for THREAD_POOL
/// Old MULTITHREADED functionality is DEPRECATED now.
#define MULTITHREADED
#define USE_INLINE


/************************ Default for Run-time Options ***********************/

#define DEFAULT_THREAD_POOL_SIZE 8
#define DEFAULT_THREAD_POOL_ADJUST 0

#define THREAD_SPAWN_AT 25
#define THREAD_KILL_AT 15

#define DEFAULT_PORT 8080
#define DEFAULT_INTERFACE NULL
#define DEFAULT_LOGFILE "miniwebsvr.log"
#define DEFAULT_ROOT "."

/******************** Variables containing Run-time Values *******************/

void getconfig(int argc, char **argv);

#ifdef LIB
typedef void (*HOOK_logger)(char*);
typedef void (*HOOK_file)(void*, char*, char*, int);

struct hook_list_struct
{
	HOOK_file hook;
	char* fileext;
	struct hook_list_struct* next;
};

typedef struct hook_list_struct hook_list;
#endif

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
	hook_list* filehooks;
#endif
};

typedef struct server_config_struct server_config;

#endif // CONFIG_H
