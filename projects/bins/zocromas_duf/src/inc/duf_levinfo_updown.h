#ifndef MAS_DUF_LEVINFO_UPDOWN_H
# define MAS_DUF_LEVINFO_UPDOWN_H

# include "duf_levinfo_types.h"                                      /* duf_levinfo_t ✗ */
# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ✗ */
# include "duf_scan_types.h"                                         /* duf_node_type_t; duf_scanstage_t ✗ */
# include "duf_sql_types.h"                                          /* duf_stmnt_t ✗ */

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

DR( PDI, levinfo_godown_dbopenat_dh, duf_depthinfo_t * pdi, duf_node_type_t node_type, duf_stmnt_t * pstmt );
DR( PDI, levinfo_godown_openat_dh, duf_depthinfo_t * pdi, const char *itemname, duf_node_type_t node_type ); /* XXX equal XXX to duf_levinfo_godown ? XXX */

DR( PDI, levinfo_goup, duf_depthinfo_t * pdi );
DR( PDI, levinfo_gotop, duf_depthinfo_t * pdi );

DR( PDI, levinfo_godown, duf_depthinfo_t * pdi, const char *itemname, duf_node_type_t node_type );

#endif
