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
#include "mysqlpfs_mstmt_std.h"
#include "mysqlpfs_mstmt.h"

#include "mysqlpfs_structs.h"

static int _uUu_
test6icb( const char *name, size_t depth, masxfs_levinfo_t * li, void *pfsv )
{
  mysqlpfs_t *pfs = ( mysqlpfs_t * ) pfsv;

#if 1
  mysqlpfs_s_ulonglong_t theid = 0;
  mysqlpfs_s_ulonglong_t parent_id = masxfs_levinfo_parent_id( li );

  theid = mysqlpfs_mstmt_std_insget_names_id( pfs, name, parent_id );
  masxfs_levinfo_set_id( li, theid );
  MARK( "(T6)", " %ld. %s ID: %llu", depth, name, ( unsigned long long ) theid );
#else
  mysqlpfs_mstmt_t *mstmt = mysqlpfs_mstmt_std_get( pfs, STD_MSTMT_INSERT_NAMES );

  {
    mysqlpfs_s_ulonglong_t parent_id = 0, oldid = 0;
    int r = 0;

    parent_id = masxfs_levinfo_parent_id( li );
    QRGP( mstmt );
    oldid = mysqlpfs_mstmt_std_get_names_id( pfs, name, parent_id );
    if ( oldid )
    {
      masxfs_levinfo_set_id( li, oldid );
    }
    else
    {
      if ( !r )
        r = mas_mysqlpfs_mstmt_set_param_string( mstmt, 0, name );
      if ( !r )
        r = mas_mysqlpfs_mstmt_set_param_longlong( mstmt, 1, parent_id, parent_id ? FALSE : TRUE );
      if ( !r )
      {
        mysqlpfs_s_ulonglong_t insert_id = 0;
        mysqlpfs_s_ulonglong_t affected = 0;

        r = mas_mysqlpfs_mstmt_execute( mstmt );
        {
          affected = mas_mysqlpfs_mstmt_affected_rows( mstmt );

          if ( affected == 1 )
          {
            insert_id = mas_mysqlpfs_mstmt_insert_id( mstmt );
            if ( liv )
              masxfs_levinfo_set_id( li, insert_id );
          }
          MARK( "(T6)", " %ld. %s r:%d AFFECTED: %llu :: (%llu) %llu", depth, name, r, ( unsigned long long ) affected, ( unsigned long long ) oldid,
                ( unsigned long long ) insert_id );
        }
      }
    }
  }
#endif
  return 0;
}

int
test6( void )
{
  mysqlpfs_t *pfs = mysqlpfs_create_setup( "mysql.mastar.lan", "masdufnt", "i2xV9KrTA54HRpj4e", "masdufntdb", 3306 );

  if ( pfs )
  {
    const char *path0 = "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_xfs/mastest";
    masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup( path0, 128 );

    if ( pi )
    {
      rSET( 0 );
      rC( masxfs_pathinfo_each_depth_cb( pi, test6icb, pfs ) );

      {
        mysqlpfs_s_ulonglong_t theid = 0;

        theid = mysqlpfs_mstmt_std_get_names_id( pfs, "zocromas_xfs", 12 );
        MARK( "RESULT", ": theid:%lld", theid );
      }

      {
        char *path = masxfs_pathinfo_pi2path( pi );

        EXAMS( path, path0, "%s : %s" );
        WARN( "restored path:%s", path );
        mas_free( path );
      }
      masxfs_pathinfo_delete( pi );
    }
    mysqlpfs_delete( pfs );
  }

  return 0;
}
