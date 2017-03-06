#define RGEMSG mysql_error(mas_qstd_mysql(qstd))
#include "qstd_defs.h"
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>

#include <mastar/mysqlpfs/mysqlpfs_base.h>
#include <mastar/mysqlpfs/mysqlpfs_mstmt_base.h>
#include <mastar/mysqlpfs/mysqlpfs_mstmt.h>

#include "qstd_structs.h"
#include "qstd_mstmt.h"
#include "qstd_mstmt_base.h"

#include "qstd_mstmt_parents.h"


mysqlpfs_s_ulonglong_t
mas_qstd_mstmt_selget_parents_id( mas_qstd_t * qstd, mysqlpfs_s_ulonglong_t dir_id )
{
  rDECL( 0 );
  mysqlpfs_s_ulonglong_t theid = 0;

  {
    mysqlpfs_mstmt_t *mstmt_s = mas_qstd_mstmt_get( qstd, STD_MSTMT_SELECT_PARENTS_ID );

    QRGP( mstmt_s );

  rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt_s, 0, dir_id, dir_id ? FALSE : TRUE ) );
    QRGS( rCODE );
    rC( mas_mysqlpfs_mstmt_execute_store( mstmt_s ) );
    QRGS( rCODE );

    rC( mas_mysqlpfs_mstmt_fetch( mstmt_s ) );
    /* QRGS( rCODE ); */

    if ( rCODE != MYSQL_NO_DATA )
    {
      mysqlpfs_s_bool_t is_null = 0;

      rC( mas_mysqlpfs_mstmt_get_result_longlong( mstmt_s, 0, &theid, &is_null ) );
      QRGS( rCODE );
    }

    mas_mysqlpfs_mstmt_free_result( mstmt_s );
  }
  return theid;
}

mysqlpfs_s_ulonglong_t
mas_qstd_mstmt_insget_parents_id( mas_qstd_t * qstd, mysqlpfs_s_ulonglong_t dir_id )
{
  rDECL( 0 );
  QRGP( qstd );
  mysqlpfs_s_ulonglong_t theid = 0;

  {
    mysqlpfs_mstmt_t *mstmt = mas_qstd_mstmt_get( qstd, STD_MSTMT_INSERT_PARENTS );

    QRGP( mstmt );
    rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt, 0, dir_id, dir_id ? FALSE : TRUE ) );
    QRGS( rCODE );

    rC( mas_mysqlpfs_mstmt_execute( mstmt ) );
    QRGS( rCODE );
    if ( !rCODE && mas_mysqlpfs_mstmt_affected_rows( mstmt ) == 1 )
      theid = mas_mysqlpfs_mstmt_insert_id( mstmt );
  }
  QRG( theid > 0 ? 0 : -1 );
  return theid;
}

mysqlpfs_s_ulonglong_t
mas_qstd_mstmt_selinsget_parents_id( mas_qstd_t * qstd, mysqlpfs_s_ulonglong_t dir_id )
{
/* rDECL( 0 ); */
  QRGP( qstd );

  mysqlpfs_s_ulonglong_t theid = mas_qstd_mstmt_selget_parents_id( qstd, dir_id );

/* WARN( "SELINS %lld", theid ); */
  if ( !theid )
    theid = mas_qstd_mstmt_insget_parents_id( qstd, dir_id );

  QRG( theid > 0 ? 0 : -1 );
  return theid;
}

mysqlpfs_s_ulonglong_t
mas_qstd_mstmt_insselget_parents_id( mas_qstd_t * qstd, mysqlpfs_s_ulonglong_t dir_id )
{
/* rDECL( 0 ); */
  QRGP( qstd );
  mysqlpfs_s_ulonglong_t theid = mas_qstd_mstmt_insget_parents_id( qstd, dir_id );

  if ( !theid )
    theid = mas_qstd_mstmt_selget_parents_id( qstd, dir_id );

  QRG( theid > 0 ? 0 : -1 );
  return theid;
}
