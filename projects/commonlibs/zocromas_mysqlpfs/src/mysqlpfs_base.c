#define R_GOOD(_r) (!(_r))
#define RGEMSG mysql_error(mas_mysqlpfs_mysql(pfs))
/* #define RGEMSG mysql_error(&pfs->mysql) */
#include "mysqlpfs_defs.h"
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>

#include "mysqlpfs_structs.h"
#include "mysqlpfs.h"

#include "mysqlpfs_base.h"

mysqlpfs_t *
mysqlpfs_create( void )
{
  mysqlpfs_t *pfs = mas_calloc( 1, sizeof( mysqlpfs_t ) );

  QRGP( pfs );
  return pfs;
}

int
mysqlpfs_init( mysqlpfs_t * pfs, const char *host, const char *user, const char *passwd, const char *db, int port, const char *table_prefix )
{
  int r = 0;
  MYSQL *mysql = NULL;

/* client_flag |= CLIENT_MULTI_STATEMENTS; */

  mysql = mysql_real_connect( &pfs->mysql, host, user, passwd, db, port, NULL /* unix_socket */ , 0 /* client_flag */  );
  QRGSP( mysql );
  if ( mysql )
  {
    pfs->inited = 1;
    pfs->table_prefix = mas_strdup( table_prefix );
  }
  else
  {
    mysqlpfs_reset( pfs );
    r = -1;
  }

/* WARN( "MYSQL: %p : %d", mysql, r ); */
  return r;
}

mysqlpfs_t *
mysqlpfs_create_setup( const char *host, const char *user, const char *passwd, const char *db, int port, const char *table_prefix )
{
  rDECLBAD;
  mysqlpfs_t *pfs = mysqlpfs_create(  );

  rC( mysqlpfs_init( pfs, host, user, passwd, db, port, table_prefix ) );

  if ( rCODE < 0 )
  {
  /* mas_free( pfs ); */
    mysqlpfs_delete( pfs );
    pfs = NULL;
  }
  return pfs;
}

void
mysqlpfs_reset( mysqlpfs_t * pfs )
{
  if ( pfs )
  {
    if ( pfs->inited )
      mysql_close( &pfs->mysql );
    mas_free( pfs->table_prefix );
    memset( pfs, 0, sizeof( mysqlpfs_t ) );
  }
}

void
mysqlpfs_delete( mysqlpfs_t * pfs )
{
  mysqlpfs_reset( pfs );
  if ( pfs )
    mas_free( pfs );
}
