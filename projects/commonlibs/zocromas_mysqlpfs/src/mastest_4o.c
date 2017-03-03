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

#include "mysqlpfs_base.h"
#include "mysqlpfs_mstmt_base.h"
#include "mysqlpfs_mstmt.h"

#include "mysqlpfs_structs.h"

static int
test4ocb( const char *name, size_t depth, masxfs_levinfo_t *li _uUu_, void *pfsv )
{
  mysqlpfs_t *_uUu_ pfs = ( mysqlpfs_t * ) pfsv;

  INFO( "%ld. %s", depth, name );
#if 0
  char *insops[] _uUu_ = {
    "PREPARE insname_stmt FROM 'INSERT INTO filenames SET name=?'"
          /* INSERT INTO filesizes */
          /* INSERT INTO fileprops */
          /* INSERT INTO filedatas */
            "EXECUTE insname_stmt USING @file_name",
    "DEALLOCATE PREPARE insname_stmt",
  };
#endif
  {
    int r = 0;
    char *insop = "INSERT INTO filenames(name) VALUES (?)";
    MYSQL_STMT *stmt = mysql_stmt_init( mas_mysqlpfs_mysql( pfs ) );

    unsigned long length = strlen( name );
    MYSQL_BIND param[1], result[1];

    r = mysql_stmt_prepare( stmt, insop, strlen( insop ) );
    INFO( "PREPARE: %d", r );
    memset( param, 0, sizeof( param ) );
    memset( result, 0, sizeof( result ) );

    param[0].buffer_type = MYSQL_TYPE_STRING;
    param[0].buffer = ( char * ) name;
    param[0].buffer_length = 255;
  /* param[0].is_unsigned = 0; */
    param[0].is_null = 0;
    param[0].length = &length;

    r = mysql_stmt_bind_param( stmt, param );
    INFO( "BIND: %d", r );

    r = mysql_stmt_execute( stmt );
    INFO( "EXECUTE: %d", r );
/* mariadb_stmt_execute_direct(); */
#if 0
    mysql_stmt_reset(  );
#else
    r = mysql_stmt_close( stmt );
    INFO( "CLOSE: %d", r );
#endif
  /* mysql_stmt_bind_result
   * mysql_stmt_fetch
   * */
  }
#if 0
  if ( pfs )
  {
    int r = 0;

    for ( size_t i = 0; i < sizeof( insops ) / sizeof( insops[0] ) && !r; i++ )
    {
    /* r = mas_mysqlpfs_query( pfs, insops[i] ); */
      INFO( "(%d) %s", r, insops[i] );
    }
  }
#endif
  return 0;
}

int
test4o( void )
{
  mysqlpfs_t *pfs = mysqlpfs_create_setup( "mysql.mastar.lan", "masdufnt", "i2xV9KrTA54HRpj4e", "masdufntdb", 3306 );

  if ( pfs )
  {
    const char *path0 = "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_xfs/mastest";
    masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup( path0, 128 );

    if ( pi )
    {
      char *path = masxfs_pathinfo_pi2path( pi );

      masxfs_pathinfo_each_depth_cb( pi, test4ocb, pfs );

      EXAMS( path, path0, "%s : %s" );
      INFO( "restored path:%s", path );
      mas_free( path );
      masxfs_pathinfo_delete( pi );
    }
    mysqlpfs_delete( pfs );
  }

  return 0;
}
