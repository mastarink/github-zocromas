#ifndef MAS_QSTD_MSTMT_H
# define MAS_QSTD_MSTMT_H

# include "qstd_types.h"

int mas_qstd_mstmt_prepare_param_longlong( mysqlpfs_mstmt_t * mstmt, int pos );
int mas_qstd_mstmt_prepare_param_string( mysqlpfs_mstmt_t * mstmt, int pos );
int mas_qstd_mstmt_prepare_result_longlong( mysqlpfs_mstmt_t * mstmt, int pos );
int mas_qstd_mstmt_prepare_result_string( mysqlpfs_mstmt_t * mstmt, int po );

int mas_qstd_mstmt_set_param_longlong( mysqlpfs_mstmt_t * mstmt, int pos, unsigned long long num, unsigned is_null );
int mas_qstd_mstmt_set_param_string( mysqlpfs_mstmt_t * mstmt, int pos, const char *string );
int mas_qstd_mstmt_set_param_binary( mysqlpfs_mstmt_t * mstmt, int pos, const unsigned char *binary, size_t length );

int mas_qstd_mstmt_data_seek( mysqlpfs_mstmt_t * mstmt, unsigned long long offset );

int mas_qstd_mstmt_execute( mysqlpfs_mstmt_t * mstmt );
int mas_qstd_mstmt_execute_store( mysqlpfs_mstmt_t * mstmt );
int mas_qstd_mstmt_fetch( mysqlpfs_mstmt_t * mstmt, int *phas_data );
int mas_qstd_mstmt_fetch_t( mysqlpfs_mstmt_t * mstmt, int *phas_data, int *truncated );

int mas_qstd_mstmt_get_result_longlong( mysqlpfs_mstmt_t * mstmt, int pos, unsigned long long *pnum, unsigned *pis_null );
int mas_qstd_mstmt_get_result_string_na( mysqlpfs_mstmt_t * mstmt, int pos, const char **pstring );

int mas_qstd_mstmt_bind_param( mysqlpfs_mstmt_t * mstmt );
int mas_qstd_mstmt_bind_result( mysqlpfs_mstmt_t * mstmt );

int mas_qstd_mstmt_free_result( mysqlpfs_mstmt_t * mstmt );
int mas_qstd_mstmt_insert_id( mysqlpfs_mstmt_t * mstmt );
unsigned long long mas_qstd_mstmt_affected_rows( mysqlpfs_mstmt_t * mstmt );
const char *mas_qstd_mstmt_error( mysqlpfs_mstmt_t * mstmt );
int mas_qstd_mstmt_ret_code( mysqlpfs_mstmt_t * mstmt );

#endif
