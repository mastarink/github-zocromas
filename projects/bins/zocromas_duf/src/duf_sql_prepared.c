/* #undef MAS_TRACING */
#include <mastar/sqlite/mas_sqlite.h>
#include <mastar/sqlite/mas_sqlite_const.h>

#include "duf_tracen_defs_preset.h"


#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>

#include "duf_tracen_defs.h"                                         /* MAST_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */


/* #include "duf_config.h" */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */

#include "duf_sql_error.h"
#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ♠ */

/* ###################################################################### */
#include "duf_sql_prepared.h"
/* ###################################################################### */

int
duf_sql_prepare( const char *sql, mas_sqlite_stmt_t ** pstmt )
{
  DUF_STARTR( r );

  MAST_TRACE( sql, 2, "@@@@@          [[%s]]", sql );
  DOR_SQLITE_LOWERE( r, mas_sqlite_prepare( sql, pstmt ), DUF_SQL_ROW, DUF_SQL_DONE );

/* assert( r >= 0 ); */
  DUF_ENDR( r );
}

int
duf_sql_step( mas_sqlite_stmt_t * stmt )
{
  DUF_STARTR( r );

  DOR_SQLITE_LOWERE( r, mas_sqlite_step( stmt ), DUF_SQL_ROW, DUF_SQL_DONE );
  mas_set_error_message_i( r, "sql:%s - %s", sqlite3_sql( stmt ), sqlite3_errmsg( mas_sqlite_pdb(  ) ) );
  MAST_TRACE( sql, 2, "(%s) [[%s]]", mas_error_name_i( r ), sqlite3_sql( stmt ) );
/* assert( mas_error_code_i( r ) != DUF_SQL_CONSTRAINT_FOREIGNKEY ); */
  DUF_ENDR( r );
}

int
duf_sql_finalize( mas_sqlite_stmt_t * stmt )
{
  DUF_STARTR( r );

  DOR_SQLITE( r, mas_sqlite_finalize( stmt ) );
  MAST_TRACE( sql, 6, "-" );
  DUF_ENDR( r );
}

int
duf_sql_reset( mas_sqlite_stmt_t * stmt )
{
  DUF_STARTR( r );

  DOR_SQLITE( r, mas_sqlite_reset( stmt ) );
  MAST_TRACE( sql, 6, "-" );
  DUF_ENDR( r );
}

int
duf_sql_changes( void )
{
  int changes = 0;

  changes = mas_sqlite_changes(  );
  MAST_TRACE( sql, 4, "changes=%d", changes );
  return changes;
}

const char *
duf_sql_column_name( mas_sqlite_stmt_t * stmt, int index )
{
  return mas_sqlite_column_name( stmt, index );
}

const char *
duf_sql_column_decltype( mas_sqlite_stmt_t * stmt, int index )
{
  return mas_sqlite_column_decltype( stmt, index );
}

int
duf_sql_column_count( mas_sqlite_stmt_t * stmt )
{
  return mas_sqlite_column_count( stmt );
}

unsigned long long
duf_sql_last_insert_rowid( void )
{
  return mas_sqlite_last_insert_rowid(  );
}
