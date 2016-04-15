#ifndef MAS_DUF_SCCBH_EVAL_H
# define MAS_DUF_SCCBH_EVAL_H

# include "duf_hook_types.h"                                         /* duf_str_cb2_t; duf_sel_cb2_t; duf_anyhook_t; duf_action_table_t; ✗ */

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

DR( SCCBH, sccbh_eval_all_and_summary, duf_sccb_handle_t * sccbh, bool f_summary ) __attribute__ ( ( warn_unused_result ) ); /* duf_sccbh_eval_all */

DR( SCCBH, sccbh_eval_db_items_str_cb, duf_sccb_handle_t * sccbh, duf_node_type_t node_type, duf_str_cb2s_t str_cb2, duf_scanstage_t scanstage );

DR( SCCBH, sccbh_eval_db_leaves_new, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_unused MAS_UNUSED, */ duf_scanstage_t scanstage )
        __attribute__ ( ( warn_unused_result ) );
/* DR( SCCBH, sccbh_eval_db_leaves_pack, duf_sccb_handle_t * sccbh, (* duf_stmnt_t * pstmt_unused MAS_UNUSED, *) duf_scanstage_t scanstage ) */
/*         __attribute__ ( ( warn_unused_result ) );                                                                                         */
DR( SCCBH, sccbh_eval_db_leaves_nofd, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_x, */ duf_scanstage_t scanstage )
        __attribute__ ( ( warn_unused_result ) );
DR( SCCBH, sccbh_eval_db_leaves_fd, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_x, */ duf_scanstage_t scanstage )
        __attribute__ ( ( warn_unused_result ) );

#endif
