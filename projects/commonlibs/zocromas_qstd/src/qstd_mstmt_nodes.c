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

#include "qstd_mstmt_nodes.h"

unsigned long long
mas_qstd_mstmt_selget_node_id( mas_qstd_t * qstd, unsigned long long parent_id, const char *name )
{
  rDECL( 0 );
  unsigned long long theid = 0;

  {
    mysqlpfs_mstmt_t *mstmt_s = mas_qstd_mstmt_get( qstd, STD_MSTMT_SELECT_NODES_ID );

    QRGP( mstmt_s );

    rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt_s, 0, parent_id, FALSE ) );
    QRGS( rCODE );
    rC( mas_mysqlpfs_mstmt_set_param_string( mstmt_s, 1, name ) );
    QRGS( rCODE );
    rC( mas_mysqlpfs_mstmt_execute_store( mstmt_s ) );
    QRGS( rCODE );

    rC( mas_mysqlpfs_mstmt_fetch( mstmt_s ) );
  /* QRGS( rCODE ); */

  /* if ( rCODE != MYSQL_NO_DATA ) */
    if ( !mas_mysqlpfs_mstmt_is_no_data( rCODE ) )
    {
      unsigned is_null = 0;

      rC( mas_mysqlpfs_mstmt_get_result_longlong( mstmt_s, 0, &theid, &is_null ) );
      QRGS( rCODE );
      WARN( "DATA for %lld, '%s' => %lld", parent_id, name, theid );
    }
    else
    {
      DIE( "NO DATA for %lld, '%s'", parent_id, name );
    }

    mas_mysqlpfs_mstmt_free_result( mstmt_s );
  }
  return theid;
}
