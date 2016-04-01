#ifndef MAS_DUF_SCCBH_EVAL_H
# define MAS_DUF_SCCBH_EVAL_H

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

DR( SCCBH, sccbh_call_scanner, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg, duf_scanstage_t scanstage, duf_scanner_t scanner, duf_node_type_t node_type );

DR( SCCBH, sccbh_eval_all_and_summary, duf_sccb_handle_t * sccbh, bool f_summary ) __attribute__ ( ( warn_unused_result ) ); /* duf_sccbh_eval_all */

DR( SCCBH, sccbh_eval_db_items_str_cb, duf_sccb_handle_t * sccbh, duf_node_type_t node_type, duf_str_cb2_t str_cb2, duf_scanstage_t scanstage );

DR( SCCBH, sccbh_eval_db_leaves_nofd, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg, duf_scanstage_t scanstage )
        __attribute__ ( ( warn_unused_result ) );
DR( SCCBH, sccbh_eval_db_leaves_fd, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg, duf_scanstage_t scanstage )
        __attribute__ ( ( warn_unused_result ) );

#endif
