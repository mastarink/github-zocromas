#define R_GOOD(_r) (_r>=0)
/* #define RGEMSG mysql_error(mas_qstd_mysql(qstd)) */
#define RGEMSG mas_mysqlpfs_mstmt_error(mstmt)
#include "qstd_defs.h"
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>
#include <mastar/mysqlpfs/mysqlpfs_defs.h>

#include <mastar/mysqlpfs/mysqlpfs_base.h>
#include <mastar/mysqlpfs/mysqlpfs.h>
#include <mastar/mysqlpfs/mysqlpfs_query.h>
#include <mastar/mysqlpfs/mysqlpfs_mstmt_base.h>
#include <mastar/mysqlpfs/mysqlpfs_mstmt.h>

#include "qstd_structs.h"

#include "qstd_mstmt.h"

mysqlpfs_s_mysql_t *
mas_qstd_mysql( mas_qstd_t * qstd )
{
  return qstd ? mas_mysqlpfs_mysql( qstd->pfs ) : NULL;
}

const char *
mas_qstd_mysql_error( mas_qstd_t * qstd )
{
  return qstd ? mas_mysqlpfs_mysql_error( qstd->pfs ) : NULL;
}

int
mas_qstd_query( mas_qstd_t * qstd, const char *op )
{
  return qstd ? mas_mysqlpfs_query( qstd->pfs, op ) : -1;
}

int
mas_qstd_update_summary( mas_qstd_t * qstd )
{
  rDECL( 0 );
  const char *updop[] = {
    "START TRANSACTION",
    "UPDATE " QSTD_TABLE_PARENTS " AS p "                            /* */
            " LEFT JOIN "                                            /* */
            "   (SELECT " QSTD_TABLE_NAMES ".parent_id, COUNT(*) AS nchilds " /* */
            "     FROM " QSTD_TABLE_NAMES " "                        /* */
            "     LEFT JOIN " QSTD_TABLE_PARENTS " ON (" QSTD_TABLE_NAMES ".parent_id=" QSTD_TABLE_PARENTS ".id) " /* */
            "     GROUP BY " QSTD_TABLE_NAMES ".parent_id) "         /* */
            "    AS fx ON (p.id=fx.parent_id) SET p.nchilds=fx.nchilds",
    "UPDATE " QSTD_TABLE_DATAS " AS fd "                             /* */
            " LEFT JOIN "                                            /* */
            "   (SELECT ifd.id AS data_id, COUNT(*) AS nlinkdb "     /* */
            "     FROM " QSTD_TABLE_DATAS " AS ifd "                 /* */
            "     LEFT JOIN " QSTD_TABLE_NAMES " AS ifn ON (ifn.data_id=ifd.id) " /* */
            "     GROUP BY ifn.data_id) "                            /* */
            "    AS fx ON (fd.id=fx.data_id) SET fd.nlinkdb=fx.nlinkdb",
    "UPDATE " QSTD_TABLE_SIZES " AS fs "                             /* */
            " LEFT JOIN "                                            /* */
            "   (SELECT ifs.size AS size, COUNT(*) AS nsame "        /* */
            "     FROM " QSTD_TABLE_SIZES " AS ifs "                 /* */
            "     LEFT JOIN "QSTD_TABLE_PROPS" AS ifp ON (ifs.size=ifp.size) " /* */
            "     GROUP BY ifp.size) "                               /* */
            "    AS fx ON (fs.size=fx.size) SET fs.nsame=fx.nsame",
    "COMMIT"
  };
  for ( size_t i = 0; i < sizeof( updop ) / sizeof( updop[0] ); i++ )
    rC( mas_qstd_query( qstd, updop[i] ) );

  rRET;
}

int
mas_qstd_start_transaction( mas_qstd_t * qstd )
{
  return mas_qstd_query( qstd, "START TRANSACTION" );
}

int
mas_qstd_end_transaction( mas_qstd_t * qstd )
{
  return mas_qstd_query( qstd, "COMMIT" );
}

int
mas_qstd_mstmt_prepare_param_longlong( mysqlpfs_mstmt_t * mstmt, int pos )
{
  rDECL( 0 );
  rC( mas_mysqlpfs_mstmt_prepare_param_longlong( mstmt, pos ) );

  rRET;
}

int
mas_qstd_mstmt_prepare_result_longlong( mysqlpfs_mstmt_t * mstmt, int pos )
{
  rDECL( 0 );
  rC( mas_mysqlpfs_mstmt_prepare_result_longlong( mstmt, pos ) );

  rRET;
}

int
mas_qstd_mstmt_prepare_param_string( mysqlpfs_mstmt_t * mstmt, int pos )
{
  rDECL( 0 );
  rC( mas_mysqlpfs_mstmt_prepare_param_string( mstmt, pos, ( mysqlpfs_s_length_t ) 255 ) );

  rRET;
}

int
mas_qstd_mstmt_prepare_result_string( mysqlpfs_mstmt_t * mstmt, int pos )
{
  return mas_mysqlpfs_mstmt_prepare_result_string( mstmt, pos, ( mysqlpfs_s_length_t ) 255 );
}

int
mas_qstd_mstmt_set_param_longlong( mysqlpfs_mstmt_t * mstmt, int pos, unsigned long long num, unsigned is_null )
{
  rDECL( 0 );
  rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt, pos, num, is_null ) );

  rRET;
}

int
mas_qstd_mstmt_data_seek( mysqlpfs_mstmt_t * mstmt, unsigned long long offset )
{
  rDECL( 0 );
  rC( mas_mysqlpfs_mstmt_data_seek( mstmt, offset ) );

  rRET;
}

int
mas_qstd_mstmt_execute_store( mysqlpfs_mstmt_t * mstmt )
{
  rDECL( 0 );
  rC( mas_mysqlpfs_mstmt_execute_store( mstmt ) );
  rRET;
}

int
mas_qstd_mstmt_fetch( mysqlpfs_mstmt_t * mstmt, int *phas_data )
{
  rDECL( 0 );
  rC( mas_mysqlpfs_mstmt_fetch( mstmt, phas_data ) );
  rRET;
}

int
mas_qstd_mstmt_get_result_longlong( mysqlpfs_mstmt_t * mstmt, int pos, unsigned long long *pnum, unsigned *pis_null )
{
  return mas_mysqlpfs_mstmt_get_result_longlong( mstmt, pos, pnum, pis_null );
}

int
mas_qstd_mstmt_get_result_string_na( mysqlpfs_mstmt_t * mstmt, int pos, const char **pstring )
{
  return mas_mysqlpfs_mstmt_get_result_string_na( mstmt, pos, pstring );
}

int
mas_qstd_mstmt_bind_param( mysqlpfs_mstmt_t * mstmt )
{
  return mas_mysqlpfs_mstmt_bind_param( mstmt );
}

int
mas_qstd_mstmt_bind_result( mysqlpfs_mstmt_t * mstmt )
{
  return mas_mysqlpfs_mstmt_bind_result( mstmt );
}
