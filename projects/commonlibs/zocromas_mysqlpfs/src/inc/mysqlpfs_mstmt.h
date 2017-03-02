#ifndef MAS_MYSQLPFS_STMT_H
# define MAS_MYSQLPFS_STMT_H

# include "mysqlpfs_types.h"

int mas_mysqlpfs_mstmt_prepare( mysqlpfs_mstmt_t * mstmt, const char *op );
int mas_mysqlpfs_mstmt_unprepare( mysqlpfs_mstmt_t * mstmt );

int mas_mysqlpfs_mstmt_set_direct_param_string( mysqlpfs_mstmt_t * mstmt, int pos, const char *str );
int mas_mysqlpfs_mstmt_prepare_param_string( mysqlpfs_mstmt_t * mstmt, int pos, mysqlpfs_s_length_t buffer_length );
int mas_mysqlpfs_mstmt_set_param_string( mysqlpfs_mstmt_t * mstmt, int pos, const char *string );

int mas_mysqlpfs_mstmt_bind( mysqlpfs_mstmt_t * mstmt );
int mas_mysqlpfs_mstmt_execute( mysqlpfs_mstmt_t * mstmt );

my_ulonglong mas_mysqlpfs_mstmt_affected_rows( mysqlpfs_mstmt_t * mstmt );

#endif
