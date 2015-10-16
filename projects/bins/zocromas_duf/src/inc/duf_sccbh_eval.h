#ifndef MAS_DUF_SCCBH_EVAL_H
#  define MAS_DUF_SCCBH_EVAL_H

#  include "duf_debug_defs.h"

int DUF_WRAPPED( duf_eval_sccbh_all_and_summary ) ( duf_sccb_handle_t * sccbh, bool f_summary ) __attribute__ ( ( warn_unused_result ) ); /* duf_eval_sccbh_all */


/* int duf_eval_sccbh_db_items_str_cb( duf_node_type_t node_type, duf_str_cb2_t str_cb2, duf_sccb_handle_t * sccbh ) */
/*       __attribute__ ( ( warn_unused_result ) );                                                                   */

int duf_sccbh_eval_db_leaves_nofd( duf_scanstage_t scanstage, duf_stmnt_t * pstmt, duf_sccb_handle_t * sccbh )
      __attribute__ ( ( warn_unused_result ) );
int duf_sccbh_eval_db_leaves_fd( duf_scanstage_t scanstage, duf_stmnt_t * pstmt, duf_sccb_handle_t * sccbh ) __attribute__ ( ( warn_unused_result ) );

int duf_sccbh_eval_db_node( duf_scanstage_t scanstage, duf_stmnt_t * pstmt, duf_sccb_handle_t * sccbh );
int duf_sccbh_eval_db_subnodes( duf_scanstage_t scanstage, duf_stmnt_t * pstmt, duf_sccb_handle_t * sccbh ) __attribute__ ( ( warn_unused_result ) );



#endif
