#ifndef MAS_DUF_SQL_FIELD2_H
#  define MAS_DUF_SQL_FIELD2_H

#  include "duf_record_types.h"
#  include "duf_hook_types.h"


#if 0
int __duf_sql_pos_by_name( const char *name, duf_record_t * precord, int *phave, int optional );
const char *__duf_sql_str_by_name( const char *name, duf_record_t * precord, int *phave, int optional );
unsigned long long __duf_sql_ull_by_name( const char *name, duf_record_t * precord, int *phave, int optional );


int duf_sel_cb_field_by_sccb( duf_record_t * precord, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata, duf_sccb_handle_t * sccbh );
#endif



#endif
