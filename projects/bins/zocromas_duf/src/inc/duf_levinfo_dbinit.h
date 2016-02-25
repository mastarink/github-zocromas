#ifndef MAS_DUF_LEVINFO_DBINIT_H
# define MAS_DUF_LEVINFO_DBINIT_H

# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ♠ */
# include "duf_scan_types.h"                                         /* duf_node_type_t ♠ */

void duf_levinfo_dbinit_level_d( duf_depthinfo_t * pdi, duf_stmnt_t * pstmt, duf_node_type_t node_type, int d );

#endif
