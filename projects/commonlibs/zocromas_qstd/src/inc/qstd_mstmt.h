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
int mas_qstd_mstmt_prepare_param_string( mysqlpfs_mstmt_t * mstmt, int pos, size_t buffer_length );

int mas_qstd_mstmt_set_param_longlong( mysqlpfs_mstmt_t * mstmt, int pos, unsigned long long num, unsigned is_null );

int mas_qstd_mstmt_data_seek( mysqlpfs_mstmt_t * mstmt, unsigned long long offset );

#endif
