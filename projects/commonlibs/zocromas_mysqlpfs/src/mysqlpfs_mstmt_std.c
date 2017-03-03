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
