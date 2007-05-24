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
#ifndef SERVER_H
#define SERVER_H

#define SEND_BUFFER_SIZE 16384
#define SERVER_BUFFER_SIZE 2048
#define FILENAME_SIZE 1024
#define MIME_size 16

struct server_struct 
{
	// Tempory recieve/send buffer
	char buffer[SERVER_BUFFER_SIZE];
	unsigned int buffer_pos;
	ssize_t buffer_size;
	char logbuffer[1024];
	
	// Socket
	SOCKET sock;

	// Incoming connection data
	u_short sin_port;
	struct in_addr sin_addr;
	
	// response header
	int respval;
	char MIMEtype[MIME_size];
	char header_content[SERVER_BUFFER_SIZE];
	char *MIMEoverride; 
};

#ifdef __WIN32__
DWORD server(struct server_struct *inst);
#else
void* server(struct server_struct *inst);
#endif

int server_readln(struct server_struct *inst, char *str, const int strsize);

#endif // SERVER_H
