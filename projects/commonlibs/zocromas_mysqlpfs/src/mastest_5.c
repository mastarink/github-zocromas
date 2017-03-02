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
#include "mysqlpfs_mstmt_d.h"

#include "mysqlpfs_structs.h"

static int
test5cb( const char *name, size_t depth, void *li _uUu_, void *mstmtv )
{
  mysqlpfs_mstmt_t *mstmt = ( mysqlpfs_mstmt_t * ) mstmtv;

  fprintf( stderr, "(T5) %ld. %s\n", depth, name );
  {
    int r = 0;

    if ( !r )
      r = mas_mysqlpfs_mstmt_set_direct_param_string( mstmt, 0, name );
    if ( !r )
      r = mas_mysqlpfs_mstmt_bind_param( mstmt );
    if ( !r )
      r = mas_mysqlpfs_mstmt_execute( mstmt );
  }
  return 0;
}

int
test5( void )
{
  mysqlpfs_t *pfs = mysqlpfs_create_setup( "mysql.mastar.lan", "masdufnt", "i2xV9KrTA54HRpj4e", "masdufntdb", 3306 );

  if ( pfs )
  {
    const char *path0 = "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_xfs/mastest";
    masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup( path0, 128 );

    if ( pi )
    {
      int r = 0;
      char *path = masxfs_pathinfo_pi2path( pi );
      char *insop _uUu_ = "INSERT INTO filenames(name) VALUES (?)";
      mysqlpfs_mstmt_t *mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 1,0, insop );

      if ( !r )
      {
        masxfs_pathinfo_each_depth_cb( pi, test5cb, mstmt );
      }
      mas_mysqlpfs_mstmt_delete( mstmt );

      EXAMS( path, path0, "%s : %s" );
      fprintf( stderr, "restored path:%s\n", path );
      mas_free( path );
      masxfs_pathinfo_delete( pi );
    }
    mysqlpfs_delete( pfs );
  }

  return 0;
}
