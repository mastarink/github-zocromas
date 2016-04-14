#ifndef MAS_DUF_SCCBH_ROW_H
# define MAS_DUF_SCCBH_ROW_H

# include "duf_sccb_types.h"                                         /* duf_scan_callbacks_t; duf_sccb_data_row_t; duf_scanner_fun_t; ✗ */
# include "duf_pathinfo_types.h"                                     /* duf_pathinfo_t ✗ */
# include "duf_sql_types.h"                                          /* duf_stmnt_t ✗ */

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

/* DRN( SCCBH, void, sccbh_row_open, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg ); */
/* DRN( SCCBH, void, sccbh_row_close, duf_sccb_handle_t * sccbh ); */
DRN( SCCBH, void, sccbh_row_next, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg );

/* unsigned long long duf_sccbh_row_get_number( duf_sccb_handle_t * sccbh, const char *name ); */
DRP( SCCBH, unsigned long long, n, 0, sccbh_row_get_number, duf_sccb_handle_t * sccbh, const char *name );

/* const char *duf_sccbh_row_get_string( duf_sccb_handle_t * sccbh, const char *name ); */
DRP( SCCBH, const char *, s, NULL, sccbh_row_get_string, duf_sccb_handle_t * sccbh, const char *name );

DRN( SCCBH, void, sccbh_rows_eval, duf_sccb_handle_t * sccbh );

#endif

/*
vi: ft=c
*/
