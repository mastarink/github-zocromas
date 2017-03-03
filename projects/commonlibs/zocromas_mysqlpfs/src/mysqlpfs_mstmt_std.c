#define RGEMSG mysql_error(&pfs->mysql)
#include "mysqlpfs_defs.h"
#include <string.h>

#include <mysql.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>

#include "mysqlpfs_structs.h"

#include "mysqlpfs_mstmt_base.h"
#include "mysqlpfs_mstmt.h"

#include "mysqlpfs_base.h"

#include "mysqlpfs_mstmt_std.h"

mysqlpfs_mstmt_t **
mysqlpfs_mstmt_std_create_array( void )
{
  return mas_calloc( STD_MSTMT_MAX, sizeof( mysqlpfs_mstmt_t ** ) );
}

void
mysqlpfs_mstmt_std_reset_array( mysqlpfs_mstmt_t ** mstmts )
{
  if ( mstmts )
    for ( int nst = 0; nst < STD_MSTMT_MAX; nst++ )
    {
      if ( mstmts[nst] )
        mas_mysqlpfs_mstmt_delete( mstmts[nst] );
      mstmts[nst] = NULL;
    }
}

void
mysqlpfs_mstmt_std_delete_array( mysqlpfs_mstmt_t ** mstmts )
{
  mysqlpfs_mstmt_std_reset_array( mstmts );
  mas_free( mstmts );
}

mysqlpfs_mstmt_t *
mysqlpfs_mstmt_std_init( mysqlpfs_t * pfs, mysqlpfs_std_id_t stdid )
{
  rSET( 0 );
  mysqlpfs_mstmt_t *mstmt = NULL;

  if ( pfs && stdid >= 0 && stdid < STD_MSTMT_MAX )
  {
    mysqlpfs_mstmt_t **mstmts = pfs->std_mstmts;

    mstmt = mstmts[stdid];
    if ( mstmt )
    {
      mas_mysqlpfs_mstmt_delete( mstmt );
      mstmt = mstmts[stdid] = NULL;
    }
    switch ( stdid )
    {
    case STD_MSTMT_INSERT_NAMES:
      {
        char *insop = "INSERT INTO filenames(name,parent_id) VALUES (?,?)";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 2, 0, insop );

        rC( mas_mysqlpfs_mstmt_prepare_param_string( mstmt, 0, 255 ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 1 ) );
        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
      }
      break;
    case STD_MSTMT_SELECT_NAMES_ID:
      {
        char *selop = "SELECT id FROM filenames WHERE name=? AND parent_id<=>?";

        mstmt = mas_mysqlpfs_mstmt_create_setup( pfs, 2, 1, selop );

        rC( mas_mysqlpfs_mstmt_prepare_param_string( mstmt, 0, 255 ) );
        rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, 1 ) );
        rC( mas_mysqlpfs_mstmt_bind_param( mstmt ) );
        rC( mas_mysqlpfs_mstmt_prepare_result_longlong( mstmt, 0 ) );
        rC( mas_mysqlpfs_mstmt_bind_result( mstmt ) );
      }
    case STD_MSTMT_MAX:
      break;
    }
    mstmts[stdid] = mstmt;
  }
  return mstmt;
}

mysqlpfs_mstmt_t *
mysqlpfs_mstmt_std_get( mysqlpfs_t * pfs, mysqlpfs_std_id_t stdid )
{
  mysqlpfs_mstmt_t *mstmt = NULL;

  if ( pfs && stdid >= 0 && stdid < STD_MSTMT_MAX )
  {
    mysqlpfs_mstmt_t **mstmts = pfs->std_mstmts;

    mstmt = mstmts[stdid];
    if ( !mstmt )
    {
      mstmt = mysqlpfs_mstmt_std_init( pfs, stdid );
      QRGP( mstmt );
    }
    QRGP( mstmt );
  }
  else
  {
    WARN( "pfs:%p; stdid:%d; STD_MSTMT_MAX:%d;", pfs, stdid, STD_MSTMT_MAX );
    QRG( -1 );
  }

  return mstmt;
}

mysqlpfs_s_ulonglong_t
mysqlpfs_mstmt_std_get_names_id( mysqlpfs_t * pfs, const char *name, mysqlpfs_s_ulonglong_t parent_id )
{
  rSET( 0 );
  mysqlpfs_s_ulonglong_t num = 0;
  mysqlpfs_mstmt_t *mstmt_s = mysqlpfs_mstmt_std_get( pfs, STD_MSTMT_SELECT_NAMES_ID );

  {
    QRGP( mstmt_s );

    rC( mas_mysqlpfs_mstmt_set_param_string( mstmt_s, 0, name ) );
    rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt_s, 1, parent_id, parent_id ? FALSE : TRUE ) );
    rC( mas_mysqlpfs_mstmt_execute_store( mstmt_s ) );

    INFO( "ROWS: %lld", mas_mysqlpfs_mstmt_num_rows( mstmt_s ) );
    rC( mas_mysqlpfs_mstmt_fetch( mstmt_s ) );

    if ( r == MYSQL_NO_DATA )
      WARN( "no more data" );
    else
    {
      mysqlpfs_s_bool_t is_null = 0;

      rC( mas_mysqlpfs_mstmt_get_result_longlong( mstmt_s, 0, &num, &is_null ) );

    /* if ( !r )                                                 */
    /*   WARN( "RESULT: num:%lld -- is_null:%d", num, is_null ); */
    }
  }

  mas_mysqlpfs_mstmt_free_result( mstmt_s );
/* mas_mysqlpfs_mstmt_delete( mstmt_s ); => centralised deletion of std mstmt's */
  return num;
}
