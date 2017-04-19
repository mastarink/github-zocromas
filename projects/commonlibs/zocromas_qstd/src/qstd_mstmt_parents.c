#define R_GOOD(_r) ((_r)>=0)
/* #define RGEMSG mysql_error(mas_qstd_mysql(qstd)) */
#define RGEMSG mas_qstd_mysql_error(qstd)
#include "qstd_defs.h"
/* #include <string.h> */

#include <mastar/wrap/mas_memory.h>
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

#include "qstd_mstmt_parents.h"

unsigned long long
mas_qstd_mstmt_selget_parents_id( mas_qstd_t * qstd, unsigned long long dir_id )
{
  rDECLBAD;
  unsigned long long theid = 0;

  {
    int np = 0;
    int nr = 0;
    mysqlpfs_mstmt_t *mstmt_s = mas_qstd_mstmt_get( qstd, STD_MSTMT_SELECT_PARENTS_ID );
    int has_data = 0;

    QRGP( mstmt_s );

    rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt_s, np++, dir_id, dir_id ? FALSE : TRUE ) );
    assert( np == STD_MSTMT_SELECT_PARENTS_NFIELDS );
    rC( mas_mysqlpfs_mstmt_execute_store( mstmt_s ) );

    rC( mas_mysqlpfs_mstmt_fetch( mstmt_s, &has_data ) );

    if ( has_data )
    {
      unsigned is_null = 0;

      rC( mas_mysqlpfs_mstmt_get_result_longlong( mstmt_s, nr++, &theid, &is_null ) );
      assert( nr == STD_MSTMT_SELECT_PARENTS_NRESULTS );
    }

    mas_mysqlpfs_mstmt_free_result( mstmt_s );
  }
  return theid;
}

unsigned long long
mas_qstd_mstmt_insget_parents_id( mas_qstd_t * qstd, unsigned long long dir_id )
{
  rDECLBAD;
  QRGP( qstd );
  unsigned long long theid = 0;

  {
    int np = 0;
    mysqlpfs_mstmt_t *mstmt = mas_qstd_mstmt_get( qstd, STD_MSTMT_INSERT_PARENTS );

    QRGP( mstmt );
    rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt, np++, dir_id, dir_id ? FALSE : TRUE ) );
    assert( np == STD_MSTMT_INSERT_PARENTS_NFIELDS );

    rC( mas_mysqlpfs_mstmt_execute( mstmt ) );
    if ( !rCODE && mas_mysqlpfs_mstmt_affected_rows( mstmt ) == 1 )
      theid = mas_mysqlpfs_mstmt_insert_id( mstmt );
  }
  QRG( theid > 0 ? 0 : -1 );
  return theid;
}

__attribute__ ( ( visibility( "default" ) ) )
     unsigned long long mas_qstd_mstmt_selinsget_parents_id( mas_qstd_t * qstd, unsigned long long dir_id )
{
/* rDECLBAD; */
  QRGP( qstd );

  unsigned long long theid = mas_qstd_mstmt_selget_parents_id( qstd, dir_id );

/* WARN( "SELINS %lld", theid ); */
  if ( !theid )
    theid = mas_qstd_mstmt_insget_parents_id( qstd, dir_id );

  QRG( theid > 0 ? 0 : -1 );
  return theid;
}

unsigned long long
mas_qstd_mstmt_insselget_parents_id( mas_qstd_t * qstd, unsigned long long dir_id )
{
/* rDECLBAD; */
  QRGP( qstd );
  unsigned long long theid = mas_qstd_mstmt_insget_parents_id( qstd, dir_id );

  if ( !theid )
    theid = mas_qstd_mstmt_selget_parents_id( qstd, dir_id );

  QRG( theid > 0 ? 0 : -1 );
  return theid;
}
