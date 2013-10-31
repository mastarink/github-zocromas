#ifndef MAS_DUF_FILE_H
#  define MAS_DUF_FILE_H

int duf_print_files( const char *path );

unsigned long long file_at_pathid_to_filenameid( unsigned long long pathid, const char *name, unsigned long long *pmd5id );
unsigned long long file_at_path_to_filenameid( const char *path, const char *name, unsigned long long *pmd5id );

/* type: duf_sql_select_callback_t */
int duf_sql_scan_files( int nrow, int nrows, char *presult[], va_list args, void *udata, duf_str_callback_t fuscan );

/* type: duf_str_callback_t */
int duf_sql_scan_print_file( unsigned long long pathid, const char *path, unsigned long long filenameid, const char *name, void *udata );

int duf_scan_vfiles_sql( duf_str_callback_t fun, const char *sql, va_list args );
int duf_scan_files_sql( duf_str_callback_t fun, const char *sql, ... );

#endif
