#include <mastar/sqlite/mas_sqlite.h>
#include <mastar/sqlite/mas_sqlite_const.h>


#include "duf_maintenance.h"

/* #include "duf_config.h" */
#include "duf_config_trace.h"

#include "duf_sql_error.h"
#include "duf_sql_defs.h"

/* ###################################################################### */
#include "duf_sql_prepared.h"
/* ###################################################################### */

int
duf_sql_prepare( const char *sql, mas_sqlite_stmt_t ** pstmt )
{
  DEBUG_STARTR( r );

  DUF_TRACE( sql, 2, "@@@@@          [[%s]]", sql );
  DOR_SQLITE_LOWERE( r, mas_sqlite_prepare( sql, pstmt ), DUF_SQL_ROW, DUF_SQL_DONE );

  /* assert( r >= 0 ); */
  DEBUG_ENDR( r );
}

int
duf_sql_step( mas_sqlite_stmt_t * stmt )
{
  DEBUG_STARTR( r );

  DOR_SQLITE_LOWERE( r, mas_sqlite_step( stmt ), DUF_SQL_ROW, DUF_SQL_DONE );
  mas_set_error_message_i( r, "sql:%s - %s", sqlite3_sql( stmt ), sqlite3_errmsg( mas_sqlite_pdb(  ) ) );
  DUF_TRACE( sql, 2, "(%s) [[%s]]", mas_error_name_i( r ), sqlite3_sql( stmt ) );
  /* assert( mas_error_code_i( r ) != DUF_SQL_CONSTRAINT_FOREIGNKEY ); */
  DEBUG_ENDR( r );
}

int
duf_sql_finalize( mas_sqlite_stmt_t * stmt )
{
  DEBUG_STARTR( r );

  DOR_SQLITE( r, mas_sqlite_finalize( stmt ) );
  DUF_TRACE( sql, 6, "-" );
  DEBUG_ENDR( r );
}

int
duf_sql_reset( mas_sqlite_stmt_t * stmt )
{
  DEBUG_STARTR( r );

  DOR_SQLITE( r, mas_sqlite_reset( stmt ) );
  DUF_TRACE( sql, 6, "-" );
  DEBUG_ENDR( r );
}

int
duf_sql_changes( void )
{
  int changes = 0;

  changes = mas_sqlite_changes(  );
  DUF_TRACE( sql, 4, "changes=%d", changes );
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
