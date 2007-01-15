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
#ifndef CONFIG_H
#define CONFIG_H

/************************* Compile time Limits/Options ***********************/

#define VERSION "MiniWebsvr/0.05svn"
#define COPYRIGHT "Copyright (C) 2007  Nickolas Antonie Grigoriadis"
#define EMAIL "nagrigoriadis@gmail.com"

/// Defines whether the server must run in multiple threads (recommended)
#define MULTITHREADED
#define USE_INLINE

/************************ Default for Run-time Options ***********************/

#define DEFAULT_PORT 8080
#define DEFAULT_INTERFACE NULL
#define DEFAULT_LOGFILE "miniwebsvr.log"
#define DEFAULT_ROOT "."

/******************** Variables containing Run-time Values *******************/

extern int PORT;
extern char* INTERFACE;
extern char* LOGFILE;
//CHANGE global variable for document root
extern char* ROOT;

void getconfig(int argc, char **argv);

#endif // CONFIG_H
