#ifndef MAS_DUF_SCCBH_EVAL_ALL_H
# define MAS_DUF_SCCBH_EVAL_ALL_H

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

int duf_sccbh_eval_all( duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_selector, duf_scanstage_t scanstage_fake )
        __attribute__ ( ( warn_unused_result ) );

#endif

/*
vi: ft=c
*/
