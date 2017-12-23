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

#include <mastar/levinfo/masxfs_levinfo_enums.h>
#include <mastar/masxfs/masxfs_pathinfo_base.h>
#include <mastar/masxfs/masxfs_pathinfo.h>
#include <mastar/levinfo/masxfs_levinfo_ref.h>

#include "mysqlpfs.h"
#include "mysqlpfs_query.h"

#include "mysqlpfs_base.h"
#include "mysqlpfs_mstmt_base.h"
#include "mysqlpfs_mstmt.h"
#include "mysqlpfs_mstmt_d.h"

#include "mysqlpfs_structs.h"

static int _uUu_
test5cb( masxfs_levinfo_t * li _uUu_, unsigned long flags _uUu_, void *mstmtv, masxfs_depth_t reldepth _uUu_ )
{
  mysqlpfs_mstmt_t *mstmt = ( mysqlpfs_mstmt_t * ) mstmtv;
  masxfs_depth_t depth _uUu_ = masxfs_levinfo_depth_ref( li, flags );
  const char *ename _uUu_ = masxfs_levinfo_name_ref( li, flags );

  MARK( "(T5)", " %ld. %s", ( long ) depth, ename );
  {
    int r = 0;

    if ( !r )
      r = mas_mysqlpfs_mstmt_set_direct_param_string( mstmt, 0, ename );
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
    masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup( path0, 128, 0 );

    if ( pi )
    {
      int r = 0;
      char *path = masxfs_pathinfo_pi2path( pi );
      char *insop _uUu_ = "INSERT INTO pfs_test_filenames(name) VALUES (?)";
      mysqlpfs_mstmt_t *mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 1, 0, insop );

      if ( !r )
      {
      /*20170319.112815 masxfs_pathinfo_scan_depth_cbf( pi, test5cb, mstmt, MASXFS_CB_MODE_FS (* flags *)  ); */
      }
      mas_mysqlpfs_mstmt_delete( mstmt );

      EXAMS( path, path0, "%s : %s" );
      INFO( "restored path:%s", path );
      mas_free( path );
      masxfs_pathinfo_delete( pi, MASXFS_CB_MODE_ALL );
    }
    mysqlpfs_delete( pfs );
  }

  return 0;
}
