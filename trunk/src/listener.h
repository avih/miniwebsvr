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
#ifndef LISTENER_H
#define LISTENER_H

extern int loop;

int listener(char *interface, unsigned short port);

#ifdef THREAD_POOL
#ifndef __WIN32__
void* worker(int n);

#ifdef SERVER_H // i need server_struct definition here

int push_request(struct server_struct* request);
struct server_struct* pop_request();
#endif // SERVER_H
#endif // __WIN32__
#endif // THREAD_POOL

#endif // LISTENER_H
