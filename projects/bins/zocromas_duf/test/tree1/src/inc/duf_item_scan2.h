#ifndef MAS_DUF_ITEM_SCAN2_H
#  define MAS_DUF_ITEM_SCAN2_H

#  include "duf_scan_types.h"   /* duf_node_type_t */



int duf_scan_db_items_with_str_cb_sql_set( const duf_sql_set_t * sql_set, duf_str_cb2_t str_cb2, duf_sccb_handle_t * sccbh,
                                           duf_node_type_t node_type );



#endif
