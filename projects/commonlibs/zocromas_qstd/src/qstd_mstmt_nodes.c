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
#include "qstd_mstmt.h"

#include "qstd_mstmt_nodes.h"

unsigned long long
mas_qstd_mstmt_selget_node_id( mas_qstd_t * qstd, unsigned long long parent_id, const char *name )
{
  rDECLBAD;
  unsigned long long theid = 0;

  {
    int np = 0;
    int nr = 0;
    int has_data = 0;
    mysqlpfs_mstmt_t *mstmt_s = mas_qstd_mstmt_get( qstd, STD_MSTMT_SELECT_NODES_ID );

    QRGP( mstmt_s );
    rC( mas_qstd_ret_code( qstd ) );
    /* rC( mas_qstd_mstmt_ret_code( mstmt_s ) ); */
    QRG( rCODE );
    rC( mas_qstd_mstmt_set_param_longlong( mstmt_s, np++, parent_id, FALSE ) );
    QRG( rCODE );
    rC( mas_qstd_mstmt_set_param_string( mstmt_s, np++, name ) );
    QRG( rCODE );
    rC( mas_qstd_mstmt_execute_store( mstmt_s ) );
    QRG( rCODE );


    rC( mas_qstd_mstmt_fetch( mstmt_s, &has_data ) );
    QRG( rCODE );

    if ( rGOOD && has_data )
    {
      unsigned is_null = 0;

      rC( mas_qstd_mstmt_get_result_longlong( mstmt_s, nr++, &theid, &is_null ) );
    /* WARN( "(%d) DATA for %lld, '%s' => %lld", rCODE, parent_id, name, theid ); */
      if ( rGOOD )
        assert( nr == STD_MSTMT_SELECT_NODES_NRESULTS );
    }
#if 0
    else
    {
      WARN( "NO DATA for parent_id: %lld, name: '%s' - %llu", parent_id, name, theid );
    }
#endif
    mas_qstd_mstmt_free_result( mstmt_s );
    QRG( rCODE );
  }
  qstd->ret_code = rCODE;
  return theid;
}
