#include <mastar/sqlite/mas_sqlite.h>
#include <mastar/sqlite/mas_sqlite_const.h>


#include "duf_maintenance.h"

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
  DOR_SQLITE_NOE( r, mas_sqlite_prepare( sql, pstmt ), DUF_SQL_ROW, DUF_SQL_DONE );

  /* assert( r >= 0 ); */
  DEBUG_ENDR( r );
}

int
duf_sql_step( mas_sqlite_stmt_t * stmt )
{
  DEBUG_STARTR( r );
  int r3 = 0;

#if 1
  DOR_SQLITE_NOE( r, mas_sqlite_step( stmt ), DUF_SQL_ROW, DUF_SQL_DONE );
#else
  r3 = mas_sqlite_step( stmt );
   DUF_MAKE_ERROR( r, DUF_SQLITE2R_ERROR_CODE( r3 ) );
  /* DOR_SQLITE_NOE( r, r3, DUF_SQL_ROW, DUF_SQL_DONE ); */
#endif
  DUF_TRACE( sql, 3, "(%s) [[%s]]", duf_error_name_i( r ), sqlite3_sql( stmt ) );
  DUF_TRACE( sql, 3, "r3:%d %s r:%d (%s)", r3, duf_error_name_c( DUF_SQLITE2R_ERROR_CODE( r3 ) ), r, duf_error_name_i( r ) );
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
