#ifndef DUF_SCCBH_EVAL_H
#  define DUF_SCCBH_EVAL_H

#  include "duf_debug_defs.h"

int DUF_WRAPPED( duf_eval_sccbh_all_and_summary ) ( duf_sccb_handle_t * sccbh ) __attribute__ ( ( warn_unused_result ) ); /* duf_eval_sccbh_all */

int duf_sccbh_eval_db_subnodes( duf_stmnt_t * pstmt, duf_sccb_handle_t * sccbh ) __attribute__ ( ( warn_unused_result ) );
int duf_eval_sccbh_db_items_str_cb( duf_node_type_t node_type, duf_str_cb2_t str_cb2, duf_sccb_handle_t * sccbh ) __attribute__ ( ( warn_unused_result ) );

int duf_sccbh_eval_db_leaves( duf_stmnt_t * pstmt, duf_sccb_handle_t * sccbh ) __attribute__ ( ( warn_unused_result ) );
int duf_sccbh_eval_fs_items( duf_stmnt_t * pstmt, duf_sccb_handle_t * sccbh ) __attribute__ ( ( warn_unused_result ) );


#endif
