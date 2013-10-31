#ifndef MAS_DUF_SQL_H
#  define MAS_DUF_SQL_H

int duf_sql_open( const char *dbpath );
int duf_sql_close( void );


int duf_sql_exec( const char *sql );
int duf_sql_exec_msg( const char *sql, const char *msg );
int duf_sql_exec_c( const char *sql, int constraint_ignore );
int duf_sql_exec_c_msg( const char *sql, const char *msg, int constraint_ignore );
int duf_sql_c( const char *fmt, int constraint_ignore, ... );
int duf_sql( const char *fmt, ... );

int duf_sql_vselect( duf_sql_select_cb_t cb, void *sel_cb_udata, duf_str_cb_t fuscan, int trace, const char *fmt, va_list args );
int duf_sql_select( duf_sql_select_cb_t func, void *sel_cb_udata, duf_str_cb_t fuscan, int trace, const char *fmt, ... );

unsigned long long duf_last_insert_rowid( void );

extern int duf_constraint;
#endif
