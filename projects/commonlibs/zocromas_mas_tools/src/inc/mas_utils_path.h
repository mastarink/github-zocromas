#ifndef MAS_UTILS_PATH_H
#  define MAS_UTILS_PATH_H

int mas_pathdepth( const char *path );
char *mas_realpath( const char *path /* , int *pr */  );
char *mas_concat_path( const char *path, const char *subpath );
char *mas_normalize_path( const char *path );
char *mas_normalize_path_plus( const char *path, ... );

#endif