#ifndef MAS_DUF_SCCB_SCANSTAGE_H
#  define MAS_DUF_SCCB_SCANSTAGE_H

#  include "duf_hook_types.h"
duf_scanner_t duf_scanstage_scanner( const duf_scan_callbacks_t * sccb, duf_scanstage_t scanstage, int deleted, duf_node_type_t nt );
const char *duf_scanstage_name( duf_scanstage_t scanstage, duf_node_type_t nt );

const char *duf_nodetype_name( duf_node_type_t nt );


#endif
