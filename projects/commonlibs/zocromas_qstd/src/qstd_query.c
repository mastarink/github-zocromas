#define R_GOOD(_r) ((_r)>=0)
#define RGEMSG mysql_error(mas_qstd_mysql(qstd))
/* #define RGEMSG mas_mysqlpfs_mstmt_error(mstmt) */
#include "qstd_defs.h"
#include <string.h>

/* #include <mastar/wrap/mas_memory.h> */
#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>
#include <mastar/mysqlpfs/mysqlpfs_defs.h>

/* #include <mastar/mysqlpfs/mysqlpfs_base.h> */
#include <mastar/mysqlpfs/mysqlpfs.h>
#include <mastar/mysqlpfs/mysqlpfs_query.h>
/* #include <mastar/mysqlpfs/mysqlpfs_mstmt_base.h> */
#include <mastar/mysqlpfs/mysqlpfs_mstmt.h>

#include "qstd_structs.h"

#include "qstd_query.h"

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
  rDECLBAD;
  if ( qstd )
  {
    if ( !mas_mysqlpfs_query( qstd->pfs, op ) )
      rSETGOOD;
  }
  else
    QRGM( rCODE, "no qstd" );
  rRET;
}

int
mas_qstd_update_summary( mas_qstd_t * qstd )
{
  rDECLGOOD;
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
            "     LEFT JOIN " QSTD_TABLE_PROPS " AS ifp ON (ifs.size=ifp.size) " /* */
            "     GROUP BY ifp.size) "                               /* */
            "    AS fx ON (fs.size=fx.size) SET fs.nsame=fx.nsame",
    "UPDATE " QSTD_TABLE_SHA1 " AS sha "                             /* */
            " LEFT JOIN "                                            /* */
            "   (SELECT ishd.sum_id, COUNT(*) AS nsame "             /* */
            "     FROM " QSTD_TABLE_SHA1DREF " AS ishd "             /* */
            "     LEFT JOIN " QSTD_TABLE_SHA1 " AS isha ON (isha.id=ishd.sum_id) " /* */
            "     GROUP BY ishd.sum_id ) "                           /* */
            "    AS fx ON (sha.id=fx.sum_id) SET sha.nsame=fx.nsame",
    "COMMIT"
  };
  for ( size_t i = 0; i < sizeof( updop ) / sizeof( updop[0] ) && rGOOD; i++ )
  {
    rC( mas_qstd_query( qstd, updop[i] ) );
    if ( rBAD )
    {
      mas_qstd_query( qstd, "ROLLBACK" );
      break;
    }
  }
  rRET;
}

int
mas_qstd_start_transaction( mas_qstd_t * qstd )
{
  rDECLBAD;
  rC( mas_qstd_query( qstd, "START TRANSACTION" ));
  rC( mas_qstd_query( qstd, "SET autocommit = 1" ));
  rRET;
}

int
mas_qstd_end_transaction( mas_qstd_t * qstd )
{
  return mas_qstd_query( qstd, "COMMIT" );
}
