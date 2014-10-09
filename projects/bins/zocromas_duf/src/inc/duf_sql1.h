#ifndef MAS_DUF_SQL1_H
#  define MAS_DUF_SQL1_H

#  include "duf_record_types.h"
#  include "duf_hook_types.h"


/* int duf_sql_execcb( const char *sql, duf_sqexe_cb_t sqexe_cb, void *sqexe_data, int *pchanges ); */
/* int duf_sql_exec( const char *sql, int *pchanges ); */
int duf_sql_exec_msg( const char *sql, const char *msg );

/* int duf_sql_c( const char *sqlfmt, int constraint_ignore, int *pchanges, ... ); */
int duf_sql( const char *sqlfmt, int *pchanges, ... ) __attribute__ ( ( format( __printf__, 1, 3 ) ) );

int duf_sql_select( duf_sel_cb_t sel_cb, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata,
                    DSCCBX, const char *sqlfmt, ... );

/* int duf_sql_vselect( duf_sel_cb_t sel_cb, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata,       */
/*                      duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, const char *sqlfmt, va_list args ); */


#endif

