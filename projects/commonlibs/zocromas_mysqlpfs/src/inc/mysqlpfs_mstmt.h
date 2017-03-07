#ifndef MAS_MYSQLPFS_STMT_H
# define MAS_MYSQLPFS_STMT_H

# include "mysqlpfs_types.h"

int mas_mysqlpfs_mstmt_prepare( mysqlpfs_mstmt_t * mstmt, const char *op );
int mas_mysqlpfs_mstmt_unprepare( mysqlpfs_mstmt_t * mstmt );

int mas_mysqlpfs_mstmt_prepare_param_string( mysqlpfs_mstmt_t * mstmt, int pos, mysqlpfs_s_length_t buffer_length );
int mas_mysqlpfs_mstmt_prepare_result_string( mysqlpfs_mstmt_t * mstmt, int pos, mysqlpfs_s_length_t buffer_length );

int mas_mysqlpfs_mstmt_set_param_string( mysqlpfs_mstmt_t * mstmt, int pos, const char *string );
int mas_mysqlpfs_mstmt_get_param_string( mysqlpfs_mstmt_t * mstmt, int pos, const char **pstring );
int mas_mysqlpfs_mstmt_set_result_string( mysqlpfs_mstmt_t * mstmt, int pos, const char *string );
int mas_mysqlpfs_mstmt_get_result_string( mysqlpfs_mstmt_t * mstmt, int pos, const char **pstring );

int mas_mysqlpfs_mstmt_prepare_param_longlong( mysqlpfs_mstmt_t * mstmt, int pos );
int mas_mysqlpfs_mstmt_prepare_result_longlong( mysqlpfs_mstmt_t * mstmt, int pos );

int mas_mysqlpfs_mstmt_set_param_longlong( mysqlpfs_mstmt_t * mstmt, int pos, unsigned long long num, unsigned is_null );
int mas_mysqlpfs_mstmt_get_param_longlong( mysqlpfs_mstmt_t * mstmt, int pos, unsigned long long *pnum, unsigned *pis_null );
int mas_mysqlpfs_mstmt_set_result_longlong( mysqlpfs_mstmt_t * mstmt, int pos, unsigned long long num, unsigned is_null );
int mas_mysqlpfs_mstmt_get_result_longlong( mysqlpfs_mstmt_t * mstmt, int pos, unsigned long long *pnum, unsigned *pis_null );

int mas_mysqlpfs_mstmt_bind_param( mysqlpfs_mstmt_t * mstmt );
int mas_mysqlpfs_mstmt_bind_result( mysqlpfs_mstmt_t * mstmt );

int mas_mysqlpfs_mstmt_execute( mysqlpfs_mstmt_t * mstmt );
int mas_mysqlpfs_mstmt_execute_store( mysqlpfs_mstmt_t * mstmt );

unsigned long long mas_mysqlpfs_mstmt_num_rows( mysqlpfs_mstmt_t * mstmt );
unsigned long long mas_mysqlpfs_mstmt_affected_rows( mysqlpfs_mstmt_t * mstmt );
unsigned long long mas_mysqlpfs_mstmt_insert_id( mysqlpfs_mstmt_t * mstmt );
int mas_mysqlpfs_mstmt_store_result( mysqlpfs_mstmt_t * mstmt );
int mas_mysqlpfs_mstmt_fetch( mysqlpfs_mstmt_t * mstmt );
int mas_mysqlpfs_mstmt_free_result( mysqlpfs_mstmt_t * mstmt );

int mas_mysqlpfs_mstmt_data_seek( mysqlpfs_mstmt_t * mstmt, unsigned long long offset );

unsigned int mas_mysqlpfs_mstmt_errno( mysqlpfs_mstmt_t * mstmt );
const char *mas_mysqlpfs_mstmt_error( mysqlpfs_mstmt_t * mstmt );

int mas_mysqlpfs_mstmt_is_no_data( int code );
#endif
