#ifndef MAS_DUF_FILE_SCAN_H
#  define MAS_DUF_FILE_SCAN_H

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 * */
int duf_sql_scan_files( duf_record_t * precord, va_list args, void *sel_cb_udata,
                        duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb );

/* 
 * duf_scan_callback_file_t:
 * */
int duf_str_cb_scan_print_file( unsigned long long pathid, unsigned long long filenameid,
                                const char *name, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb,
                                duf_record_t * precord );


int duf_scan_items_sql( duf_node_type_t node_type, duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi,
                        duf_scan_callbacks_t * sccb, const char *sql, ... );

/* int duf_scan_items_sql_trace( duf_scan_callback_file_t str_cb, void *str_cb_udata, int trace, const char *sql, ... ); */

#endif
