#ifndef MAS_DUF_SQL_H
#  define MAS_DUF_SQL_H


int duf_sql_open( const char *dbpath );
int duf_sql_close( void );


int duf_sql_exec( const char *sql );
int duf_sql_exec_msg( const char *sql, const char *msg );
int duf_sql_exec_c( const char *sql, int constraint_ignore );
int duf_sql_exec_c_msg( const char *sql, const char *msg, int constraint_ignore );

int duf_vsql_c( const char *sqlfmt, int constraint_ignore, va_list args );
int duf_sql_c( const char *sqlfmt, int constraint_ignore, ... );
int duf_sql( const char *sqlfmt, ... );

int duf_sql_vselect( duf_sql_select_cb_t sel_cb, void *sel_cb_udata, duf_scan_callback_file_t str_cb, void *str_cb_udata,
                     duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb, const char *sqlfmt, va_list args );
int duf_sql_select( duf_sql_select_cb_t sel_cb, void *sel_cb_udata, duf_scan_callback_file_t str_cb, void *str_cb_udata,
                    duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb, const char *sqlfmt, ... );

unsigned long long duf_last_insert_rowid( void );

int duf_sql_pos_by_name( const char *name, duf_record_t * precord, int optional );
const char *duf_sql_str_by_name( const char *name, duf_record_t * precord, int optional );
unsigned long long duf_sql_ull_by_name( const char *name, duf_record_t * precord , int optional);

extern int duf_constraint;
#endif
