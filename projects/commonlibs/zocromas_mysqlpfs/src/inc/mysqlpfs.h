#ifndef MAS_MYSQLPFS_H
# define MAS_MYSQLPFS_H

# include "mysqlpfs_types_my.h"
# include "mysqlpfs_types.h"

mysqlpfs_s_mysql_t *mas_mysqlpfs_mysql( mysqlpfs_t * pfs );
const char *mas_mysqlpfs_mysql_error( mysqlpfs_t * pfs );
const char *mas_mysqlpfs_expand_sqlop( const char *name, const char *arg );

#endif
