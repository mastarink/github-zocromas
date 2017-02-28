#ifndef MAS_MYSQLPFS_H
# define MAS_MYSQLPFS_H

# include "mysqlpfs_types.h"

int mas_mysqlpfs( void );
int mas_mysqlpfs_query( mysqlpfs_t * pfs, const char *sql );
MYSQL_RES *mas_mysqlpfs_result( mysqlpfs_t * pfs );
MYSQL_RES *mas_mysqlpfs_query_result( mysqlpfs_t * pfs, const char *sql );

#endif
