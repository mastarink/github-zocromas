#ifndef MAS_DUF_FILE_MD5ID_H
#  define MAS_DUF_FILE_MD5ID_H

unsigned long long duf_filepath_md5id( const char *path );

int duf_print_files_by_md5id( unsigned long long md5id );
int duf_print_files_same( const char *path );

#endif
