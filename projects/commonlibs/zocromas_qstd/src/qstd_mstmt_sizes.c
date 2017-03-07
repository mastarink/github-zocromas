/* #define RGEMSG mysql_error(mas_qstd_mysql(qstd)) */
#define RGEMSG mas_qstd_mysql_error(qstd)
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

#include "qstd_mstmt_sizes.h"

unsigned long long
mas_qstd_mstmt_selget_sizes_id( mas_qstd_t * qstd, unsigned long long size )
{
  rDECL( 0 );
  unsigned long long thesize = 0;

  {
    mysqlpfs_mstmt_t *mstmt_s = mas_qstd_mstmt_get( qstd, STD_MSTMT_SELECT_SIZES_ID );

    QRGP( mstmt_s );

    rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt_s, 0, size, FALSE ) );
    QRGS( rCODE );
    rC( mas_mysqlpfs_mstmt_execute_store( mstmt_s ) );
    QRGS( rCODE );
    rC( mas_mysqlpfs_mstmt_fetch( mstmt_s ) );
  /* QRGS( rCODE ); */

    /* if ( rCODE != MYSQL_NO_DATA ) */
    if ( !mas_mysqlpfs_mstmt_is_no_data( rCODE ) )
    {
      unsigned is_null = 0;

      rC( mas_mysqlpfs_mstmt_get_result_longlong( mstmt_s, 0, &thesize, &is_null ) );
      QRGS( rCODE );
    }

    mas_mysqlpfs_mstmt_free_result( mstmt_s );
  }
  return thesize;
}

unsigned long long
mas_qstd_mstmt_insget_sizes_id( mas_qstd_t * qstd, unsigned long long size )
{
  rDECL( 0 );
  QRGP( qstd );
  unsigned long long thesize = 0;

  {
    mysqlpfs_mstmt_t *mstmt = mas_qstd_mstmt_get( qstd, STD_MSTMT_INSERT_SIZES );

    QRGP( mstmt );
    rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt, 0, size, FALSE ) );
    QRGS( rCODE );
    rC( mas_mysqlpfs_mstmt_execute( mstmt ) );
    QRGS( rCODE );
    if ( !rCODE && mas_mysqlpfs_mstmt_affected_rows( mstmt ) == 1 )
      thesize = mas_mysqlpfs_mstmt_insert_id( mstmt );
  }
/* QRG( thesize >= 0 ? 0 : -1 ); */
  return thesize;
}

unsigned long long
mas_qstd_mstmt_selinsget_sizes_id( mas_qstd_t * qstd, unsigned long long size )
{
/* rDECL( 0 ); */
  QRGP( qstd );

  unsigned long long thesize = mas_qstd_mstmt_selget_sizes_id( qstd, size );

  if ( !thesize )
    thesize = mas_qstd_mstmt_insget_sizes_id( qstd, size );

/* QRG( thesize >= 0 ? 0 : -1 ); */
  return thesize;
}

unsigned long long
mas_qstd_mstmt_insselget_sizes_id( mas_qstd_t * qstd, unsigned long long size )
{
/* rDECL( 0 ); */
  QRGP( qstd );
  unsigned long long thesize = mas_qstd_mstmt_insget_sizes_id( qstd, size );

  if ( !thesize )
    thesize = mas_qstd_mstmt_selget_sizes_id( qstd, size );

/* QRG( thesize >= 0 ? 0 : -1 ); */
  return thesize;
}
