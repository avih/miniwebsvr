#!/bin/sh

BVER=`cat Makefile | grep ^VERSION= | sed s/^VERSION=\"MiniWebSvr\\\/// | sed s/\"// | sed s/[^\\\.a-zA-Z0-9]*\$//`
VERSION="miniwebsvr_"$BVER"_"
echo "For version MiniWebSvr/$BVER"

copypart () {
	for item in $1; do
		FILES=`cat Makefile | grep ^$item= | sed s/^$item=// | sed s/[^\\\.a-zA-Z0-9_]*\$//`
		for file in $FILES ; do
			`cp $file $VERSION$2/$file`
			`chmod 664 $VERSION$2/$file`
		done
	done
	cat src/config.h | sed s/\"MiniWebSvr\\\/[^\"].*/\"MiniWebSvr\\\/$BVER\"/ > $VERSION$2/src/config.h
}

copyfiles () {
	for file in $1; do
		`cp $file $VERSION$2/$file`
		`chmod 664 $VERSION$2/$file`
	done
}


# for src
do_src () {
	echo $VERSION"src"
	gmake clean
	mkdir $VERSION"src"
	mkdir $VERSION"src/src"
	copypart "FILES HFILES S_FILES" "src"
	copyfiles "ChangeLog.txt Compiling.txt License.txt Makefile ReadMe.txt build.sh" "src"
	cp -r test $VERSION"src"
	find $VERSION"src/test" | fgrep .svn | xargs rm -fR  2> /dev/null
	find $VERSION"src/test/" -type f | sed s/^/\\\"/ | sed s/\$/\\\"/ | xargs chmod 664
	chmod 775 $VERSION"src/build.sh"
	chmod 775 $VERSION"src/test/test.pl"
	tar cpf $VERSION"src.tar" $VERSION"src"
	rm -f $VERSION"src.tar.gz" 2> /dev/null
	gzip -9 $VERSION"src.tar"
	rm -fR $VERSION"src/"
}

# for linux
do_linux () {
	echo $VERSION"linux"
	gmake clean release
	mkdir $VERSION"linux"
	mkdir $VERSION"linux/src"
	copypart "FILES HFILES S_FILES" "linux"
	copyfiles "ChangeLog.txt Compiling.txt License.txt Makefile ReadMe.txt miniwebsvr" "linux"
	tar cpf $VERSION"linux.tar" $VERSION"linux"
	rm -f $VERSION"linux.tar.gz" 2> /dev/null
	gzip -9 $VERSION"linux.tar"
	rm -fR $VERSION"linux/"
}

# for win32
do_win32 () {
	echo $VERSION"win32"
	gmake clean xwin32 xwin32c
	mkdir $VERSION"win32"
	mkdir $VERSION"win32/src"
	copypart "FILES HFILES S_FILES" "win32"
	copyfiles "ChangeLog.txt Compiling.txt License.txt ReadMe.txt miniwebsvr.exe miniwebsvrcon.exe" "win32"
	zip -r9q $VERSION"win32.zip" $VERSION"win32"
	rm -fR $VERSION"win32/"
}

# for FreeBSD
do_freebsd () {
	echo $VERSION"freebsd"
	gmake clean release
	mkdir $VERSION"freebsd"
	mkdir $VERSION"freebsd/src"
	copypart "FILES HFILES S_FILES" "freebsd"
	copyfiles "ChangeLog.txt Compiling.txt License.txt Makefile ReadMe.txt miniwebsvr" "freebsd"
	tar cpf $VERSION"freebsd.tar" $VERSION"freebsd"
	rm -f $VERSION"freebsd.tar.gz" 2> /dev/null
	gzip -9 $VERSION"freebsd.tar"
	rm -fR $VERSION"freebsd/"
}

if [ -z $1 ]; then
	echo "$0 [src] [linux] [win32] [freebsd]"
	exit
fi

T=$1

while shift; do
	if [ $T = "src" ]; then
		do_src
	fi
	if [ $T = "linux" ]; then
	        do_linux
	fi
	if [ $T = "win32" ]; then
	        do_win32
	fi
	if [ $T = "freebsd" ]; then
	        do_freebsd
	fi
	T=$1
done

gmake clean
