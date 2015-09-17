#ifndef MAS_DUF_SQL_H
#  define MAS_DUF_SQL_H

/* #  include "duf_sql_const.h" */



/* char *duf_sql_vmprintf( const char *fmt, va_list args ); */
char *duf_sql_mprintf( const char *fmt, ... );

/* void duf_sql_free( char *s ); */


int duf_sql_select( duf_sel_cb_t sel_cb, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata, duf_sccb_handle_t * sccbh, const char *sqlfmt,
                    ... );

#endif
