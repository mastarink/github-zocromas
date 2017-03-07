#include "mysqlpfs_defs.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>
#include <mastar/exam/masexam.h>
#include <mastar/masxfs/masxfs_pathinfo_base.h>
#include <mastar/masxfs/masxfs_pathinfo.h>

#include "mysqlpfs.h"
#include "mysqlpfs_query.h"
#include "mysqlpfs_query_result.h"

#include "mysqlpfs_base.h"
#include "mysqlpfs_mstmt_base.h"
#include "mysqlpfs_mstmt.h"

#include "mysqlpfs_structs.h"

int
test1status( void )
{
  mysqlpfs_t *pfs = mysqlpfs_create_setup( "mysql.mastar.lan", "masdufnt", "i2xV9KrTA54HRpj4e", "masdufntdb", 3306 );

  if ( pfs )
  {
    mysqlpfs_s_row_t row;
    int num = 0;

    mas_mysqlpfs_query_result( pfs, "SHOW STATUS" );
    while ( ( row = mas_mysqlpfs_fetch_row( pfs ) ) )
    {
      printf( "%d #X# %s: %s\n", num, row[0], row[1] );
      num++;
    }

    mas_mysqlpfs_free_result( pfs );
    mysqlpfs_delete( pfs );
  }

/* masregerr_print_simple_all( NULL, NULL ); */

  return 0;
}
