#include <mastar/sqlite/mas_sqlite_const.h>

#include "duf_maintenance.h"


/* ###################################################################### */
#include "duf_sql_error.h"
/* ###################################################################### */

int
duf_sqlite2r_error_code( int r3 )
{
  int rt;

  rt = ( r3 == SQLITE_OK ) ? 0 : ( r3 > 0 ? MAS_SQLITE_ERROR_BASE + r3 : r3 );
  return rt;
}

int
duf_r2sqlite_error_code( int rt )
{
  int r3;

  r3 = ( rt == 0 ) ? SQLITE_OK : ( rt < 0 ? rt - MAS_SQLITE_ERROR_BASE : rt );
  return r3;
}


