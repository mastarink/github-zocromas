#ifndef MAS_DUF_FILE_H
#  define MAS_DUF_FILE_H

int duf_print_files( const char *path );

unsigned long long file_at_pathid_to_filenameid( unsigned long long pathid, const char *name, unsigned long long *pmd5id );
unsigned long long file_at_path_to_filenameid( const char *path, const char *name, unsigned long long *pmd5id );

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 *             int fun( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t fuscan )
 * */
int duf_sql_scan_files( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t fuscan );

/* 
 * duf_str_cb_t:
 *                  int fun( unsigned long long pathid, const char *path, unsigned long long filenameid, const char *name, void *str_cb_udata ); 
 * */
int duf_sql_scan_print_file( unsigned long long pathid, const char *path, unsigned long long filenameid, const char *name, void *str_cb_udata );

int duf_scan_vfiles_sql( duf_str_cb_t fun, const char *sql, va_list args );
int duf_scan_files_sql( duf_str_cb_t fun, const char *sql, ... );

#endif
