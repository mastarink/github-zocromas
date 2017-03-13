/* #define RGEMSG mysql_error(mas_qstd_mysql(qstd)) */
#define RGEMSG mas_mysqlpfs_mstmt_error(mstmt)
#include "qstd_defs.h"
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>

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
    "UPDATE parents AS p "                                           /* */
            " LEFT JOIN "                                            /* */
            "   (SELECT filenames.parent_id, COUNT(*) AS nchilds "   /* */
            "     FROM filenames "                                   /* */
            "     LEFT JOIN parents ON (filenames.parent_id=parents.id) " /* */
            "     GROUP BY filenames.parent_id) "                    /* */
            "    AS fx ON (p.id=fx.parent_id) SET p.nchilds=fx.nchilds",
    "UPDATE filedatas AS fd "                                        /* */
            " LEFT JOIN "                                            /* */
            "   (SELECT ifd.id AS data_id, COUNT(*) AS nlinkdb "     /* */
            "     FROM filedatas AS ifd "                            /* */
            "     LEFT JOIN filenames AS ifn ON (ifn.data_id=ifd.id) " /* */
            "     GROUP BY ifn.data_id) "                            /* */
            "    AS fx ON (fd.id=fx.data_id) SET fd.nlinkdb=fx.nlinkdb",
    "UPDATE filesizes AS fs "                                        /* */
            " LEFT JOIN "                                            /* */
            "   (SELECT ifs.size AS size, COUNT(*) AS nsame "        /* */
            "     FROM filesizes AS ifs "                            /* */
            "     LEFT JOIN fileprops AS ifp ON (ifs.size=ifp.size) " /* */
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
  QRGS( rCODE );

  rRET;
}

int
mas_qstd_mstmt_prepare_param_string( mysqlpfs_mstmt_t * mstmt, int pos, size_t buffer_length )
{
  rDECL( 0 );
  rC( mas_mysqlpfs_mstmt_prepare_param_string( mstmt, pos, ( mysqlpfs_s_length_t ) buffer_length ) );
  QRGS( rCODE );

  rRET;
}

int
mas_qstd_mstmt_set_param_longlong( mysqlpfs_mstmt_t * mstmt, int pos, unsigned long long num, unsigned is_null )
{
  rDECL( 0 );
  rC( mas_mysqlpfs_mstmt_set_param_longlong( mstmt, pos, num, is_null ) );
  QRGS( rCODE );

  rRET;
}

int
mas_qstd_mstmt_data_seek( mysqlpfs_mstmt_t * mstmt, unsigned long long offset )
{
  rDECL( 0 );
  rC( mas_mysqlpfs_mstmt_data_seek( mstmt, offset ) );
  QRGS( rCODE );

  rRET;
}
