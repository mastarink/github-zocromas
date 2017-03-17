#ifndef MAS_QSTD_MSTMT_H
# define MAS_QSTD_MSTMT_H

# include "qstd_types.h"

int mas_qstd_query( mas_qstd_t * qstd, const char *op );

/* mysqlpfs_s_mysql_t *mas_qstd_mysql( mas_qstd_t * qstd ); */
const char *mas_qstd_mysql_error( mas_qstd_t * qstd );
int mas_qstd_update_summary( mas_qstd_t * qstd );

int mas_qstd_start_transaction( mas_qstd_t * qstd );
int mas_qstd_end_transaction( mas_qstd_t * qstd );

int mas_qstd_mstmt_prepare_param_longlong( mysqlpfs_mstmt_t * mstmt, int pos );
int mas_qstd_mstmt_prepare_param_string( mysqlpfs_mstmt_t * mstmt, int pos );
int mas_qstd_mstmt_prepare_result_longlong( mysqlpfs_mstmt_t * mstmt, int pos );
int mas_qstd_mstmt_prepare_result_string( mysqlpfs_mstmt_t * mstmt, int po );

int mas_qstd_mstmt_set_param_longlong( mysqlpfs_mstmt_t * mstmt, int pos, unsigned long long num, unsigned is_null );
int mas_qstd_mstmt_set_param_string( mysqlpfs_mstmt_t * mstmt, int pos, const char *string );

int mas_qstd_mstmt_data_seek( mysqlpfs_mstmt_t * mstmt, unsigned long long offset );

int mas_qstd_mstmt_execute_store( mysqlpfs_mstmt_t * mstmt );
int mas_qstd_mstmt_fetch( mysqlpfs_mstmt_t * mstmt, int *phas_data );

int mas_qstd_mstmt_get_result_longlong( mysqlpfs_mstmt_t * mstmt, int pos, unsigned long long *pnum, unsigned *pis_null );
int mas_qstd_mstmt_get_result_string_na( mysqlpfs_mstmt_t * mstmt, int pos, const char **pstring );

int mas_qstd_mstmt_bind_param( mysqlpfs_mstmt_t * mstmt );
int mas_qstd_mstmt_bind_result( mysqlpfs_mstmt_t * mstmt );

#endif
