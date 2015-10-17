#ifndef MAS_DUF_ATABLE_SCCB_H
#  define MAS_DUF_ATABLE_SCCB_H

#  include "duf_scan_types.h"   /* duf_node_type_t */


const duf_action_table_t *duf_find_atable_sccb_by_evname( const char *name, const duf_action_table_t * table );
const duf_action_table_t *duf_find_atable_sccb_by_evnamen( const char *name, size_t namelen, const duf_action_table_t * table );


#endif
