#include "qstd_defs.h"
#include <stdio.h>

#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>
#include <mastar/exam/masexam.h>

#include <mastar/mysqlpfs/mysqlpfs_defs.h>
#include <mastar/mysqlpfs/mysqlpfs_query.h>

#include <mastar/mysqlpfs/mysqlpfs_base.h>

int
test_drop_tables( void )
{
  const char *creops[] _uUu_ = {
    "START TRANSACTION",
    "DROP VIEW IF EXISTS qstd_test_filefull",
    "DROP TABLE IF EXISTS qstd_test_filenames",
    "DROP TABLE IF EXISTS qstd_test_parents",
    "DROP TABLE IF EXISTS qstd_test_fileprops",
    "DROP TABLE IF EXISTS qstd_test_filedatas",
    "DROP TABLE IF EXISTS qstd_test_filesizes",
    "COMMIT",
  };
  mysqlpfs_t *pfs = mysqlpfs_create_setup( "mysql.mastar.lan", "masdufnt", "i2xV9KrTA54HRpj4e", "masdufntdb", 3306, NULL );

  if ( pfs )
  {
    int r = 0;

    for ( size_t i = 0; i < sizeof( creops ) / sizeof( creops[0] ) && !r; i++ )
    {
      r = mas_mysqlpfs_query( pfs, creops[i] );
      INFO( "(%d) %s", r, creops[i] );
    }
    mysqlpfs_delete( pfs );
  }

  return 0;
}
