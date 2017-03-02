#include "mysqlpfs_defs.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/regerr/masregerr.h>
#include <mastar/exam/masexam.h>
#include <mastar/masxfs/masxfs_pathinfo_base.h>
#include <mastar/masxfs/masxfs_pathinfo.h>

#include "mysqlpfs.h"
#include "mysqlpfs_query.h"

#include "mysqlpfs_base.h"
#include "mysqlpfs_mstmt_base.h"
#include "mysqlpfs_mstmt.h"

#include "mysqlpfs_structs.h"

int
test3drop( void )
{
  const char *creops[] _uUu_ = {
    "START TRANSACTION",
    "DROP TABLE IF EXISTS filenames",
    "DROP TABLE IF EXISTS parents",
    "DROP TABLE IF EXISTS filedatas",
    "DROP TABLE IF EXISTS fileprops",
    "DROP TABLE IF EXISTS filesizes",
    "COMMIT",
  };
  mysqlpfs_t *pfs = mysqlpfs_create_setup( "mysql.mastar.lan", "masdufnt", "i2xV9KrTA54HRpj4e", "masdufntdb", 3306 );

  if ( pfs )
  {
    int r = 0;

    for ( size_t i = 0; i < sizeof( creops ) / sizeof( creops[0] ) && !r; i++ )
    {
      r = mas_mysqlpfs_query( pfs, creops[i] );
      fprintf( stderr, "(%d) %s\n", r, creops[i] );
    }
    mysqlpfs_delete( pfs );
  }

  return 0;
}
