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
#include "duf_sql.h"
/* ###################################################################### */

int
duf_sql_prepare( const char *sql, mas_sqlite_stmt_t ** pstmt )
{
  DEBUG_STARTR( r );

  DOR_NOE( r, DUF_SQLITE2R_ERROR_CODE( mas_sqlite_prepare( sql, pstmt ) ), MAS_SQL_ROW, MAS_SQL_DONE );

  DUF_TRACE( sql, 2, "          [[%s]]", sql );
  DEBUG_ENDR( r );
}

int
duf_sql_step( mas_sqlite_stmt_t * stmt )
{
  DEBUG_STARTR( r );

  DOR_NOE( r, DUF_SQLITE2R_ERROR_CODE( mas_sqlite_step( stmt ) ), MAS_SQL_ROW, MAS_SQL_DONE );
  DUF_TRACE( sql, 0, "(%s) [[%s]]", duf_error_name( r ), sqlite3_sql( stmt ) );
  DEBUG_ENDR( r );
}

int
duf_sql_finalize( mas_sqlite_stmt_t * stmt )
{
  DEBUG_STARTR( r );

  DOR( r, DUF_SQLITE2R_ERROR_CODE( mas_sqlite_finalize( stmt ) ) );
  DUF_TRACE( sql, 6, "-" );
  DEBUG_ENDR( r );
}

int
duf_sql_reset( mas_sqlite_stmt_t * stmt )
{
  DEBUG_STARTR( r );

  DOR( r, DUF_SQLITE2R_ERROR_CODE( mas_sqlite_reset( stmt ) ) );
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
