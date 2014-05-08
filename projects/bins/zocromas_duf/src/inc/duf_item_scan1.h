#ifndef MAS_DUF_ITEM_SCAN1_H
#  define MAS_DUF_ITEM_SCAN1_H

int duf_match_leaf( duf_record_t * precord );




int duf_scan_db_items( duf_node_type_t node_type, duf_str_cb_t str_cb, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, const char *sql,
                       ... );


/* will be static! */
int duf_sel_cb_node( duf_record_t * precord, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata,
                     duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb );
/* will be static! */
int duf_sel_cb_leaf( duf_record_t * precord, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata,
                     duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb );



#endif

