#include <string.h>

#include <my_global.h>
#include <mysql.h>

#include <mastar/wrap/mas_memory.h>

#include "mysqlpfs_defs.h"
#include "mysqlpfs_structs.h"
#include "mysqlpfs_base.h"

mysqlpfs_t *
mysqlpfs_create( void )
{
  mysqlpfs_t *pfs = mas_calloc( 1, sizeof( mysqlpfs_t ) );

  return pfs;
}

void
mysqlpfs_init( mysqlpfs_t * pfs, const char *host, const char *user, const char *passwd, const char *db, int port )
{
  MYSQL *mysql _uUu_ = NULL;

  mysql = mysql_real_connect( &pfs->mysql, host, user, passwd, db, port, NULL, 0 );
#if 0
  if ( !mysql )
  {
    fprintf( stderr, "%s\n", mysql_error( &mysql ) );
    mysqlpfs_reset( pfs );
    exit( 1 );
  }
#endif
}

mysqlpfs_t *
mysqlpfs_create_setup( const char *host, const char *user, const char *passwd, const char *db, int port )
{
  mysqlpfs_t *pfs = mysqlpfs_create(  );

  mysqlpfs_init( pfs, host, user, passwd, db, port );
  return pfs;
}

void
mysqlpfs_reset( mysqlpfs_t * pfs )
{
  if ( pfs )
    mysql_close( &pfs->mysql );
  memset( pfs, 0, sizeof( mysqlpfs_t ) );
}

void
mysqlpfs_delete( mysqlpfs_t * pfs )
{
  mysqlpfs_reset( pfs );
  mas_free( pfs );
}
