#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>
#include <mastar/sqlite/mas_sqlite.h>
#include <mastar/sqlite/mas_sqlite_const.h>


#include "duf_maintenance.h"

#include "duf_service.h"

#include "duf_config_ref.h"

#include "duf_sql_error.h"

#include "duf_sqlite_select.h"

/* ###################################################################### */
#include "duf_sql_open.h"
/* ###################################################################### */

int
duf_sql_open( const char *dbpath )
{
  DEBUG_STARTR( r );
  int r3 = 0;

  DUF_TRACE( explain, 0, "open database if fpath set; fpath:%s", DUF_CONFIGG( db.main.fpath ) );
  DOR( r, DUF_SQLITE2R_ERROR_CODE( ( r3 = mas_sqlite_open( dbpath ) ) ) );
  DUF_TRACE( sql, 1, "open database; dbpath:%s : %d", dbpath, r );
  DUF_TRACE( explain, 0, "opened (?%d) database", r );
  DEBUG_ENDR( r );
}

int
duf_sql_close( void )
{
  DEBUG_STARTR( r );
  DOR( r, DUF_SQLITE2R_ERROR_CODE( mas_sqlite_close(  ) ) );
  DEBUG_ENDR( r );
}
