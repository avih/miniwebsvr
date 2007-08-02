# Makefile for MiniWebSvr
#
# MiniWebSvr - A small webserver
# Copyright (C) 2007  Nickolas Antonie Grigoriadis and Sergey Sytchewoj
# E-Mail: grigi_ at users.sourceforge.net and brumal at users.sourceforge.net
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

VERSION="MiniWebSvr/0.0.9svn"

# Common source files
FILES=src/config.c src/gethead.c src/header.c src/listener.c src/logging.c src/options.c src/server.c src/strl.c src/url.c
# Common header files
HFILES=src/config.h src/gethead.h src/header.h src/listener.h src/logging.h src/options.h src/os_compat.h src/server.h src/strl.h src/url.h
# Standalone executable source files
S_FILES=src/main.c

# POSIX-Style Libraries
LIBS=-lpthread
# Win32 Libraries
LIBSWIN=-lws2_32

COMMONFLAGS=-Wall -Wextra -DVERSION='${VERSION}'
RELEASEFLAGS=-Os -s
DEBUGFLAGS=-O0 -g -D_DEBUG -pedantic -std=gnu99
OS=$(shell uname)
ifeq (${OS},Linux)
LIBS+= -lrt
endif


help:
	@echo "${VERSION} make:"
	@echo ''
	@echo 'Flags that affect compilation:'
	@echo '  CFLAGS        - Compiler optimization flags'
	@echo ''
	@echo 'Targets:'
	@echo '  release       - Builds a release build of MiniWebSvr'
	@echo '  debug         - Builds a debug build of MiniWebSvr'
	@echo '  xwin32        - Builds a release build of MiniWebSvr for Windows using mingw32-gcc cross compiler'
	@echo '  xwin32d       - Builds a debug build of MiniWebSvr for Windows using mingw32-gcc cross compiler'
	@echo ''
	@echo 'Maintenance:'
	@echo '  clean         - Cleans up the files created by make'
	@echo ''
	@echo 'e.g.'
	@echo 'CFLAGS="-march=i586 -mtune=i686 -fomit-frame-pointer" make release'


debug: $(FILES) $(HFILES) $(S_FILES)
	gcc -o miniwebsvr ${FILES} ${S_FILES} ${COMMONFLAGS} ${DEBUGFLAGS} ${LIBS} ${CFLAGS}

release: $(FILES) $(HFILES) $(S_FILES)
	gcc -o miniwebsvr ${FILES} ${S_FILES} ${COMMONFLAGS} ${RELEASEFLAGS} ${LIBS} ${CFLAGS}

xwin32: $(FILES) $(HFILES) $(S_FILES)
	mingw32-gcc -o miniwebsvr.exe ${FILES} ${S_FILES} ${COMMONFLAGS} ${RELEASEFLAGS} ${LIBSWIN} ${CFLAGS}
	upx --best -q miniwebsvr.exe

xwin32d: $(FILES) $(HFILES) $(S_FILES)
	mingw32-gcc -o miniwebsvr.exe ${FILES} ${S_FILES} ${COMMONFLAGS} ${DEBUGFLAGS} ${LIBSWIN} ${CFLAGS}

clean:
	rm -f miniwebsvr miniwebsvr.exe miniwebsvr.log
