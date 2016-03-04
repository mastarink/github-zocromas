#ifndef MAS_DUF_SCCBH_PSTMT_H
# define MAS_DUF_SCCBH_PSTMT_H

# include "duf_hook_types.h"                                         /* duf_action_table_t ✗ */
# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

DR( SCCBH, sccbh_pstmt_godown_dbopenat_dh, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt, duf_node_type_t node_type );

#endif
