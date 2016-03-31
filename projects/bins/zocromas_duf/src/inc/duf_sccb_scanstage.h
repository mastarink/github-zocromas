#ifndef MAS_DUF_SCCB_SCANSTAGE_H
# define MAS_DUF_SCCB_SCANSTAGE_H

# include "duf_sccb_types.h"                                         /* duf_scan_callbacks_t; duf_sccb_handle_t; duf_sccb_data_row_t ✗ */
# include "duf_scan_types.h"                                         /* duf_node_type_t; duf_scanstage_t ✗ */
# include "duf_hook_types.h"                                         /* duf_action_table_t ✗ */

duf_scanner_t duf_sccb_scanstage_scanner( const duf_scan_callbacks_t * sccb, duf_scanstage_t scanstage, int deleted, duf_node_type_t nt );

const char *duf_scanstage_name( duf_scanstage_t scanstage );
const char *duf_scanstage_shortname( duf_scanstage_t scanstage );


#endif
