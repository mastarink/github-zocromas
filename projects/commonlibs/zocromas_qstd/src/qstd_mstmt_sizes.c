#define R_GOOD(_r) ((_r)>=0)
/* #define RGEMSG mysql_error(mas_qstd_mysql(qstd)) */
#define RGEMSG mas_qstd_mysql_error(qstd)
#include "qstd_defs.h"
/* #include <string.h> */

/* #include <mastar/wrap/mas_memory.h> */
#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>

/* #include <mastar/mysqlpfs/mysqlpfs_base.h> */
/* #include <mastar/mysqlpfs/mysqlpfs_mstmt_base.h> */
#include <mastar/mysqlpfs/mysqlpfs_mstmt.h>
#include <mastar/mysqlpfs/mysqlpfs_defs.h>

#include "qstd_structs.h"
#include "qstd_enums.h"
/* #include "qstd_mstmt.h" */
#include "qstd_mstmt_base.h"
#include "qstd_mstmt.h"

#include "qstd_mstmt_sizes.h"

unsigned long long
mas_qstd_mstmt_selget_sizes_id( mas_qstd_t * qstd, unsigned long long size )
{
  rDECLBAD;
  unsigned long long thesize = 0;

  {
    int np = 0;
    int nr = 0;
    int has_data = 0;
    mysqlpfs_mstmt_t *mstmt_s = mas_qstd_mstmt_get( qstd, STD_MSTMT_SELECT_SIZES_ID );

    QRGP( mstmt_s );
    rC( mas_qstd_ret_code( qstd ) );
    /* rC( mas_qstd_mstmt_ret_code( mstmt_s ) ); */

    rC( mas_qstd_mstmt_set_param_longlong( mstmt_s, np++, size, FALSE ) );
    rC( mas_qstd_mstmt_execute_store( mstmt_s ) );
    rC( mas_qstd_mstmt_fetch( mstmt_s, &has_data ) );

    if ( has_data )
    {
      unsigned is_null = 0;

      rC( mas_qstd_mstmt_get_result_longlong( mstmt_s, nr++, &thesize, &is_null ) );
      assert( nr == STD_MSTMT_SELECT_SIZES_NRESULTS );
    }

    mas_qstd_mstmt_free_result( mstmt_s );
  }
  qstd->ret_code = rCODE;
  return thesize;
}

unsigned long long
mas_qstd_mstmt_insget_sizes_id( mas_qstd_t * qstd, unsigned long long size )
{
  rDECLGOOD;
  QRGP( qstd );
  unsigned long long thesize = 0;

  {
    int np = 0;
    mysqlpfs_mstmt_t *mstmt = mas_qstd_mstmt_get( qstd, STD_MSTMT_INSERT_SIZES );
    int ar = 0;

    QRGP( mstmt );
    rC( mas_qstd_ret_code( qstd ) );
    /* rC( mas_qstd_mstmt_ret_code( mstmt ) ); */
    rC( mas_qstd_mstmt_set_param_longlong( mstmt, np++, size, FALSE ) );
    rC( mas_qstd_mstmt_execute( mstmt ) );
    ar = mas_qstd_mstmt_affected_rows( mstmt );
    if ( rGOOD && ar == 1 )
    {
    /* 20180124.124534: NOT AUTO_INCREMENT!! :: thesize = mas_qstd_mstmt_insert_id( mstmt ); */
      thesize = size;
    }
    else
    {
    }
  }
/* QRG( thesize >= 0 ? 0 : -1 ); */
  qstd->ret_code = rCODE;
  return thesize;
}

unsigned long long
mas_qstd_mstmt_selinsget_sizes_id( mas_qstd_t * qstd, unsigned long long size )
{
  rDECLGOOD;
  QRGP( qstd );

  unsigned long long thesize = mas_qstd_mstmt_selget_sizes_id( qstd, size );

  rC( mas_qstd_ret_code( qstd ) );

  if ( rGOOD && !thesize )
  {
    thesize = mas_qstd_mstmt_insget_sizes_id( qstd, size );
    rC( mas_qstd_ret_code( qstd ) );
  }
/* QRG( thesize >= 0 ? 0 : -1 ); */
  qstd->ret_code = rCODE;
  return thesize;
}

unsigned long long
mas_qstd_mstmt_insselget_sizes_id( mas_qstd_t * qstd, unsigned long long size )
{
  rDECLGOOD;
  QRGP( qstd );
  unsigned long long thesize = mas_qstd_mstmt_insget_sizes_id( qstd, size );

  rC( mas_qstd_ret_code( qstd ) );

  if ( rGOOD && !thesize )
  {
    thesize = mas_qstd_mstmt_selget_sizes_id( qstd, size );
    rC( mas_qstd_ret_code( qstd ) );
  }
/* QRG( thesize >= 0 ? 0 : -1 ); */
  qstd->ret_code = rCODE;
  return thesize;
}
