#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"

#include "duf_service.h"

#include "duf_config_ref.h"

/* #include "duf_sqlite.h" */
#include <mastar/sqlite/mas_sqlite.h>

/* ###################################################################### */
#include "duf_sql_const.h"
#include "duf_sql_error.h"
/* ###################################################################### */

int
duf_sqlite2r_error_code( int r3 )
{
  int rt;

  rt = ( r3 == SQLITE_OK ) ? 0 : ( r3 > 0 ? DUF_SQLITE_ERROR_BASE + r3 : r3 );
  return rt;
}

int
duf_r2sqlite_error_code( int rt )
{
  int r3;

  r3 = ( rt == 0 ) ? SQLITE_OK : ( rt < 0 ? rt - DUF_SQLITE_ERROR_BASE : rt );
  return r3;
}


