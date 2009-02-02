/*  MiniWebSvr - A small webserver
    Copyright (c) 2009 Jackie Ku and Nickolas Antonie Grigoriadis
    E-Mail: jackieku at users.sourceforge.net and grigi_ at users.sourceforge.net

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
#ifndef UTF_LAYER_H
#define UTF_LAYER_H

#include <stdio.h>
#include <sys/stat.h>

#ifdef fopen
#undef fopen
#endif
#define fopen utf8_fopen

FILE *utf8_fopen(
	const char *filename,
	const char *mode
);

#ifdef funcstat
#undef funcstat
#endif
#define funcstat utf8_stat

int utf8_stat(
	const char *path,
	struct structstat *buffer
);

#ifdef __GNUC__
#include <dirent.h>

#ifdef opendir
#undef opendir
#endif
#define opendir utf8_opendir
DIR* utf8_opendir(const char *path);

#ifdef readdir
#undef readdir
#endif
#define readdir utf8_readdir
struct dirent* utf8_readdir(DIR*);

#ifdef closedir
#undef closedir
#endif
#define closedir utf8_closedir
int utf8_closedir(DIR*);

#endif // __GNUC__

#endif // ! UTF_LAYER_H
