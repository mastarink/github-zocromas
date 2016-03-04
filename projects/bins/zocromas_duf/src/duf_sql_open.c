/* #undef MAS_TRACING */
#include <mastar/sqlite/mas_sqlite.h>
#include <mastar/sqlite/mas_sqlite_const.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ♠ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>

#include "duf_tracen_defs.h"                                         /* T; TT; TR ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_config.h"                                              /* duf_get_config ♠ */
#include "duf_config_ref.h"
#include "duf_config_defs.h"                                         /* DUF_CONF... ♠ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */

#include "duf_sql_error.h"
#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ♠ */

/* ###################################################################### */
#include "duf_sql_open.h"                                            /* duf_sql_open() ♠ */
/* ###################################################################### */

int
duf_sql_open( const char *dbpath )
{
  DUF_STARTR( r );

  MAST_TRACE( explain, 0, "open database if fpath set; fpath:%s", DUF_CONFIGG( db.main.fpath ) );
  DOR_SQLITE( r, mas_sqlite_open( dbpath ) );

  MAST_TRACE( sql, 1, "open database; dbpath:%s : %d", dbpath, r );
  MAST_TRACE( explain, 0, "opened (?%d) database", r );
  DUF_ENDR( r );
}

int
duf_sql_close( void )
{
  DUF_STARTR( r );
  DOR_SQLITE( r, mas_sqlite_close(  ) );
  DUF_ENDR( r );
}
