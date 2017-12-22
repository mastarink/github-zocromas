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

#include "qstd_mstmt_names.h"

unsigned long long
mas_qstd_mstmt_selget_names_id( mas_qstd_t * qstd, const char *name, unsigned long long updir_id )
{
  rDECLBAD;
  unsigned long long num = 0;

  QRGP( name );
  {
    int np = 0;
    int nr = 0;
    mysqlpfs_mstmt_t *mstmt_s = mas_qstd_mstmt_get( qstd, STD_MSTMT_SELECT_NAMES_ID );
    int has_data = 0;

    QRGP( mstmt_s );

    rC( mas_mysqlpfs_mstmt_set_param_string( mstmt_s, np++, name ) );
    rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt_s, np++, updir_id, updir_id ? FALSE : TRUE ) );
    assert( np == STD_MSTMT_SELECT_NAMES_NFIELDS );

    rC( mas_mysqlpfs_mstmt_execute_store( mstmt_s ) );
    rC( mas_mysqlpfs_mstmt_fetch( mstmt_s, &has_data ) );

  /* if ( rCODE != MYSQL_NO_DATA ) */
    if ( rGOOD && has_data )
    {
      unsigned is_null = 0;

      rC( mas_mysqlpfs_mstmt_get_result_longlong( mstmt_s, nr++, &num, &is_null ) );
      assert( nr == STD_MSTMT_SELECT_NAMES_NRESULTS );
    }

    mas_mysqlpfs_mstmt_free_result( mstmt_s );
  }
  return num;
}

unsigned long long
mas_qstd_mstmt_insget_names_id( mas_qstd_t * qstd, const char *name, unsigned long long updir_id, unsigned long long data_id,
                                const char *sdetype _uUu_ )
{
  rDECLBAD;
  QRGP( qstd );
  QRGP( name );
  unsigned long long theid = 0;

  {
    int np = 0;
    mysqlpfs_mstmt_t *mstmt = mas_qstd_mstmt_get( qstd, STD_MSTMT_INSERT_NAMES );

    assert( updir_id );
    QRGP( mstmt );
    rC( mas_mysqlpfs_mstmt_set_param_string( mstmt, np++, name ) );
    rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt, np++, updir_id, updir_id ? FALSE : TRUE ) );
    rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt, np++, data_id, data_id ? FALSE : TRUE ) );
  /* rC( mas_mysqlpfs_mstmt_set_param_string( mstmt, np++, sdetype ) ); */
    assert( np == STD_MSTMT_INSERT_NAMES_NFIELDS );
    rC( mas_mysqlpfs_mstmt_execute( mstmt ) );
    if ( rCODE )
      WARN( "ERR: %s; [%lld] '%s'", mas_mysqlpfs_mstmt_error( mstmt ), updir_id, name );

    if ( !rCODE && mas_mysqlpfs_mstmt_affected_rows( mstmt ) == 1 )
      theid = mas_mysqlpfs_mstmt_insert_id( mstmt );
  }
  QRG( theid > 0 ? 0 : -1 );
  return theid;
}

unsigned long long
mas_qstd_mstmt_selinsget_names_id( mas_qstd_t * qstd, const char *name, unsigned long long updir_id, unsigned long long data_id, const char *sdetype )
{
/* rDECLBAD; */
  QRGP( qstd );
  QRGP( name );

  unsigned long long theid = mas_qstd_mstmt_selget_names_id( qstd, name, updir_id );

  if ( !theid )
  {
    assert( updir_id );
    theid = mas_qstd_mstmt_insget_names_id( qstd, name, updir_id, data_id, sdetype );
  }
  QRG( theid > 0 ? 0 : -1 );
  return theid;
}

unsigned long long
mas_qstd_mstmt_insselget_names_id( mas_qstd_t * qstd, const char *name, unsigned long long updir_id, unsigned long long data_id, const char *sdetype )
{
/* rDECLBAD; */
  QRGP( qstd );
  QRGP( name );
  unsigned long long theid = mas_qstd_mstmt_insget_names_id( qstd, name, updir_id, data_id, sdetype );

  if ( !theid )
    theid = mas_qstd_mstmt_selget_names_id( qstd, name, updir_id );

  QRG( theid > 0 ? 0 : -1 );
  return theid;
}
