#ifndef MAS_DUF_SEL_CB_NODE_H
# define MAS_DUF_SEL_CB_NODE_H

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */
DR( SCCBH, sel_cb2_node, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt, duf_str_cb2_t str_cb2, duf_scanstage_t scanstage );

#endif
