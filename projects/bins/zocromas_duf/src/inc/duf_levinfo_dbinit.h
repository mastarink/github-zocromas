#ifndef MAS_DUF_LEVINFO_DBINIT_H
# define MAS_DUF_LEVINFO_DBINIT_H

# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ✗ */
# include "duf_scan_types.h"                                         /* duf_node_type_t; duf_scanstage_t ✗ */
# include "duf_sql_types.h"                                          /* duf_stmnt_t ✗ */

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

void duf_levinfo_dbinit_level_d( duf_depthinfo_t * pdi, duf_stmnt_t * pstmt_arg, duf_node_type_t node_type, int d );

#endif
