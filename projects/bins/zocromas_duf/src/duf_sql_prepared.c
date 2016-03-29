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

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_sql_error.h"
#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */

/* ###################################################################### */
#include "duf_sql_prepared.h"                                        /* duf_sql_(prepare|step|finalize) ✗ */
/* ###################################################################### */

SR( SQL, sql_prepare, const char *sql, mas_sqlite_stmt_t ** pstmt )
{
/* DUF_STARTR( r ); */

  MAST_TRACE( sql, 2, "@@@@@          [[%s]]", sql );
  ERRLOWER1( SQL_ROW );
  ERRLOWER1( SQL_DONE );
/* DOR_SQLITE_LOWERE( r, mas_sqlite_prepare( sql, pstmt ), DUF_SQL_ROW, DUF_SQL_DONE ); */
  CRV_SQLITE( mas_sqlite_prepare, sql, pstmt );
  ERRUPPER1( SQL_DONE );
  ERRUPPER1( SQL_ROW );

/* assert( r >= 0 ); */
/* DUF_ENDR( r ); */
  ER( SQL, sql_prepare, const char *sql, mas_sqlite_stmt_t ** pstmt );
}

SR( SQL, sql_step, mas_sqlite_stmt_t * stmt )
{
/* DUF_STARTR( r ); */

  ERRLOWER1( SQL_ROW );
  ERRLOWER1( SQL_DONE );
/* DOR_SQLITE_LOWERE( r, mas_sqlite_step( stmt ), DUF_SQL_ROW, DUF_SQL_DONE ); */
  CRV_SQLITE( mas_sqlite_step, stmt );
  ERRUPPER1( SQL_DONE );
  ERRUPPER1( SQL_ROW );
  if ( QISERR )
    mas_set_error_message_i( QERRIND, "sql:%s - %s", sqlite3_sql( stmt ), sqlite3_errmsg( mas_sqlite_pdb(  ) ) );
  MAST_TRACE( sql, 2, "(%s) [[%s]]", QERRNAME, sqlite3_sql( stmt ) );
/* assert( mas_error_code_i( r ) != DUF_SQL_CONSTRAINT_FOREIGNKEY ); */
/* DUF_ENDR( r ); */
  ER( SQL, sql_step, mas_sqlite_stmt_t * stmt );
}

SR( SQL, sql_finalize, mas_sqlite_stmt_t * stmt )
{
/* DUF_STARTR( r ); */

  CRV_SQLITE( mas_sqlite_finalize, stmt );
  MAST_TRACE( sql, 6, "-" );
/* DUF_ENDR( r ); */
  ER( SQL, sql_finalize, mas_sqlite_stmt_t * stmt );
}

SR( SQL, sql_reset, mas_sqlite_stmt_t * stmt )
{
/* DUF_STARTR( r ); */

  CRV_SQLITE( mas_sqlite_reset, stmt );
  MAST_TRACE( sql, 6, "-" );
/* DUF_ENDR( r ); */
  ER( SQL, sql_reset, mas_sqlite_stmt_t * stmt );
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

duf_sqltype_t
duf_sql_column_type( mas_sqlite_stmt_t * stmt, int index )
{
  int t;
  duf_sqltype_t rt;

/*
 * SQLITE_INTEGER
 * SQLITE_FLOAT
 * SQLITE_TEXT
 * SQLITE_BLOB
 * SQLITE_NULL
 * */
  t = mas_sqlite_column_type( stmt, index );
  switch ( t )
  {
  case SQLITE_INTEGER:
    rt = DUF_SQLTYPE_INTEGER;
    break;
  case SQLITE_FLOAT:
    rt = DUF_SQLTYPE_FLOAT;
    break;
  case SQLITE_TEXT:
    rt = DUF_SQLTYPE_TEXT;
    break;
  case SQLITE_BLOB:
    rt = DUF_SQLTYPE_BLOB;
    break;
  case SQLITE_NULL:
    rt = DUF_SQLTYPE_NULL;
    break;
  }
  return rt;
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
