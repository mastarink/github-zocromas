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
#include <mastar/masxfs/masxfs_levinfo_ref.h>

#include "mysqlpfs.h"
#include "mysqlpfs_query.h"

#include "mysqlpfs_base.h"
#include "mysqlpfs_mstmt_base.h"
#include "mysqlpfs_mstmt.h"

#include "mysqlpfs_structs.h"

static int
test6cb( const char *name, size_t depth, void *liv _uUu_, void *mstmtv )
{
  mysqlpfs_mstmt_t *mstmt = ( mysqlpfs_mstmt_t * ) mstmtv;
  masxfs_levinfo_t *li = ( masxfs_levinfo_t * ) liv;

  {
    my_ulonglong id = 0;
    int r = 0;

    id = masxfs_levinfo_parent_id( li );
    if ( !r )
      r = mas_mysqlpfs_mstmt_set_param_string( mstmt, 0, name );
    if ( !r )
      r = mas_mysqlpfs_mstmt_set_param_longlong( mstmt, 1, id, id ? 0 : 1 );
    if ( !r )
    {
      my_ulonglong insert_id = 0;
      my_ulonglong affected = 0;

      r = mas_mysqlpfs_mstmt_execute( mstmt );
      {
        affected = mas_mysqlpfs_mstmt_affected_rows( mstmt );

        if ( affected == 1 )
        {
          insert_id = mas_mysqlpfs_mstmt_insert_id( mstmt );
          if ( liv )
          {

            masxfs_levinfo_set_id( li, insert_id );
          }
        }
        fprintf( stderr, "(T6) %ld. %s r:%d AFFECTED: %llu :: %llu\n", depth, name, r,
                 ( unsigned long long ) affected, ( unsigned long long ) insert_id );
      }
    }
  }
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
      int r = 0;
      char *path = masxfs_pathinfo_pi2path( pi );
      char *insop _uUu_ = "INSERT INTO filenames(name,parent_id) VALUES (?,?)";
      mysqlpfs_mstmt_t *mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 2, insop );

      if ( !r )
        mas_mysqlpfs_mstmt_prepare_param_string( mstmt, 0, 255 );
      if ( !r )
        mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 1 );
      if ( !r )
        r = mas_mysqlpfs_mstmt_bind( mstmt );

      if ( !r )
        masxfs_pathinfo_each_depth_cb( pi, test6cb, mstmt );

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
