#ifndef MAS_DUF_SQL_H
#  define MAS_DUF_SQL_H

int duf_sql_open( const char *dbpath );
int duf_sql_close( void );



int duf_sql_exec( const char *sql );
int duf_sql_exec_msg( const char *sql, const char *msg );
int duf_sql_exec_c_msg( const char *sql, const char *msg, int constraint_ignore );

int duf_sql_c( const char *sqlfmt, int constraint_ignore, ... );
int duf_sql( const char *sqlfmt, ... );

int duf_sql_select( duf_sql_select_cb_t sel_cb, void *sel_cb_udata, duf_scan_callback_file_t str_cb, void *str_cb_udata,
                    duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, duf_dirhandle_t * pdhu, const char *sqlfmt, ... );

int duf_sql_vselect( duf_sql_select_cb_t sel_cb, void *sel_cb_udata, duf_scan_callback_file_t str_cb, void *str_cb_udata,
                     duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, duf_dirhandle_t * pdhu, const char *sqlfmt, va_list args );

unsigned long long duf_sql_last_insert_rowid( void );


#endif
