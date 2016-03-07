/* #undef MAS_TRACING */
#include <mastar/sqlite/mas_sqlite_const.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

/* #include "duf_tracen_defs.h"                                         (* T; TT; TR ♠ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ *) */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ♠ *) */
/* #include "duf_dodefs.h"                                              (* DOR ♠ *) */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

/* ###################################################################### */
#include "duf_sql_error.h"
/* ###################################################################### */

mas_error_code_t
duf_sqlite2r_error_code( int r3 )
{
  mas_error_code_t rc;

  rc = ( r3 == SQLITE_OK ) ? 0 : ( r3 > 0 ? MAS_SQLITE_ERROR_BASE + r3 : r3 );
  return rc;
}

int
duf_r2sqlite_error_code( mas_error_code_t rc )
{
  int r3;

  r3 = ( rc == 0 ) ? SQLITE_OK : ( rc < 0 ? rc - MAS_SQLITE_ERROR_BASE : rc );
  return r3;
}

SR( SQL, sqlite2duf, int r3, const char *func, int line )
{
/* DUF_STARTR( r ); */
  mas_error_code_t rc = 0;

  rc = duf_sqlite2r_error_code( r3 );
  if ( rc<0 )
    ERRMAKE_FLV( rc, func, line );
/* DUF_MAKE_ERRORFL( r, rc, func, line ); */
/* DUF_ENDR( r ); */
  ER( SQL, sqlite2duf, int r3, const char *func, int line );
}
