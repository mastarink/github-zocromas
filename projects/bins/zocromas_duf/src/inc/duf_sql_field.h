#ifndef MAS_DUF_SQL_FIELD_H
#  define MAS_DUF_SQL_FIELD_H



int __duf_sql_pos_by_name( const char *name, duf_record_t * precord, int *phave, int optional );
const char *__duf_sql_str_by_name( const char *name, duf_record_t * precord, int *phave, int optional );
unsigned long long __duf_sql_ull_by_name( const char *name, duf_record_t * precord, int *phave, int optional );

int duf_sel_cb_field_by_sccb( duf_record_t * precord, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata,
                              duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb );



#endif
