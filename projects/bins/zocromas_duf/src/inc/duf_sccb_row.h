#ifndef MAS_DUF_SCCB_ROW_H
# define MAS_DUF_SCCB_ROW_H

# include "duf_sccb_types.h"                                         /* duf_scan_callbacks_t; duf_sccb_data_row_t; duf_scanner_fun_t; ✗ */
# include "duf_sccbh_types.h"                                        /* duf_sccb_handle_t; duf_sccbh_fun_t; duf_rsccbh_fun_t ✗ */
# include "duf_pathinfo_types.h"                                     /* duf_pathinfo_t ✗ */
# include "duf_sql_types.h"                                          /* duf_stmnt_t; duf_sqltype_t; duf_sqexe_cb_t ✗ */

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

DRX( OTHER, duf_sccb_data_row_t *, row, NULL, datarow_create, duf_stmnt_t * pstmt_arg, const duf_pathinfo_t * pi, const seq_t * seqq );

DRN( OTHER, void, datarow_delete, duf_sccb_data_row_t * row );

DRX( OTHER, duf_sccb_data_row_t *, row, NULL, datarow_list_first, const duf_sccb_data_list_t * rowlist );
DRX( OTHER, duf_sccb_data_row_t *, row, NULL, datarow_list_last, const duf_sccb_data_list_t * rowlist );

/* duf_sccb_data_row_t *duf_datarow_list_next(void); */
DRX( OTHER, duf_sccb_data_row_t *, row, NULL, datarow_list_next, const duf_sccb_data_row_t * qrow );
DRX( OTHER, duf_sccb_data_row_t *, row, NULL, datarow_list_penult, const duf_sccb_data_list_t * rowlist );

DRX( OTHER, duf_sccb_data_row_t *, row, NULL, datarow_list_unlink, duf_sccb_data_list_t * rowlist, duf_sccb_data_row_t * later,
     duf_sccb_data_row_t * urow );
DRX( OTHER, duf_sccb_data_row_t *, row, NULL, datarow_list_link, duf_sccb_data_list_t * rowlist, duf_sccb_data_row_t * lrow );

DRP( OTHER, int, cnt, 0, datarow_list_count, const duf_sccb_data_list_t * rowlist );

DRN( OTHER, void, datarow_list_delete_f, duf_sccb_data_list_t * rowlist, int min, int max );

/* DRN( OTHER, void, datarow_list_delete_r, duf_sccb_data_row_t * rows ); */

DRP( OTHER, duf_sccb_data_value_t *, val, NULL, datarow_field_find, const duf_sccb_data_row_t * row, const char *name );
DRX( OTHER, char *, lst, NULL, datarow_field_list, const duf_sccb_data_row_t * row );

DRP( SCCBH, duf_sqltype_t, typ, DUF_SQLTYPE_NONE, datarow_get_type, const duf_sccb_data_row_t * row, const char *name );

DRP( OTHER, unsigned long long, n, 0, datarow_get_number, const duf_sccb_data_row_t * row, const char *name );

DRP( OTHER, const char *, s, NULL, datarow_get_string, const duf_sccb_data_row_t * row, const char *name );


DRP( OTHER, int, isnull, 1, datarow_get_null, const duf_sccb_data_row_t * row, const char *name );

/* DRP( SCCBH, unsigned long long, n, 0, sccbh_row_get_number, duf_sccb_handle_t * sccbh, const char *name ); */

/* DRP( SCCBH, const char *, s, NULL, sccbh_row_get_string, duf_sccb_handle_t * sccbh, const char *name ); */

/* DRN( SCCBH, void, sccbh_rows_eval, duf_sccb_handle_t * sccbh ); */

DRX( OTHER, const duf_pathinfo_t *, pi, NULL, datarow_get_pathinfo, const duf_sccb_data_row_t * row );

#endif

/*
vi: ft=c
*/
