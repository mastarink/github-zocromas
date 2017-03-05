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
#include <mastar/masxfs/masxfs_levinfo_ref.h>

#include "mysqlpfs.h"
#include "mysqlpfs_query.h"

#include "mysqlpfs_base.h"
#include "mysqlpfs_mstmt_base.h"
#include "mysqlpfs_mstmt.h"
#include "mysqlpfs_mstmt_d.h"

#include "mysqlpfs_structs.h"

static int
test4cb( masxfs_levinfo_t * li _uUu_, unsigned long flags _uUu_, void *pfsv )
{
  mysqlpfs_t *_uUu_ pfs = ( mysqlpfs_t * ) pfsv;
  masxfs_depth_t depth _uUu_ = masxfs_levinfo_depth_ref( li, flags );
  const char *ename _uUu_ = masxfs_levinfo_name_ref( li, flags );

  MARK( "(T4)", " %ld. %s", depth, ename );
  {
    int r = 0;
    char *insop _uUu_ = "INSERT INTO filenames(name) VALUES (?)";

    {
      mysqlpfs_mstmt_t *mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 1, 0, insop );

    /* r = mas_mysqlpfs_mstmt_prepare( mstmt, insop ); */

      if ( !r )
        r = mas_mysqlpfs_mstmt_set_direct_param_string( mstmt, 0, ename );
      if ( !r )
        r = mas_mysqlpfs_mstmt_bind_param( mstmt );
      if ( !r )
        r = mas_mysqlpfs_mstmt_execute( mstmt );
      mas_mysqlpfs_mstmt_delete( mstmt );
    }
  }
  return 0;
}

int
test4( void )
{
  mysqlpfs_t *pfs = mysqlpfs_create_setup( "mysql.mastar.lan", "masdufnt", "i2xV9KrTA54HRpj4e", "masdufntdb", 3306 );

  if ( pfs )
  {
    const char *path0 = "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_xfs/mastest";
    masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup( path0, 128 );

    if ( pi )
    {
      char *path = masxfs_pathinfo_pi2path( pi );

      masxfs_pathinfo_scan_depth( pi, test4cb, pfs, 0L /* flags */  );

      EXAMS( path, path0, "%s : %s" );
      INFO( "restored path:%s", path );
      mas_free( path );
      masxfs_pathinfo_delete( pi );
    }
    mysqlpfs_delete( pfs );
  }

  return 0;
}