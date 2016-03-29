#ifndef MAS_DUF_SCCB_BEGFIN_H
# define MAS_DUF_SCCB_BEGFIN_H

# include "duf_sccb_types.h"                                         /* duf_scan_callbacks_t; duf_sccb_handle_t; duf_sccb_data_row_t ✗ */
# include "duf_ufilter_types.h"                                      /* duf_ufilter_t; duf_yfilter_t; etc. ✗ */

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

DR( SCCBH, sccb_eval_sqlsq, const duf_scan_callbacks_t * sccb, const duf_ufilter_t * pu, const duf_yfilter_t * py, const char *selected_db )
        __attribute__ ( ( warn_unused_result ) );
DR( SCCBH, sccb_eval_final_sqlsq, const duf_scan_callbacks_t * sccb, const duf_ufilter_t * pu, const duf_yfilter_t * py )
        __attribute__ ( ( warn_unused_result ) );

#endif

/*
vi: ft=c
*/
