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
#ifndef HEADER_H
#define HEADER_H

#include "server.h"

#define EXT_size 8

#define headerResp_size 11
#define headerExt_size 16

struct headerResp_struct
{
	int autogen;
	int respval;
	char* respstr;
};

extern const struct headerResp_struct headerResp[headerResp_size];

struct headerExt_struct
{
	char ext[EXT_size];
	char type[MIME_size];
};

extern const struct headerExt_struct headerExt[headerExt_size];


void setHeader_filename(struct server_struct *inst,char* filename);
void setHeader_respval(struct server_struct *inst,int respval);
void setHeader_generic(struct server_struct *inst,char* genstr);

void clearHeader(struct server_struct *inst);
// Returns the no of bytes used up in the buffer
int printHeader(struct server_struct *inst, int headeronly, char* Buffer, int bufsize);

#endif // HEADER_H
