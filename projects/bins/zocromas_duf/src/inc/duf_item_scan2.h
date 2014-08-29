#ifndef MAS_DUF_ITEM_SCAN2_H
#  define MAS_DUF_ITEM_SCAN2_H

#  include "duf_scan_types.h"   /* duf_node_type_t */

int duf_match_leaf2( duf_sqlite_stmt_t * pstmt );

int duf_scan_db_items2( duf_node_type_t node_type, duf_str_cb2_t str_cb2, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, const char *selector2,
                        const char *fieldset );

int duf_count_db_items2( duf_sel_cb2_match_t match_cb2, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, const char *selector2,
                          const char *fieldset );



#endif
