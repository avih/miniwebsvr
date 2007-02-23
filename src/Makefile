FILES=config.c gethead.c header.c listener.c logging.c options.c pthreads.c server.c strl.c url.c 
HFILES=config.h gethead.h header.h libminiwebsvr.h listener.h logging.h options.h os_compat.h pthreads.h server.h strl.h url.h
OBJS=`echo "${FILES}" | sed 's/\.cp*/\.o/g'`

LIBS=-lpthread -lrt

RELEASEFLAGS=-O2 -s
DEBUGFLAGS=-O0 -g -D_DEBUG

help:
	@echo 'MiniWebsvr make:'
	@echo 
	@echo 'Flags that affect compilation:'
	@echo '  CFLAGS        - Compiler optimization flags'
	@echo 
	@echo 'Targets:'
	@echo '  release       - Builds a release build of MiniWebsvr'
	@echo '  debug         - Builds a debug build of MiniWebsvr'
	@echo '  lib           - Builds a release library of MiniWebsvr'
	@echo '  lib_debug     - Builds a debug library of MiniWebsvr'
	@echo '  libtest       - Builds a test application using the dynamic library'
	@echo '  slib_release  - Builds a test application using the static library'
	@echo
	@echo 'Maintenance:'
	@echo '  clean         - Cleans up the files created by make'
	@echo
	@echo 'e.g.'
	@echo 'CFLAGS="-march=i686 -fomit-frame-pointer" make release'

slib: $(FILES) $(HFILES)
	gcc -c libmain.c ${FILES} ${RELEASEFLAGS} ${CFLAGS} -DLIB
	ar rc libminiwebsvr.a libmain.o ${OBJS}
	ranlib libminiwebsvr.a

slib_release: slib libminiwebsvr.a
	gcc -o miniwebsvr test libtest.c libminiwebsvr.a -DMULTITHREADED ${LIBS} ${RELEASEFLAGS} ${CFLAGS}
debug:
	gcc -o miniwebsvr ${FILES} main.c ${DEBUGFLAGS} ${LIBS} ${CFLAGS}
release: 
	gcc -o miniwebsvr ${FILES} main.c ${RELEASEFLAGS} ${LIBS} ${CFLAGS}
lib_debug:
	gcc -o libminiwebsvr.so ${FILES} libmain.c ${DEBUGFLAGS} ${LIBS} -shared -fPIC -DLIB ${CFLAGS}
lib:
	gcc -o libminiwebsvr.so ${FILES} libmain.c ${RELEASEFLAGS} ${LIBS} -shared -fPIC -DLIB ${CFLAGS}
libtest:
	gcc -o test libtest.c  -lminiwebsvr ${DEBUGFLAGS} ${LIBS} -DMULTITHREADED ${CFLAGS} -L.
clean:
	rm --force miniwebsvr libminiwebsvr.so test libminiwebsvr.a *.o
