/* #undef MAS_TRACING */
#include <mastar/sqlite/mas_sqlite.h>
#include <mastar/sqlite/mas_sqlite_const.h>

#include "duf_tracen_defs.h"                                         /* DUF_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_service.h"

#include "duf_config_ref.h"

#include "duf_sql_error.h"

/* ###################################################################### */
#include "duf_sql_positional.h"
/* ###################################################################### */

int
duf_sql_column_int( mas_sqlite_stmt_t * stmt, int icol )
{
  return mas_sqlite_column_int( stmt, icol );
}

long long
duf_sql_column_long_long( mas_sqlite_stmt_t * stmt, int icol )
{
  return mas_sqlite_column_long_long( stmt, icol );
}

const char *
duf_sql_column_string( mas_sqlite_stmt_t * stmt, int icol )
{
  return mas_sqlite_column_string( stmt, icol );
}
