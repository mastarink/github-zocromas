#ifndef MAS_MYSQLPFS_BASE_H
# define MAS_MYSQLPFS_BASE_H

# include "mysqlpfs_types.h"

mysqlpfs_t *mysqlpfs_create( void );
int mysqlpfs_init( mysqlpfs_t * han, const char *host, const char *user, const char *passwd, const char *db, int port, const char *table_prefix );
mysqlpfs_t *mysqlpfs_create_setup( const char *host, const char *user, const char *passwd, const char *db, int port, const char *table_prefix  );
void mysqlpfs_reset( mysqlpfs_t * pfs );
void mysqlpfs_delete( mysqlpfs_t * pfs );

#endif
