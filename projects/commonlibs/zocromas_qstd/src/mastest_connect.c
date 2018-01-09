#include "qstd_defs.h"
#include <stdio.h>

#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>
#include <mastar/exam/masexam.h>

#include <mastar/mysqlpfs/mysqlpfs_base.h>

int
test_connect( void )
{
  mysqlpfs_t *pfs = mysqlpfs_create_setup( "mysql.mastar.lan", "masdufnt", "i2xV9KrTA54HRpj4e", "masdufntdb", 3306, NULL );

  if ( pfs )
  {
    mysqlpfs_delete( pfs );
  }

/* masregerr_print_simple_all( NULL, NULL ); */

  return 0;
}
