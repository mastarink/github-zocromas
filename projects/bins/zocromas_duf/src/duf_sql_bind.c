/* #undef MAS_TRACING */
#include <mastar/wrap/mas_std_def.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_service.h"

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include <mastar/sqlite/mas_sqlite.h>

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */
/* ###################################################################### */
#include "duf_sql_error.h"
#include "duf_sql_bind.h"                                            /* duf_sql_... for DUF_SQL_BIND_... etc. ✗ */
/* ###################################################################### */

/* #define BI         DOR_SQLITE( r, (r3=mas_sqlite_bind_parameter_index( stmt, fldname ))) */
#define BI         mas_sqlite_bind_parameter_index( stmt, fldname )
/* #define NUB(_fun)   DOR_SQLITE( r, value?_fun( stmt, pi, value ):mas_sqlite_bind_null( stmt, pi ) ) */
#define NUB(_fun)   { if (value) { CRV_SQLITE(_fun, stmt, pi, value); } else { CRV_SQLITE(mas_sqlite_bind_null, stmt, pi ); } }

SR( SQL, sql_bindu_long_long, mas_sqlite_stmt_t * stmt, const char *fldname, int pi, long long value, int opt )
{
/* DUF_STARTR( r ); */

  if ( fldname )
    pi = BI;
  if ( pi > 0 )                                                      /*noerr */
  {
    CRV_SQLITE( mas_sqlite_bind_long_long, stmt, pi, value );
    MAST_TRACE( sql, 5, "long long %s='%lld'", fldname, value );
  }
  else if ( QNOERR && !opt )
  {
  /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
    ERRMAKE_M( BIND_NAME, "1. wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) );
  }
/* DUF_ENDR( r ); */
  ER( SQL, sql_bindu_long_long, mas_sqlite_stmt_t * stmt, const char *fldname, int pi, long long value, int opt );
}

SR( SQL, sql_bindu_long_long_nz, mas_sqlite_stmt_t * stmt, const char *fldname, int pi, long long value, int opt )
{
/* DUF_STARTR( r ); */

  if ( fldname )
    pi = BI;
  if ( pi > 0 )
  {
    NUB( mas_sqlite_bind_long_long );
    MAST_TRACE( sql, 5, "long long nz %s='%lld'", fldname, value );
  }
  else if ( QNOERR && !opt )
  {
  /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
    ERRMAKE_M( BIND_NAME, "2. wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) );
  }
/* DUF_ENDR( r ); */
  ER( SQL, sql_bindu_long_long_nz, mas_sqlite_stmt_t * stmt, const char *fldname, int pi, long long value, int opt );
}

SR( SQL, sql_bindu_int, mas_sqlite_stmt_t * stmt, const char *fldname, int pi, int value, int opt )
{
/* DUF_STARTR( r ); */

  if ( fldname )
    pi = BI;
  if ( pi > 0 )
  {
    CRV_SQLITE( mas_sqlite_bind_int, stmt, pi, value );
    MAST_TRACE( sql, 5, "int %s='%d'", fldname, value );
  }
  else if ( QNOERR && !opt )
  {
  /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
    ERRMAKE_M( BIND_NAME, "3. wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) );
  }
/* DUF_ENDR( r ); */
  ER( SQL, sql_bindu_int, mas_sqlite_stmt_t * stmt, const char *fldname, int pi, int value, int opt );
}

SR( SQL, sql_bindu_int_nz, mas_sqlite_stmt_t * stmt, const char *fldname, int pi, int value, int opt )
{
/* DUF_STARTR( r ); */

  if ( fldname )
    pi = BI;
  if ( pi > 0 )
  {
    NUB( mas_sqlite_bind_int );
    MAST_TRACE( sql, 5, "int nz %s='%d'", fldname, value );
  }
  else if ( QNOERR && !opt )
  {
  /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
    ERRMAKE_M( BIND_NAME, "4. wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) );
  }
/* DUF_ENDR( r ); */
  ER( SQL, sql_bindu_int_nz, mas_sqlite_stmt_t * stmt, const char *fldname, int pi, int value, int opt );
}

SR( SQL, sql_bindu_string, mas_sqlite_stmt_t * stmt, const char *fldname, int pi, const char *value, int opt )
{
/* DUF_STARTR( r ); */

  if ( fldname )
    pi = BI;
  if ( pi > 0 )
  {
    NUB( mas_sqlite_bind_string );
    MAST_TRACE( sql, 5, "string %s='%s'", fldname, value );
  }
  else if ( QNOERR && !opt )
  {
  /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
    ERRMAKE_M( BIND_NAME, "5. wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) );
  }
/* DUF_ENDR( r ); */
  ER( SQL, sql_bindu_string, mas_sqlite_stmt_t * stmt, const char *fldname, int pi, const char *value, int opt );
}

SR( SQL, sql_bindn_long_long, mas_sqlite_stmt_t * stmt, const char *fldname, long long value, int opt )
{
/* DUF_STARTR( r ); */
  int pi = 0;

  if ( fldname )
    pi = BI;
  if ( pi > 0 )
  {
    CRV_SQLITE( mas_sqlite_bind_long_long, stmt, pi, value );
    MAST_TRACE( sql, 5, "long long %s='%lld'", fldname, value );
  }
  else if ( QNOERR && !opt )
  {
  /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
    ERRMAKE_M( BIND_NAME, "6. wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) );
  }
/* DUF_ENDR( r ); */
  ER( SQL, sql_bindn_long_long, mas_sqlite_stmt_t * stmt, const char *fldname, long long value, int opt );
}

SR( SQL, sql_bindn_long_long_nz, mas_sqlite_stmt_t * stmt, const char *fldname, long long value, int opt )
{
/* DUF_STARTR( r ); */
  int pi = 0;

  if ( fldname )
    pi = BI;
  if ( pi > 0 )
  {
  /* DUF_PRINTF(0,  "1 long long nz %s='%lld' [%s]", fldname, value,  sqlite3_sql( stmt ) ); */
    NUB( mas_sqlite_bind_long_long );
    MAST_TRACE( sql, 5, "long long nz %s='%lld'", fldname, value );
  }
  else if ( QNOERR && !opt )
  {
  /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
    ERRMAKE_M( BIND_NAME, "7. wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) );
  }
/* DUF_ENDR( r ); */
  ER( SQL, sql_bindn_long_long_nz, mas_sqlite_stmt_t * stmt, const char *fldname, long long value, int opt );
}

SR( SQL, sql_bindn_int, mas_sqlite_stmt_t * stmt, const char *fldname, int value, int opt )
{
/* DUF_STARTR( r ); */
  int pi = 0;

  if ( fldname )
    pi = BI;
  if ( pi > 0 )
  {
    CRV_SQLITE( mas_sqlite_bind_int, stmt, pi, value );
    MAST_TRACE( sql, 5, "int %s='%d'", fldname, value );
  }
  else if ( QNOERR && !opt )
  {
  /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
    ERRMAKE_M( BIND_NAME, "8. wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) );
  }
/* DUF_ENDR( r ); */
  ER( SQL, sql_bindn_int, mas_sqlite_stmt_t * stmt, const char *fldname, int value, int opt );
}

SR( SQL, sql_bindn_int_nz, mas_sqlite_stmt_t * stmt, const char *fldname, int value, int opt )
{
/* DUF_STARTR( r ); */
  int pi = 0;

  if ( fldname )
    pi = BI;
  if ( pi > 0 )
  {
    NUB( mas_sqlite_bind_int );
    MAST_TRACE( sql, 5, "int nz %s='%d'", fldname, value );
  }
  else if ( QNOERR && !opt )
  {
  /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
    ERRMAKE_M( BIND_NAME, "9. wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) );
  }
/* DUF_ENDR( r ); */
  ER( SQL, sql_bindn_int_nz, mas_sqlite_stmt_t * stmt, const char *fldname, int value, int opt );
}

SR( SQL, sql_bindn_string, mas_sqlite_stmt_t * stmt, const char *fldname, const char *value, int opt )
{
/* DUF_STARTR( r ); */
  int pi = 0;

  if ( fldname )
    pi = BI;
  if ( pi > 0 )
  {
    NUB( mas_sqlite_bind_string );
    MAST_TRACE( sql, 5, "string %s='%s'", fldname, value );
  }
  else if ( QNOERR && !opt )
  {
  /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
    ERRMAKE_M( BIND_NAME, "10. wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) );
  }
/* DUF_ENDR( r ); */
  ER( SQL, sql_bindn_string, mas_sqlite_stmt_t * stmt, const char *fldname, const char *value, int opt );
}

SR( SQL, sql_bind_long_long, mas_sqlite_stmt_t * stmt, const char *fldname, long long value, int opt )
{
/* DUF_STARTR( r ); */
  int pi;

  pi = BI;
  if ( pi > 0 )
  {
    CRV_SQLITE( mas_sqlite_bind_long_long, stmt, pi, value );
    MAST_TRACE( sql, 5, "long long %s='%lld'", fldname, value );
  }
  else if ( QNOERR && !opt )
  {
  /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
    ERRMAKE_M( BIND_NAME, "11. wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) );
  }
/* DUF_ENDR( r ); */
  ER( SQL, sql_bind_long_long, mas_sqlite_stmt_t * stmt, const char *fldname, long long value, int opt );
}

SR( SQL, sql_bind_long_long_nz, mas_sqlite_stmt_t * stmt, const char *fldname, long long value, int opt )
{
/* DUF_STARTR( r ); */
  int pi;

  pi = BI;
  if ( pi > 0 )
  {
    NUB( mas_sqlite_bind_long_long );
  /* MAST_TRACE( sql, 0, "long long nz %s='%lld'", fldname, value ); */
  }
  else if ( QNOERR && !opt )
  {
  /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
    ERRMAKE_M( BIND_NAME, "12. wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) );
  }
/* DUF_ENDR( r ); */
  ER( SQL, sql_bind_long_long_nz, mas_sqlite_stmt_t * stmt, const char *fldname, long long value, int opt );
}

SR( SQL, sql_bind_int, mas_sqlite_stmt_t * stmt, const char *fldname, int value, int opt )
{
/* DUF_STARTR( r ); */
  int pi;

  pi = BI;
  if ( pi > 0 )
  {
    CRV_SQLITE( mas_sqlite_bind_int, stmt, pi, value );
    MAST_TRACE( sql, 0, "int %s='%d'", fldname, value );
  }
  else if ( QNOERR && !opt )
  {
  /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
    ERRMAKE_M( BIND_NAME, "13. wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) );
  }
/* DUF_ENDR( r ); */
  ER( SQL, sql_bind_int, mas_sqlite_stmt_t * stmt, const char *fldname, int value, int opt );
}

SR( SQL, sql_bind_int_nz, mas_sqlite_stmt_t * stmt, const char *fldname, int value, int opt )
{
/* DUF_STARTR( r ); */
  int pi;

  pi = BI;
  if ( pi > 0 )
  {
    NUB( mas_sqlite_bind_int );
    MAST_TRACE( sql, 0, "int nz %s='%d'", fldname, value );
  }
  else if ( QNOERR && !opt )
  {
  /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
    ERRMAKE_M( BIND_NAME, "14. wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) );
  }
/* DUF_ENDR( r ); */
  ER( SQL, sql_bind_int_nz, mas_sqlite_stmt_t * stmt, const char *fldname, int value, int opt );
}

SR( SQL, sql_bind_string, mas_sqlite_stmt_t * stmt, const char *fldname, const char *value, int opt )
{
/* DUF_STARTR( r ); */
  int pi;

  pi = BI;
  if ( pi > 0 )
  {
    NUB( mas_sqlite_bind_string );
    MAST_TRACE( sql, 0, "string %s='%s'", fldname, value );
  }
  else if ( QNOERR && !opt )
  {
  /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
    ERRMAKE_M( BIND_NAME, "15. wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) );
  }
/* DUF_ENDR( r ); */
  ER( SQL, sql_bind_string, mas_sqlite_stmt_t * stmt, const char *fldname, const char *value, int opt );
}

void
duf_sql_clear_bindings( mas_sqlite_stmt_t * stmt )
{
  mas_sqlite_clear_bindings( stmt );
}

const char *
duf_sql_stmt( mas_sqlite_stmt_t * stmt )
{
  return sqlite3_sql( stmt );
}
