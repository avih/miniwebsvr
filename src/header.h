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
#ifndef HEADER_H
#define HEADER_H

#include "server.h"

#define EXT_size 8

#define headerResp_size 8
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
