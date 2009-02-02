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
