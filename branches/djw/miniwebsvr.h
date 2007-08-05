/*  MiniWebSvr - A small webserver
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
#ifndef MINIWEBSVR_H
#define MINIWEBSVR_H

#ifdef __cplusplus
extern "C" {
#endif

// PART 1: Basic server interface

typedef void *miniwebsvr_connection;
typedef void miniwebsvr_logger(void *param, const char *message);
typedef void miniwebsvr_connection_callback(void *param, miniwebsvr_connection connection);

// creates a server which accepts connections, and calls a connection callback for each connection
// returns 0 if successful, negative if error
int miniwebsvr_listen(int port, const char *interface, miniwebsvr_connection_callback*, void *connection_callback_param, miniwebsvr_logger*, void *logger_param);

// causes the server to begin to shut down.
// no more connections will be accepted.
// miniwebsvr_listen will return control once all connections have closed.
void miniwebsvr_close_server(miniwebsvr_connection);

// writes a message to the log
void miniwebsvr_write_log(miniwebsvr_connection, const char*);

// sends characters through the connection
void miniwebsvr_send(miniwebsvr_connection, const char*, int strsize);

// receives characters through the connection
// if error, returns negative, otherwise returns the number of characters read
int miniwebsvr_receive(miniwebsvr_connection, char*, int strsize);
int miniwebsvr_read_line(miniwebsvr_connection, char*, int strsize);
int miniwebsvr_flush_buffer(miniwebsvr_connection, char*, int strsize);

// forces all connections to close immediately. Has no effect if the library is compiled to be single-threaded.
void miniwebsvr_close_all_connections(miniwebsvr_connection);

#ifdef __cplusplus
}
#endif

#endif//MINIWEBSVR_H
