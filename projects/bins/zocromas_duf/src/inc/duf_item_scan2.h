#ifndef MAS_DUF_ITEM_SCAN2_H
#  define MAS_DUF_ITEM_SCAN2_H

#  include "duf_scan_types.h"   /* duf_node_type_t */


int duf_eval_sccbh_db_items_str_cb( duf_node_type_t node_type, duf_str_cb2_t str_cb2, duf_sccb_handle_t * sccbh );

char *duf_selector_total2sql( const duf_sql_set_t * sql_set );


#endif
