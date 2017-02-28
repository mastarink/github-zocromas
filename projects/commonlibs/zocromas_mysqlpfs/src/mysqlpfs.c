#define RGEMSG mysql_error(&pfs->mysql)
#include "mysqlpfs_defs.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdlib.h>

#include <mysql.h>

#include <mastar/wrap/mas_memory.h>
/* #include <mastar/minierr/minierr.h> */
#include <mastar/regerr/masregerr.h>

#include "mysqlpfs_structs.h"
#include "mysqlpfs.h"

int
mas_mysqlpfs_query( mysqlpfs_t * pfs, const char *sql )
{
  int r = -1;

  QRGP( pfs );
  QRGP( sql );
  if ( pfs && sql && *sql )
  {
    r = mysql_query( &pfs->mysql, sql );
    QRGS( r );
  }
  return r;
}

mysqlpfs_result_t *
mas_mysqlpfs_result( mysqlpfs_t * pfs )
{
  mysqlpfs_result_t *result = NULL;

  QRGP( pfs );
  if ( pfs )
  {
#if 1
    result = mysql_store_result( &pfs->mysql );
    pfs->result = result;
#else
    pfs->result = result = mysql_use_result( &pfs->mysql );
#endif
    QRGSP( result );
  }
  return result;
}

mysqlpfs_result_t *
mas_mysqlpfs_query_result( mysqlpfs_t * pfs, const char *sql )
{
  mysqlpfs_result_t *result = NULL;

  QRGP( pfs );
  if ( pfs )
  {
    int r = -1;

    r = mas_mysqlpfs_query( pfs, sql );
    if ( !r )
      result = mas_mysqlpfs_result( pfs );
    QRGSP( result );
  }
  return result;
}

void
mas_mysqlpfs_query_result_cb( mysqlpfs_t * pfs, const char *sql, mysqlpfs_row_cb_t cb )
{
  QRGP( pfs );
  if ( pfs )
  {
    int num = 0;
    mysqlpfs_row_t row = NULL;

    mas_mysqlpfs_query_result( pfs, sql );

    while ( ( row = mas_mysqlpfs_fetch_row( pfs ) ) )
    {
      cb( row, num );
      num++;
    }

    mas_mysqlpfs_free_result( pfs );
  }
}

mysqlpfs_row_t
mas_mysqlpfs_fetch_row( mysqlpfs_t * pfs )
{
  mysqlpfs_row_t row = NULL;

  QRGP( pfs );
  if ( pfs )
  {
    QRGP( pfs->result );
    if ( pfs->result )
      row = mysql_fetch_row( pfs->result );
  }
  return row;
}

void
mas_mysqlpfs_free_result( mysqlpfs_t * pfs )
{
  if ( pfs )
  {
    if ( pfs->result )
      mysql_free_result( pfs->result );
    pfs->result = NULL;
  }
}
