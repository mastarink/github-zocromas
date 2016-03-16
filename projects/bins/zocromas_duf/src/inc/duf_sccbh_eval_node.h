#ifndef MAS_DUF_SCCBH_EVAL_NODE_H
# define MAS_DUF_SCCBH_EVAL_NODE_H

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */



DR( SCCBH, sccbh_eval_db_node, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt, duf_scanstage_t scanstage );
DR( SCCBH, sccbh_eval_db_subnodes, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt, duf_scanstage_t scanstage )
        __attribute__ ( ( warn_unused_result ) );

#endif

