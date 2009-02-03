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
#ifndef SERVER_H
#define SERVER_H

#define SEND_BUFFER_SIZE 16384
#define SERVER_BUFFER_SIZE 2048
#define FILENAME_SIZE 1024
#define MIME_size 16

#ifdef __BORLANDC__
#define ssize_t int
#endif

#if !defined(ssize_t)
#define ssize_t size_t
#endif

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
