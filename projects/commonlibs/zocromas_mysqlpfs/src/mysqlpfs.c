#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdlib.h>

#include <mysql.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>


#include "mysqlpfs_defs.h"
#include "mysqlpfs_structs.h"
#include "mysqlpfs.h"

int
mas_mysqlpfs( void )
{
  return 0;
}

int
mas_mysqlpfs_query( mysqlpfs_t * pfs, const char *sql )
{
  int r = 0;

  if ( pfs && sql && *sql )
    r = mysql_query( &pfs->mysql, sql );
  else
    r = -1;
  return r;
}

MYSQL_RES *
mas_mysqlpfs_result( mysqlpfs_t * pfs )
{
  MYSQL_RES *result = NULL;

  if ( pfs )
  {
#if 1
    result = mysql_store_result( &pfs->mysql );
#else
    result = mysql_use_result( &pfs->mysql );
#endif
    if ( !result )
    {
      QRG(-1);
    }
  }
  return result;
}

MYSQL_RES *
mas_mysqlpfs_query_result( mysqlpfs_t * pfs, const char *sql )
{
  int r = 0;
  MYSQL_RES *result = NULL;

  if ( pfs )
  {
    r = mas_mysqlpfs_query( pfs, sql );
    if ( !r )
      result = mas_mysqlpfs_result( pfs );
  }
  else
    r = -1;
  return result;
}
