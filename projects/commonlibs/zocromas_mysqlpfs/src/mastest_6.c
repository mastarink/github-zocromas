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

#if 0
static int
test6scb( const char *name, size_t depth _uUu_, void *liv, void *mstmtv )
{
  int r = 0;
  mysqlpfs_mstmt_t *mstmt = ( mysqlpfs_mstmt_t * ) mstmtv;
  masxfs_levinfo_t *li = ( masxfs_levinfo_t * ) liv;
  mysqlpfs_s_ulonglong_t id = 0;

  id = masxfs_levinfo_parent_id( li );
  if ( !r )
    r = mas_mysqlpfs_mstmt_set_param_string( mstmt, 0, name );
  if ( !r )
    r = mas_mysqlpfs_mstmt_set_param_longlong( mstmt, 1, id, id ? FALSE : TRUE );
  if ( !r )
    r = mas_mysqlpfs_mstmt_execute( mstmt );

  if ( !r )
    r = mas_mysqlpfs_mstmt_bind_result( mstmt );
  if ( !r )
    r = mas_mysqlpfs_mstmt_store_result( mstmt );
  if ( !r )
    r = mas_mysqlpfs_mstmt_fetch( mstmt );
  if ( !r )
  {
  /* fprintf( stderr, "%d\n",  ); */
  }
  return 0;
}
#endif

static int
test6icb( const char *name, size_t depth, void *liv _uUu_, void *mstmtv )
{
  mysqlpfs_mstmt_t *mstmt = ( mysqlpfs_mstmt_t * ) mstmtv;
  masxfs_levinfo_t *li = ( masxfs_levinfo_t * ) liv;

  {
    mysqlpfs_s_ulonglong_t parent_id = 0;
    int r = 0;

    parent_id = masxfs_levinfo_parent_id( li );
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

      {
        char *insop _uUu_ = "INSERT INTO filenames(name,parent_id) VALUES (?,?)";
        mysqlpfs_mstmt_t *mstmt_i = mas_mysqlpfs_mstmt_create_setup( pfs, 2, 0, insop );

        if ( !r )
          mas_mysqlpfs_mstmt_prepare_param_string( mstmt_i, 0, 255 );
        if ( !r )
          mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt_i, 1 );
        if ( !r )
          r = mas_mysqlpfs_mstmt_bind_param( mstmt_i );

        if ( !r )
          masxfs_pathinfo_each_depth_cb( pi, test6icb, mstmt_i );

        mas_mysqlpfs_mstmt_delete( mstmt_i );
      }

      {
        char *selop _uUu_ = "SELECT id FROM filenames WHERE name=? AND parent_id<=>?";
        mysqlpfs_mstmt_t *mstmt_s = mas_mysqlpfs_mstmt_create_setup( pfs, 2, 1, selop );

        if ( !r )
          mas_mysqlpfs_mstmt_prepare_param_string( mstmt_s, 0, 255 );
        if ( !r )
          mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt_s, 1 );
        if ( !r )
          r = mas_mysqlpfs_mstmt_bind_param( mstmt_s );

        if ( !r )
          mas_mysqlpfs_mstmt_prepare_result_longlong( mstmt_s, 0 );
        if ( !r )
          r = mas_mysqlpfs_mstmt_bind_result( mstmt_s );

        if ( !r )
        {
        /* mas_mysqlpfs_mstmt_set_param_string( mstmt_s, 0, "zocromas_xfs" );               */
        /* mas_mysqlpfs_mstmt_set_param_longlong( mstmt_s, 1, 12 (* parent_id *) , FALSE ); */
          mas_mysqlpfs_mstmt_set_param_string( mstmt_s, 0, "" );
          mas_mysqlpfs_mstmt_set_param_longlong( mstmt_s, 1, 0 /* parent_id */ , TRUE );
          if ( !r )
            r = mas_mysqlpfs_mstmt_execute_store_fetch( mstmt_s );
          if ( !r )
          {
            mysqlpfs_s_ulonglong_t num = 0;
            mysqlpfs_s_bool_t is_null = 0;

            r = mas_mysqlpfs_mstmt_get_result_longlong( mstmt_s, 0, &num, &is_null );
            fprintf( stderr, "RESULT: num:%lld -- is_null:%d\n", num, is_null );
          }
          else if ( r == MYSQL_NO_DATA )
          {
            fprintf( stderr, "no more data\n" );
          }
        }
#if 0
        if ( !r )
          masxfs_pathinfo_each_depth_cb( pi, test6scb, mstmt_s );
#endif
        mas_mysqlpfs_mstmt_free_result( mstmt_s );
        mas_mysqlpfs_mstmt_delete( mstmt_s );
      }

      EXAMS( path, path0, "%s : %s" );
      fprintf( stderr, "restored path:%s\n", path );
      mas_free( path );
      masxfs_pathinfo_delete( pi );
    }
    mysqlpfs_delete( pfs );
  }

  return 0;
}
