#include "duf_maintenance.h"

#include "duf_service.h"

#include "duf_config_ref.h"

#include <mastar/sqlite/mas_sqlite.h>

#include "duf_sql_defs.h"
/* ###################################################################### */
#include "duf_sql_error.h"
#include "duf_sql_bind.h"
/* ###################################################################### */

/* #define BI         DOR_SQL( r, (r3=mas_sqlite_bind_parameter_index( stmt, fldname ))) */
#define BI         mas_sqlite_bind_parameter_index( stmt, fldname )
#define NUB(fun)   DOR_SQL( r, value?fun( stmt, pi, value ):mas_sqlite_bind_null( stmt, pi ) )

int
duf_sql_bindu_long_long( mas_sqlite_stmt_t * stmt, const char *fldname, int pi, long long value )
{
  DEBUG_STARTR( r );

  if ( fldname )
    pi = BI;
  if ( pi > 0 )                 /*noerr */
  {
    DOR_SQL( r, mas_sqlite_bind_long_long( stmt, pi, value ) );
    DUF_TRACE( sql, 5, "long long %s='%lld'", fldname, value );
  }
  else if ( !r )
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_BIND_NAME );
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  DEBUG_ENDR( r );
}

int
duf_sql_bindu_long_long_nz( mas_sqlite_stmt_t * stmt, const char *fldname, int pi, long long value )
{
  DEBUG_STARTR( r );

  if ( fldname )
    pi = BI;
  if ( pi > 0 )
  {
    NUB( mas_sqlite_bind_long_long );
    DUF_TRACE( sql, 5, "long long nz %s='%lld'", fldname, value );
  }
  else if ( !r )
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_BIND_NAME );
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  DEBUG_ENDR( r );
}

int
duf_sql_bindu_int( mas_sqlite_stmt_t * stmt, const char *fldname, int pi, int value )
{
  DEBUG_STARTR( r );

  if ( fldname )
    pi = BI;
  if ( pi > 0 )
  {
    DOR_SQL( r, mas_sqlite_bind_int( stmt, pi, value ) );
    DUF_TRACE( sql, 5, "int %s='%d'", fldname, value );
  }
  else if ( !r )
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_BIND_NAME );
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  DEBUG_ENDR( r );
}

int
duf_sql_bindu_int_nz( mas_sqlite_stmt_t * stmt, const char *fldname, int pi, int value )
{
  DEBUG_STARTR( r );

  if ( fldname )
    pi = BI;
  if ( pi > 0 )
  {
    NUB( mas_sqlite_bind_int );
    DUF_TRACE( sql, 5, "int nz %s='%d'", fldname, value );
  }
  else if ( !r )
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_BIND_NAME );
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  DEBUG_ENDR( r );
}

int
duf_sql_bindu_string( mas_sqlite_stmt_t * stmt, const char *fldname, int pi, const char *value )
{
  DEBUG_STARTR( r );

  if ( fldname )
    pi = BI;
  if ( pi > 0 )
  {
    NUB( mas_sqlite_bind_string );
    DUF_TRACE( sql, 5, "string %s='%s'", fldname, value );
  }
  else if ( !r )
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_BIND_NAME );
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  DEBUG_ENDR( r );
}

int
duf_sql_bindn_long_long( mas_sqlite_stmt_t * stmt, const char *fldname, long long value )
{
  DEBUG_STARTR( r );
  int pi = 0;

  if ( fldname )
    pi = BI;
  if ( pi > 0 )
  {
    DOR_SQL( r, mas_sqlite_bind_long_long( stmt, pi, value ) );
    DUF_TRACE( sql, 5, "long long %s='%lld'", fldname, value );
  }
  else if ( !r )
  {
    DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) );
    DUF_MAKE_ERROR( r, DUF_ERROR_BIND_NAME );
  }
  DEBUG_ENDR( r );
}

int
duf_sql_bindn_long_long_nz( mas_sqlite_stmt_t * stmt, const char *fldname, long long value )
{
  DEBUG_STARTR( r );
  int pi = 0;

  if ( fldname )
    pi = BI;
  if ( pi > 0 )
  {
    /* DUF_PRINTF(0,  "1 long long nz %s='%lld' [%s]", fldname, value,  sqlite3_sql( stmt ) ); */
    NUB( mas_sqlite_bind_long_long );
    DUF_TRACE( sql, 5, "long long nz %s='%lld'", fldname, value );
  }
  else if ( !r )
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_BIND_NAME );
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  DEBUG_ENDR( r );
}

int
duf_sql_bindn_int( mas_sqlite_stmt_t * stmt, const char *fldname, int value )
{
  DEBUG_STARTR( r );
  int pi = 0;

  if ( fldname )
    pi = BI;
  if ( pi > 0 )
  {
    DOR_SQL( r, mas_sqlite_bind_int( stmt, pi, value ) );
    DUF_TRACE( sql, 5, "int %s='%d'", fldname, value );
  }
  else if ( !r )
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_BIND_NAME );
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  DEBUG_ENDR( r );
}

int
duf_sql_bindn_int_nz( mas_sqlite_stmt_t * stmt, const char *fldname, int value )
{
  DEBUG_STARTR( r );
  int pi = 0;

  if ( fldname )
    pi = BI;
  if ( pi > 0 )
  {
    NUB( mas_sqlite_bind_int );
    DUF_TRACE( sql, 5, "int nz %s='%d'", fldname, value );
  }
  else if ( !r )
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_BIND_NAME );
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  DEBUG_ENDR( r );
}

int
duf_sql_bindn_string( mas_sqlite_stmt_t * stmt, const char *fldname, const char *value )
{
  DEBUG_STARTR( r );
  int pi = 0;

  if ( fldname )
    pi = BI;
  if ( pi > 0 )
  {
    NUB( mas_sqlite_bind_string );
    DUF_TRACE( sql, 5, "string %s='%s'", fldname, value );
  }
  else if ( !r )
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_BIND_NAME );
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  DEBUG_ENDR( r );
}

int
duf_sql_bind_long_long( mas_sqlite_stmt_t * stmt, const char *fldname, long long value )
{
  DEBUG_STARTR( r );
  int pi;

  pi = BI;
  if ( pi > 0 )
  {
    DOR_SQL( r, mas_sqlite_bind_long_long( stmt, pi, value ) );
    DUF_TRACE( sql, 5, "long long %s='%lld'", fldname, value );
  }
  else if ( !r )
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_BIND_NAME );
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  DEBUG_ENDR( r );
}

int
duf_sql_bind_long_long_nz( mas_sqlite_stmt_t * stmt, const char *fldname, long long value )
{
  DEBUG_STARTR( r );
  int pi;

  pi = BI;
  if ( pi > 0 )
  {
    NUB( mas_sqlite_bind_long_long );
    /* DUF_TRACE( sql, 0, "long long nz %s='%lld'", fldname, value ); */
  }
  else if ( !r )
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_BIND_NAME );
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  DEBUG_ENDR( r );
}

int
duf_sql_bind_int( mas_sqlite_stmt_t * stmt, const char *fldname, int value )
{
  DEBUG_STARTR( r );
  int pi;

  pi = BI;
  if ( pi > 0 )
  {
    DOR_SQL( r, mas_sqlite_bind_int( stmt, pi, value ) );
    DUF_TRACE( sql, 0, "int %s='%d'", fldname, value );
  }
  else if ( !r )
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_BIND_NAME );
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  DEBUG_ENDR( r );
}

int
duf_sql_bind_int_nz( mas_sqlite_stmt_t * stmt, const char *fldname, int value )
{
  DEBUG_STARTR( r );
  int pi;

  pi = BI;
  if ( pi > 0 )
  {
    NUB( mas_sqlite_bind_int );
    DUF_TRACE( sql, 0, "int nz %s='%d'", fldname, value );
  }
  else if ( !r )
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_BIND_NAME );
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  DEBUG_ENDR( r );
}

int
duf_sql_bind_string( mas_sqlite_stmt_t * stmt, const char *fldname, const char *value )
{
  DEBUG_STARTR( r );
  int pi;

  pi = BI;
  if ( pi > 0 )
  {
    NUB( mas_sqlite_bind_string );
    DUF_TRACE( sql, 0, "string %s='%s'", fldname, value );
  }
  else if ( !r )
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_BIND_NAME );
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  DEBUG_ENDR( r );
}

void
duf_sql_clear_bindings( mas_sqlite_stmt_t * stmt )
{
  mas_sqlite_clear_bindings( stmt );
}

const char *
duf_sql_stmt(  mas_sqlite_stmt_t * stmt )
{
  return sqlite3_sql( stmt );
}
