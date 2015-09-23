#ifndef MAS_DUF_UTILS_PATH_H
#  define MAS_DUF_UTILS_PATH_H

int duf_pathdepth( const char *path );
char *duf_realpath( const char *path, int *pr );
char *duf_concat_path( char *path, const char *subpath );
char *duf_normalize_path( char *path );

#endif
