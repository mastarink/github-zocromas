#ifndef MAS_DUF_SCCB_H
#  define MAS_DUF_SCCB_H

/* #  include "duf_sccb_way.h" */

#  include "duf_hook_types.h"
#  include "duf_scan_types.h"
const char *_duf_uni_scan_action_title( const duf_scan_callbacks_t * sccb );
const char *duf_uni_scan_action_title( const duf_scan_callbacks_t * sccb );

int duf_sccb_pdi( duf_sccb_handle_t * sccbh );

const duf_sql_set_t *duf_get_leaf_sql_set( const duf_scan_callbacks_t * sccb );
const duf_sql_set_t *duf_get_node_sql_set( const duf_scan_callbacks_t * sccb );
const duf_sql_set_t *duf_get_sql_set( const duf_scan_callbacks_t * sccb, duf_node_type_t node_type, int *pr );

#endif
