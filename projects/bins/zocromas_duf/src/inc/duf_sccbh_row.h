#ifndef MAS_DUF_SCCBH_ROW_H
# define MAS_DUF_SCCBH_ROW_H

# include "duf_sccb_types.h"                                         /* duf_scan_callbacks_t; duf_sccb_data_row_t; duf_scanner_fun_t; ✗ */
# include "duf_sccbh_types.h"                                        /* duf_sccb_handle_t; duf_sccbh_fun_t; duf_rsccbh_fun_t ✗ */
# include "duf_pathinfo_types.h"                                     /* duf_pathinfo_t ✗ */
# include "duf_sql_types.h"                                          /* duf_stmnt_t ✗ */

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

DRN( SCCBH, void, sccbh_row_add, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg );
DRN( SCCBH, void, sccbh_row_add_dummy, duf_sccb_handle_t * sccbh );

DRP( SCCBH, unsigned long long, n, 0, sccbh_row_get_number, duf_sccb_handle_t * sccbh, const char *name );

DRP( SCCBH, const char *, s, NULL, sccbh_row_get_string, duf_sccb_handle_t * sccbh, const char *name );


DRX( SCCBH, const duf_sccb_data_row_t *, row, NULL, sccbh_prev_row, duf_sccb_handle_t * sccbh );
DRN( SCCBH, void, sccbh_end_row, duf_sccb_handle_t * sccbh );
DRX( SCCBH, const duf_sccb_data_row_t *, row, NULL, sccbh_start_row, duf_sccb_handle_t * sccbh );

DRX( SCCBH, const duf_sccb_data_row_t *, row, NULL, sccbh_row_current, const duf_sccb_handle_t * sccbh );
DRX( SCCBH, const duf_pathinfo_t *, pi, NULL, sccbh_row_pathinfo, const duf_sccb_handle_t * sccbh );

#endif

/*
vi: ft=c
*/
