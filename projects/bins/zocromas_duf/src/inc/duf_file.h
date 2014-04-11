#ifndef MAS_DUF_FILE_H
#  define MAS_DUF_FILE_H

int duf_print_files( const char *path, duf_ufilter_t u );

unsigned long long file_at_pathid_to_filenameid_x( unsigned long long pathid, const char *name, unsigned long long *pmd5id );
unsigned long long file_at_path_to_filenameid_x( const char *path, const char *name, unsigned long long *pmd5id );

unsigned long long file_at_pathid_to_filenameid( unsigned long long pathid, const char *name );
unsigned long long file_at_path_to_filenameid( const char *path, const char *name );

char *filenameid_to_filepath( unsigned long long filenameid );

#endif
