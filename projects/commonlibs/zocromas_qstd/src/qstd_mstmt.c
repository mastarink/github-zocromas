/* #define RGEMSG mysql_error(mas_qstd_mysql(qstd)) */
#define RGEMSG mas_qstd_mysql_error(qstd)
#include "qstd_defs.h"
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>

#include <mastar/mysqlpfs/mysqlpfs_base.h>
#include <mastar/mysqlpfs/mysqlpfs.h>
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
