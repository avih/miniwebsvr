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

VERSION="MWH Template/0.0.1svn"

# Common source files
FILES=lexer.cpp parser.cpp
# Common header files
HFILES=lexer.h parser.h
# Standalone executable source files
S_FILES=template.cpp

# POSIX-Style Libraries
LIBS=
# Win32 Libraries
LIBSWIN=

COMMONFLAGS=-Wall -Wextra -DVERSION='${VERSION}' -fno-rtti -fno-exceptions
RELEASEFLAGS=-Os -s
DEBUGFLAGS=-O0 -g -D_DEBUG -pedantic
MINGW=$(shell ls -1 /usr/bin/ | grep "mingw.*\-g++$$")
#OS=$(shell uname)
#ifeq (${OS},Linux)
#LIBS+= -lrt
#endif


help:
	@echo "${VERSION} make:"
	@echo ''
	@echo 'Flags that affect compilation:'
	@echo '  CFLAGS        - Compiler optimization flags'
	@echo ''
	@echo 'Targets:'
	@echo '  release       - Builds a release build of MWH Template'
	@echo '  debug         - Builds a debug build of MWH Template'
	@echo '  xwin32        - Builds a release build of MWH Template for Windows using mingw32-gcc cross compiler'
	@echo '  xwin32d       - Builds a debug build of MWH Template for Windows using mingw32-gcc cross compiler'
	@echo ''
	@echo 'Maintenance:'
	@echo '  clean         - Cleans up the files created by make'
	@echo ''
	@echo 'e.g.'
	@echo 'CFLAGS="-march=i586 -mtune=i686 -fomit-frame-pointer" make release'


debug: $(FILES) $(HFILES) $(S_FILES)
	$(CXX) -o template ${FILES} ${S_FILES} ${COMMONFLAGS} ${DEBUGFLAGS} ${LIBS} ${CFLAGS}

release: $(FILES) $(HFILES) $(S_FILES)
	$(CXX) -o template ${FILES} ${S_FILES} ${COMMONFLAGS} ${RELEASEFLAGS} ${LIBS} ${CFLAGS}

xwin32: $(FILES) $(HFILES) $(S_FILES)
	$(MINGW) -o template.exe -mwindows ${FILES} ${S_FILES} -DUSEWINMAIN ${COMMONFLAGS} ${RELEASEFLAGS} ${LIBSWIN} ${CFLAGS}
#	-@upx --best -q miniwebsvr.exe

xwin32d: $(FILES) $(HFILES) $(S_FILES)
	$(MINGW) -o template.exe ${FILES} ${S_FILES} ${COMMONFLAGS} ${DEBUGFLAGS} ${LIBSWIN} ${CFLAGS}

clean:
	rm -f template template.exe

go: clean debug
	./template
