#ifndef MAS_DUF_ITEM_SCAN_H
#  define MAS_DUF_ITEM_SCAN_H


int duf_scan_items_sql( duf_node_type_t node_type, duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_depthinfo_t * pdi,
                        duf_scan_callbacks_t * sccb, const duf_dirhandle_t * pdhu, const char *sql, ... );


/* will be static! */
int duf_sel_cb_items( duf_record_t * precord, va_list args, void *sel_cb_udata, duf_scan_callback_file_t str_cb, void *str_cb_udata,
                      duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, const duf_dirhandle_t * pdhu );
/* will be static! */
int duf_sel_cb_node( duf_record_t * precord, va_list args, void *sel_cb_udata, duf_scan_callback_file_t str_cb, void *str_cb_udata,
                      duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, const duf_dirhandle_t * pdhu );
/* will be static! */
int duf_sel_cb_leaf( duf_record_t * precord, va_list args, void *sel_cb_udata, duf_scan_callback_file_t str_cb, void *str_cb_udata,
                      duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, const duf_dirhandle_t * pdhu );



#endif
