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
#include "os_compat.h"

#ifdef USE_UNICODE

wchar_t *_w(const char *str) {
	wchar_t *wstr;
	int len;

	if (!str)
		return NULL;

	len = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	wstr = (wchar_t *) malloc(len * sizeof(wchar_t));
	if (!wstr)
		return NULL;
	MultiByteToWideChar(CP_UTF8, 0, str, -1, wstr, len);

	return wstr;
}

char *_u(const wchar_t *wstr) {
	char *str;
	int len;

	if (!wstr)
		return NULL;
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	if (len <= 0)
		return NULL;
	str = (char *) malloc(len * sizeof(char));
	if (!str)
		return NULL;
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if (len <= 0) {
		free(str);
		return NULL;
	}

	return str;
}

FILE *utf8_fopen(const char *filename, const char *mode) {
	wchar_t *wfilename;
	wchar_t *wmode;
	FILE *file;
	
	wfilename = _w(filename);
	wmode = _w(mode);

	file = _wfopen(wfilename, wmode);

	free(wfilename);
	free(wmode);
	
	return file;
}

int utf8_stat(const char *path, struct structstat *buffer) {
	wchar_t *wpath;
	int ret;
	
	wpath = _w(path);

	ret = wstat(wpath, buffer);

	free(wpath);

	return ret;
}

#ifdef __GNUC__
DIR* utf8_opendir(const char *path) {
	DIR *ret;
	wchar_t *wpath;

	wpath = _w(path);
	ret = (DIR *) _wopendir(wpath);
	
	free(wpath);
	return ret;
}

struct dirent* utf8_readdir(DIR *dir) {
	struct _wdirent* ret;
	char *name;

	if (!dir)
		return NULL;

	ret = _wreaddir((_WDIR *) dir);
	if (ret && (name = _u(ret->d_name))) {
		strncpy((char *)ret->d_name, name, FILENAME_MAX);
		free(name);
		ret->d_namlen = strlen((char *)ret->d_name);
	}

	return (struct dirent*) ret;
}

int utf8_closedir(DIR *dir) {
	return _wclosedir((_WDIR *)dir); 
}
#endif //__GNUC__

#endif
