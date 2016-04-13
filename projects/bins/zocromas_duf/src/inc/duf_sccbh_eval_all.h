#ifndef MAS_DUF_SCCBH_EVAL_ALL_H
# define MAS_DUF_SCCBH_EVAL_ALL_H

/* # include "duf_sccb_types.h"                                         (* duf_scan_callbacks_t; duf_sccb_data_row_t; duf_scanner_fun_t; ✗ *) */
# include "duf_sccbh_types.h"                                        /* duf_sccb_handle_t; duf_sccbh_fun_t; duf_rsccbh_fun_t ✗ */
# include "duf_scan_types.h"                                         /* duf_node_type_t; duf_scanstage_t ✗ */

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

DR( SCCBH, sccbh_eval_all, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_selector, */ duf_scanstage_t scanstage_fake )
        __attribute__ ( ( warn_unused_result ) );

#endif

/*
vi: ft=c
*/
