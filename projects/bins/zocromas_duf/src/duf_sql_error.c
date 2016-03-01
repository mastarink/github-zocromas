/* #undef MAS_TRACING */
#include <mastar/sqlite/mas_sqlite_const.h>

#include <mastar/wrap/mas_std_def.h>

#include "duf_tracen_defs.h"                                         /* MAST_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */


#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */

/* ###################################################################### */
#include "duf_sql_error.h"
/* ###################################################################### */

mas_error_code_t
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

mas_error_index_t
duf_sqlite2duf( int r3, const char *func, int line )
{
  DUF_STARTR( r );
  mas_error_code_t rc = 0;

  rc = duf_sqlite2r_error_code( r3 );
  if ( rc < 0 )
    DUF_MAKE_ERRORFL( r, rc, func, line );
  DUF_ENDR( r );
}
