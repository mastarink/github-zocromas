#ifndef MAS_DUF_SQLITE_SELECT_H
#  define MAS_DUF_SQLITE_SELECT_H

#  include <sqlite3.h>

int duf_sqlite_vselect( duf_sel_cb_t sel_cb, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata,
                        duf_sccb_handle_t * sccbh, const char *sqlfmt, va_list args );



#endif

