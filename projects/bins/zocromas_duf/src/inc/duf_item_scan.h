#ifndef MAS_DUF_ITEM_SCAN_H
#  define MAS_DUF_ITEM_SCAN_H

int duf_match_leaf( duf_record_t * precord );
int duf_match_leaf2( duf_sqlite_stmt_t * pstmt );




int duf_scan_db_items( duf_node_type_t node_type, duf_str_cb_t str_cb, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, const char *sql,
                       ... );
int duf_scan_db_items2( duf_node_type_t node_type, duf_str_cb2_t str_cb2, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb,
                        const char *selector2, const char *fieldset, ... );

int duf_count_db_vitems2( duf_sel_cb2_match_t match_cb2, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, const char *selector2,
                          const char *fieldset, unsigned long long dirid );


/* will be static! */
int duf_sel_cb_node( duf_record_t * precord, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata,
                     duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb );
/* will be static! */
int duf_sel_cb_leaf( duf_record_t * precord, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata,
                     duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb );



#endif
