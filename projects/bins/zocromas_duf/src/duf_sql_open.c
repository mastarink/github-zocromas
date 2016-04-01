/* #undef MAS_TRACING */
#include <mastar/sqlite/mas_sqlite.h>
#include <mastar/sqlite/mas_sqlite_const.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_db_get.h"
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_sql_error.h"
#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */

/* ###################################################################### */
#include "duf_sql_open.h"                                            /* duf_sql_open ✗ */
/* ###################################################################### */

SR( SQL, sql_open, const char *dbpath )
{
/* DUF_STARTR( r ); */

  MAST_TRACE( explain, 100, "open database if fpath set; fpath:%s", /* DUF_CONFIGG( db.main.fpath ) */ duf_get_config_db_main_fpath(  ) );
  CRV_SQLITE( mas_sqlite_open, dbpath );

  MAST_TRACE( sql, 1, "open database; dbpath:%s : %d", dbpath, QERRIND );
  MAST_TRACE( explain, 100, "opened (?%d) database", QERRIND );
/* DUF_ENDR( r ); */
  ER( SQL, sql_open, const char *dbpath );
}

SR( SQL, sql_close, void )
{
/* DUF_STARTR( r ); */
  CRV_SQLITE( mas_sqlite_close );
/* DUF_ENDR( r ); */
  ER( SQL, sql_close, void );
}
