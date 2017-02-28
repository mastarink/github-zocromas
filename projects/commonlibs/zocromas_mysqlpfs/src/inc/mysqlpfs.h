#ifndef MAS_MYSQLPFS_H
# define MAS_MYSQLPFS_H

# include "mysqlpfs_types.h"

int mas_mysqlpfs_query( mysqlpfs_t * pfs, const char *sql );
mysqlpfs_result_t *mas_mysqlpfs_result( mysqlpfs_t * pfs );

mysqlpfs_result_t *mas_mysqlpfs_query_result( mysqlpfs_t * pfs, const char *sql );
void mas_mysqlpfs_query_result_cb( mysqlpfs_t * pfs, const char *sql, mysqlpfs_row_cb_t cb );

mysqlpfs_row_t mas_mysqlpfs_fetch_row( mysqlpfs_t * pfs );
void mas_mysqlpfs_free_result( mysqlpfs_t * pfs );

#endif
