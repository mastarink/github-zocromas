#ifndef MAS_DUF_SCCB_ROW_H
# define MAS_DUF_SCCB_ROW_H

duf_sccb_data_row_t duf_sccb_row_create( duf_stmnt_t * pstmt );
void duf_sccb_row_delete( duf_sccb_data_row_t row );

duf_sccb_data_value_t *duf_sccb_row_field_find( duf_sccb_data_row_t row, const char *name );
unsigned long long duf_sccb_row_get_number( duf_sccb_data_row_t row, const char *name );
const char *duf_sccb_row_get_string( duf_sccb_data_row_t row, const char *name );

unsigned long long duf_sccbh_row_get_number( duf_sccb_handle_t * sccbh, const char *name );
unsigned long long duf_sccbh_prevrow_get_number( duf_sccb_handle_t * sccbh, const char *name );
const char *duf_sccbh_row_get_string( duf_sccb_handle_t * sccbh, const char *name );
const char *duf_sccbh_prevrow_get_string( duf_sccb_handle_t * sccbh, const char *name );

#endif

/*
vi: ft=c
*/
