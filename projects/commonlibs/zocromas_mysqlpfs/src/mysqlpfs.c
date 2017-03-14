#define R_GOOD(_r) (!_r)
#define RGEMSG mysql_error(mas_mysqlpfs_mysql(pfs))
/* #define RGEMSG mysql_error(&pfs->mysql) */
#include "mysqlpfs_defs.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdlib.h>

#include <mysql.h>

#include <mastar/wrap/mas_memory.h>
/* #include <mastar/minierr/minierr.h> */
#include <mastar/regerr/masregerr.h>

#include "mysqlpfs_structs.h"
#include "mysqlpfs.h"

mysqlpfs_s_mysql_t *
mas_mysqlpfs_mysql( mysqlpfs_t * pfs )
{
  return pfs ? &pfs->mysql : NULL;
}

const char *
mas_mysqlpfs_mysql_error( mysqlpfs_t * pfs )
{
  return pfs ? mysql_error( &pfs->mysql ) : NULL;
}
