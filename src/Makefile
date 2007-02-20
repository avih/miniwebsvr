FILES=config.c gethead.c header.c listener.c logging.c miniweblogo.c options.c pthreads.c server.c strl.c url.c 

LIBS=-lpthread


debug:
	gcc -o miniwebsvr ${FILES} main.c -O0 -g -D_DEBUG ${LIBS}
release:
	gcc -o miniwebsvr ${FILES} main.c -O3 ${LIBS}
lib:
	gcc -o libminiwebsvr.so ${FILES} libmain.c -O0 -g ${LIBS} -shared -fPIC -DLIB
	sudo cp libminiwebsvr.so /usr/lib/
lib_release:
	gcc -o libminiwebsvr.so ${FILES} libmain.c -O3 ${LIBS} -shared -fPIC -DLIB
	sudo cp libminiwebsvr.so /usr/lib/
libtest:
	gcc -o test libtest.c  -lminiwebsvr -O0 -g ${LIBS} -DMULTITHREADED
