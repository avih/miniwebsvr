/*  MiniWebSvr - A small webserver
    Copyright (c) 2009 Daniel John Walker and Nickolas Antonie Grigoriadis
    E-Mail: djwalker at users.sourceforge.net and grigi_ at users.sourceforge.net

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
#ifndef CONFIG_H
#define CONFIG_H

/************************* Compile time Limits/Options ***********************/

#ifndef VERSION
        #define VERSION "MiniWebSvr/0.0.9svn"
#endif
#define COPYRIGHT "Copyright (C) 2007  Nickolas Antonie Grigoriadis"
#define EMAIL "grigi_ at users.sourceforge.net"

/// Defines whether the server must run in multiple threads (recommended)
#define MULTITHREADED
#if ! __BSD_VISIBLE
#define USE_INLINE
#endif

/// Defines if a Thread Pool is to be used (recommended)
#ifdef MULTITHREADED
#ifndef __WIN32__
#define THREAD_POOL
#endif
#endif

/************************ Default for Run-time Options ***********************/

#define DEFAULT_THREAD_POOL_SIZE 8
#define DEFAULT_PORT 8080
#define DEFAULT_NODIRLIST 0
#define DEFUALT_DOLOG 1
#define DEFAULT_LISTSERVER 1
#define DEFAULT_INTERFACE NULL
#define DEFAULT_LOGFILE "miniwebsvr.log"
#define DEFAULT_ROOT "."
#define DEFAULT_DEFAULTFILE "index.html"

/******************** Variables containing Run-time Values *******************/

#ifdef THREAD_POOL
extern unsigned int THREAD_POOL_SIZE;
#endif
extern int PORT;
extern int DOLOG;
extern int NODIRLIST;
extern int LISTSERVER;
extern char* INTERFACE;
extern char* LOGFILE;
extern char* ROOT;
extern char* DEFAULTFILE;
//extern char MiniWebSvrlogo_data[2316];

void getconfig(int argc, char **argv);

#endif // CONFIG_H
