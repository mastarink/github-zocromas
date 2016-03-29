#ifndef MAS_DUF_SCCB_ROW_H
# define MAS_DUF_SCCB_ROW_H

# include "duf_sccb_types.h"                                         /* duf_scan_callbacks_t; duf_sccb_handle_t; duf_sccb_data_row_t ✗ */
# include "duf_pathinfo_types.h"                                     /* duf_pathinfo_t ✗ */
# include "duf_sql_types.h"                                          /* duf_stmnt_t ✗ */

duf_sccb_data_row_t *duf_datarow_create( duf_stmnt_t * pstmt, const duf_pathinfo_t * pi );
void duf_datarow_delete( duf_sccb_data_row_t * row );

int duf_datarow_list_count( const duf_sccb_data_row_t * rows );
void duf_datarow_list_delete_f( duf_sccb_data_row_t * rows, int skip );
void duf_datarow_list_delete_r( duf_sccb_data_row_t * rows );

duf_sccb_data_value_t *duf_datarow_field_find( const duf_sccb_data_row_t * row, const char *name );
unsigned long long duf_datarow_get_number( const duf_sccb_data_row_t * row, const char *name );
const char *duf_datarow_get_string( duf_sccb_data_row_t * row, const char *name );

unsigned long long duf_sccbh_row_get_number( duf_sccb_handle_t * sccbh, const char *name );
const char *duf_sccbh_row_get_string( duf_sccb_handle_t * sccbh, const char *name );

void duf_sccbh_rows_eval( duf_sccb_handle_t * sccbh );

#endif

/*
vi: ft=c
*/
