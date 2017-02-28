#include "mysqlpfs_defs.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdlib.h>

#include <mysql.h>
#include <my_net.h>

#include <mastar/regerr/masregerr.h>

#include "mysqlpfs_base.h"
#include "mysqlpfs.h"

static int
row_cb( mysqlpfs_row_t row, int num )
{
  printf( "%d #X# %s: %s\n", num, row[0], row[1] );
  return 0;
}

int
test1( void )
{
  mysqlpfs_t *pfs = mysqlpfs_create_setup( "mysql.mastar.lan", "masdufnt", "i2xV9KrTA54HRpj4e", "masdufntdb", 3306 );

  {
    mysqlpfs_row_t row;
    int num = 0;

    mas_mysqlpfs_query_result( pfs, "SHOW STATUS" );
    while ( ( row = mas_mysqlpfs_fetch_row( pfs ) ) )
    {
      printf( "%d #X# %s: %s\n", num, row[0], row[1] );
      num++;
    }

    mas_mysqlpfs_free_result( pfs );
  }
  mysqlpfs_delete( pfs );

/* masregerr_print_simple_all( NULL, NULL ); */

  return 0;
}

int
test2( void )
{
  mysqlpfs_t *pfs = mysqlpfs_create_setup( "mysql.mastar.lan", "masdufnt", "wrong_i2xV9KrTA54HRpj4e", "masdufntdb", 3306 );

  /* fprintf( stderr, "PFS:%p\n", pfs ); */
  if ( pfs )
  {
    mas_mysqlpfs_query_result_cb( pfs, "SHOW STATUS", row_cb );

    mysqlpfs_delete( pfs );
  }
/* masregerr_print_simple_all( NULL, NULL ); */

  return 0;
}

int
main( int argc __attribute__ ( ( unused ) ), char *argv[] __attribute__ ( ( unused ) ) )
{
/* test1(  ); */
  test2(  );
  return 0;
}
