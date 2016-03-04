#ifndef MAS_DUF_SCCBH_EVAL_H
# define MAS_DUF_SCCBH_EVAL_H

int DUF_WRAPPED( duf_sccbh_eval_all_and_summary ) ( duf_sccb_handle_t * sccbh, bool f_summary ) __attribute__ ( ( warn_unused_result ) ); /* duf_sccbh_eval_all */

/* int duf_sccbh_eval_db_items_str_cb( duf_node_type_t node_type, duf_str_cb2_t str_cb2, duf_sccb_handle_t * sccbh ) */
/*       __attribute__ ( ( warn_unused_result ) );                                                                   */

int duf_sccbh_eval_db_leaves_nofd( duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt, duf_scanstage_t scanstage )
        __attribute__ ( ( warn_unused_result ) );
int duf_sccbh_eval_db_leaves_fd( duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt, duf_scanstage_t scanstage ) __attribute__ ( ( warn_unused_result ) );

int duf_sccbh_eval_db_node( duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt, duf_scanstage_t scanstage );
int duf_sccbh_eval_db_subnodes(duf_sccb_handle_t * sccbh , duf_stmnt_t * pstmt, duf_scanstage_t scanstage ) __attribute__ ( ( warn_unused_result ) );

#endif
