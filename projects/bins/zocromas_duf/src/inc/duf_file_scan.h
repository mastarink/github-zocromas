#ifndef MAS_DUF_FILE_SCAN_H
#  define MAS_DUF_FILE_SCAN_H

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 * */
int duf_sql_scan_files( int nrow, int nrows, const char *const *presult, va_list args, void *sel_cb_udata, duf_str_cb_t str_cb,
                        void *str_cb_udata );

/* 
 * duf_str_cb_t:
 * */
int duf_sql_scan_print_file( unsigned long long pathid, const char *path, unsigned long long filenameid, const char *name,
                             const struct stat *st, void *str_cb_udata, const char *const *presult );


int duf_scan_vfiles_sql( duf_str_cb_t str_cb, void *str_cb_udata, const char *sql, va_list args );
int duf_scan_files_sql( duf_str_cb_t str_cb, void *str_cb_udata, const char *sql, ... );
int duf_scan_files_sql_trace( duf_str_cb_t str_cb, void *str_cb_udata, int trace, const char *sql, ... );

#endif
