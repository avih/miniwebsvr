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
#ifndef LOGGING_H
#define LOGGING_H

#define MESSAGE_BUFFER 1024

void StartLogging(char* name);
void StopLogging();

void Message(const char *format, ...);

#if (defined _DEBUG) || (defined __BORLANDC__)
void DebugMSG(const char *format, ...);
#else // _DEBUG
#define DebugMSG(f,...)
#endif // _DEBUG

#ifdef LIB
void mwb_Log(char* txt);
#else
void Log(char* txt);
#endif

void BIGMessage(const char *format, ...);

void Error(const char *format, ...);

void Critical(const char *format, ...);

#endif // LOGGING_H
