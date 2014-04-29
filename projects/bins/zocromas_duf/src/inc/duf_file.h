#ifndef MAS_DUF_FILE_H
#  define MAS_DUF_FILE_H

int duf_print_files( const char *path, duf_ufilter_t u );

unsigned long long file_at_pathid_to_filenameid_x( unsigned long long pathid, const char *name, unsigned long long *pmd5id, int *pr );
unsigned long long file_at_path_to_filenameid_x( const char *path, const char *name, unsigned long long *pmd5id, int *pr );

unsigned long long file_at_pathid_to_filenameid( unsigned long long pathid, const char *name, int *pr );
unsigned long long file_at_path_to_filenameid( const char *path, const char *name, int *pr );

#ifdef DUF_COMPILE_EXPIRED
char *filenameid_to_filepath( unsigned long long filenameid, duf_depthinfo_t * pdi, int *pr );
#endif

#endif
