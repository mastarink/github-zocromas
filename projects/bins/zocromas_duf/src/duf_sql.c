#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"

#include "duf_service.h"

#include "duf_config_ref.h"

#include "duf_sqlite.h"

/* ###################################################################### */
#include "duf_sql.h"
/* ###################################################################### */


int
duf_sql_open( const char *dbpath )
{
  return DUF_SQLITE_ERROR_CODE( duf_sqlite_open( dbpath ) );
}

int
duf_sql_close( void )
{
  return DUF_SQLITE_ERROR_CODE( duf_sqlite_close(  ) );
}

int
duf_sql_exec_c( const char *sql, int constraint_ignore, int *pchanges )
{
  DEBUG_STARTR( r );

  r = DUF_SQLITE_ERROR_CODE( duf_sqlite_exec_c( sql, constraint_ignore, pchanges ) );

  DUF_TEST_R( r );
  DEBUG_ENDR( r );
}

/* static int                                                         */
/* duf_sql_exec( const char *sql, int *pchanges )                     */
/* {                                                                  */
/*   DEBUG_STARTR(r  );                                               */
/*                                                                    */
/*   r = DUF_SQLITE_ERROR_CODE( duf_sqlite_exec_e( sql, pchanges ) ); */
/*                                                                    */
/*   DUF_TEST_R( r );                                                 */
/*   DEBUG_ENDR( r );                                                 */
/* }                                                                  */

/* static int                                                                                  */
/* duf_sql_execcb( const char *sql, duf_sqexe_cb_t sqexe_cb, void *sqexe_data, int *pchanges ) */
/* {                                                                                           */
/*   DEBUG_STARTR( r );                                                                        */
/*   r = DUF_SQLITE_ERROR_CODE( duf_sqlite_execcb_e( sql, sqexe_cb, sqexe_data, pchanges ) );  */
/*                                                                                             */
/*   DUF_TEST_R( r );                                                                          */
/*   DEBUG_ENDR( r );                                                                          */
/* }                                                                                           */

static int
duf_vsql_c( const char *fmt, int constraint_ignore, int *pchanges, va_list args )
{
  DEBUG_STARTR( r );


  DUF_TRACE( sql, 1, " [[%s]]", fmt );
  r = DUF_SQLITE_ERROR_CODE( duf_vsqlite_c( fmt, constraint_ignore, pchanges, args ) );
  DUF_TRACE( sql, 1, " [[%s]] : %d", fmt, r );
  DEBUG_ENDR( r );
}

static int
duf_sql_vselect( duf_sel_cb_t sel_cb, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata,
                 duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, const char *sqlfmt, va_list args )
{
  DEBUG_STARTR( r );

  r = DUF_SQLITE_ERROR_CODE( duf_sqlite_vselect( sel_cb, sel_cb_udata, str_cb, str_cb_udata, pdi, sccb, sqlfmt, args ) );
  DEBUG_ENDR( r );
}

unsigned long long
duf_sql_last_insert_rowid( void )
{
  return duf_sqlite_last_insert_rowid(  );
}

/************************************************************************/
/************************************************************************/
/************************************************************************/

static int
duf_sql_exec_c_msg( const char *sql, const char *msg, int constraint_ignore )
{
  DEBUG_STARTR( r );
  r = duf_sql_exec_c( sql, constraint_ignore, ( int * ) NULL );
  DUF_TRACE( sql, 1, "[%-40s] %s (%d)", msg, r != SQLITE_OK ? "FAIL" : "OK", r );
  DEBUG_ENDR( r );
}

int
duf_sql_exec_msg( const char *sql, const char *msg )
{
  DEBUG_STARTR( r );
  r = duf_sql_exec_c_msg( sql, msg, DUF_CONSTRAINT_IGNORE_NO );
  if ( r )
    DUF_SHOW_ERROR( "SQL EXEC ERROR in [%s]", sql );

  DEBUG_ENDR( r );
}


/* int                                                                     */
/* duf_sql_c( const char *fmt, int constraint_ignore, int *pchanges, ... ) */
/* {                                                                       */
/*   DEBUG_STARTR( r );                                                    */
/*   va_list args;                                                         */
/*                                                                         */
/*   va_start( args, pchanges );                                           */
/*   r = duf_vsql_c( fmt, constraint_ignore, pchanges, args );             */
/*   DUF_TRACE( sql, 1, " [[%s]] : %d", fmt, r );                          */
/*   va_end( args );                                                       */
/*   if ( !constraint_ignore || r != DUF_SQL_CONSTRAINT )                  */
/*     DUF_TEST_R( r );                                                    */
/*   DEBUG_ENDR( r );                                                      */
/* }                                                                       */

int
duf_sql( const char *fmt, int *pchanges, ... )
{
  DEBUG_STARTR( r );
  va_list args;

  va_start( args, pchanges );
  r = duf_vsql_c( fmt, DUF_CONSTRAINT_IGNORE_NO, pchanges, args );
  DUF_TRACE( sql, 1, " [[%s]] : %d", fmt, r );
  va_end( args );
  DEBUG_ENDR( r );
}



/*
 *  sel_cb -- callback called for each row with:
 *    sel_cb_udata	     passed
 *    str_cb + str_cb_udata  passed to be called there
 *  sqlfmt + ... - sql
 * */
int
duf_sql_select( duf_sel_cb_t sel_cb, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata,
                duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, const char *sqlfmt, ... )
{
  DEBUG_STARTR( r );
  va_list args;

  va_start( args, sqlfmt );
  r = duf_sql_vselect( sel_cb, sel_cb_udata, str_cb, str_cb_udata, pdi, sccb, sqlfmt, args );
  va_end( args );
  DEBUG_ENDR( r );
}

int
duf_sql_prepare( const char *sql, duf_sqlite_stmt_t ** pstmt )
{
  DEBUG_STARTR( r );

  DOR_NOE( r, DUF_SQLITE_ERROR_CODE( duf_sqlite_prepare( sql, pstmt ) ), DUF_SQL_ROW, DUF_SQL_DONE );

  DUF_TRACE( sql, 2, " [[%s]]", sql );
  DEBUG_ENDR( r );
}

int
duf_sql_step( duf_sqlite_stmt_t * stmt )
{
  DEBUG_STARTR( r );

  DUF_TRACE( sql, 2, " [[%s]]", sqlite3_sql( stmt ) );
  DOR_NOE( r, DUF_SQLITE_ERROR_CODE( duf_sqlite_step( stmt ) ), DUF_SQL_ROW, DUF_SQL_DONE );
  DUF_TRACE( sql, 0, " [[%s]]", sqlite3_sql( stmt ) );
  DEBUG_ENDR( r );
}

int
duf_sql_finalize( duf_sqlite_stmt_t * stmt )
{
  DEBUG_STARTR( r );

  r = DUF_SQLITE_ERROR_CODE( duf_sqlite_finalize( stmt ) );
  DUF_TRACE( sql, 6, "-" );
  DUF_TEST_R( r );
  DEBUG_ENDR( r );
}

int
duf_sql_reset( duf_sqlite_stmt_t * stmt )
{
  DEBUG_STARTR( r );

  r = DUF_SQLITE_ERROR_CODE( duf_sqlite_reset( stmt ) );
  DUF_TRACE( sql, 6, "-" );
  DUF_TEST_R( r );
  DEBUG_ENDR( r );
}

int
duf_sql_bindu_long_long( duf_sqlite_stmt_t * stmt, const char *fldname, int pi, long long value )
{
  DEBUG_STARTR( r );

  if ( fldname )
    r = pi = duf_sqlite_bind_parameter_index( stmt, fldname );
  if ( pi > 0 )
  {
    r = DUF_SQLITE_ERROR_CODE( duf_sqlite_bind_long_long( stmt, pi, value ) );
    DUF_TEST_R( r );
    DUF_TRACE( sql, 5, "long long %s='%lld'", fldname, value );
  }
  else if ( !r )
  {
    r = DUF_ERROR_BIND_NAME;
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  else
    DUF_TEST_R( r );
  DEBUG_ENDR( r );
}

int
duf_sql_bindu_long_long_nz( duf_sqlite_stmt_t * stmt, const char *fldname, int pi, long long value )
{
  DEBUG_STARTR( r );

  if ( fldname )
    r = pi = duf_sqlite_bind_parameter_index( stmt, fldname );
  if ( pi > 0 )
  {
    if ( value )
      r = DUF_SQLITE_ERROR_CODE( duf_sqlite_bind_long_long( stmt, pi, value ) );
    else
      r = DUF_SQLITE_ERROR_CODE( duf_sqlite_bind_null( stmt, pi ) );
    DUF_TEST_R( r );
    DUF_TRACE( sql, 5, "long long nz %s='%lld'", fldname, value );
  }
  else if ( !r )
  {
    r = DUF_ERROR_BIND_NAME;
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  else
    DUF_TEST_R( r );
  DEBUG_ENDR( r );
}

int
duf_sql_bindu_int( duf_sqlite_stmt_t * stmt, const char *fldname, int pi, int value )
{
  DEBUG_STARTR( r );

  if ( fldname )
    r = pi = duf_sqlite_bind_parameter_index( stmt, fldname );
  if ( pi > 0 )
  {
    r = DUF_SQLITE_ERROR_CODE( duf_sqlite_bind_int( stmt, pi, value ) );
    DUF_TEST_R( r );
    DUF_TRACE( sql, 5, "int %s='%d'", fldname, value );
  }
  else if ( !r )
  {
    r = DUF_ERROR_BIND_NAME;
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  else
    DUF_TEST_R( r );
  DEBUG_ENDR( r );
}

int
duf_sql_bindu_int_nz( duf_sqlite_stmt_t * stmt, const char *fldname, int pi, int value )
{
  DEBUG_STARTR( r );

  if ( fldname )
    r = pi = duf_sqlite_bind_parameter_index( stmt, fldname );
  if ( pi > 0 )
  {
    if ( value )
      r = DUF_SQLITE_ERROR_CODE( duf_sqlite_bind_int( stmt, pi, value ) );
    else
      r = DUF_SQLITE_ERROR_CODE( duf_sqlite_bind_null( stmt, pi ) );
    DUF_TEST_R( r );
    DUF_TRACE( sql, 5, "int nz %s='%d'", fldname, value );
  }
  else if ( !r )
  {
    r = DUF_ERROR_BIND_NAME;
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  else
    DUF_TEST_R( r );
  DEBUG_ENDR( r );
}

int
duf_sql_bindu_string( duf_sqlite_stmt_t * stmt, const char *fldname, int pi, const char *value )
{
  DEBUG_STARTR( r );

  if ( fldname )
    r = pi = duf_sqlite_bind_parameter_index( stmt, fldname );
  if ( pi > 0 )
  {
    if ( value )
      r = DUF_SQLITE_ERROR_CODE( duf_sqlite_bind_string( stmt, pi, value ) );
    else
      r = DUF_SQLITE_ERROR_CODE( duf_sqlite_bind_null( stmt, pi ) );
    DUF_TEST_R( r );
    DUF_TRACE( sql, 5, "string %s='%s'", fldname, value );
  }
  else if ( !r )
  {
    r = DUF_ERROR_BIND_NAME;
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  else
    DUF_TEST_R( r );
  DEBUG_ENDR( r );
}

int
duf_sql_bindn_long_long( duf_sqlite_stmt_t * stmt, const char *fldname, long long value )
{
  DEBUG_STARTR( r );
  int pi = 0;

  if ( fldname )
    r = pi = duf_sqlite_bind_parameter_index( stmt, fldname );
  if ( pi > 0 )
  {
    r = DUF_SQLITE_ERROR_CODE( duf_sqlite_bind_long_long( stmt, pi, value ) );
    DUF_TRACE( sql, 5, "long long %s='%lld'", fldname, value );
  }
  else if ( !r )
  {
    r = DUF_ERROR_BIND_NAME;
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  else
    DUF_TEST_R( r );
  DEBUG_ENDR( r );
}

int
duf_sql_bindn_long_long_nz( duf_sqlite_stmt_t * stmt, const char *fldname, long long value )
{
  DEBUG_STARTR( r );
  int pi = 0;

  if ( fldname )
    r = pi = duf_sqlite_bind_parameter_index( stmt, fldname );
  if ( pi > 0 )
  {
    /* DUF_PRINTF(0,  "1 long long nz %s='%lld' [%s]", fldname, value,  sqlite3_sql( stmt ) ); */
    if ( value )
      r = DUF_SQLITE_ERROR_CODE( duf_sqlite_bind_long_long( stmt, pi, value ) );
    else
      r = DUF_SQLITE_ERROR_CODE( duf_sqlite_bind_null( stmt, pi ) );
    DUF_TRACE( sql, 5, "long long nz %s='%lld'", fldname, value );
  }
  else if ( !r )
  {
    r = DUF_ERROR_BIND_NAME;
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  else
    DUF_TEST_R( r );
  DEBUG_ENDR( r );
}

int
duf_sql_bindn_int( duf_sqlite_stmt_t * stmt, const char *fldname, int value )
{
  DEBUG_STARTR( r );
  int pi = 0;

  if ( fldname )
    r = pi = duf_sqlite_bind_parameter_index( stmt, fldname );
  if ( pi > 0 )
  {
    r = DUF_SQLITE_ERROR_CODE( duf_sqlite_bind_int( stmt, pi, value ) );
    DUF_TEST_R( r );
    DUF_TRACE( sql, 5, "int %s='%d'", fldname, value );
  }
  else if ( !r )
  {
    r = DUF_ERROR_BIND_NAME;
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  else
    DUF_TEST_R( r );
  DEBUG_ENDR( r );
}

int
duf_sql_bindn_int_nz( duf_sqlite_stmt_t * stmt, const char *fldname, int value )
{
  DEBUG_STARTR( r );
  int pi = 0;

  if ( fldname )
    r = pi = duf_sqlite_bind_parameter_index( stmt, fldname );
  if ( pi > 0 )
  {
    if ( value )
      r = DUF_SQLITE_ERROR_CODE( duf_sqlite_bind_int( stmt, pi, value ) );
    else
      r = DUF_SQLITE_ERROR_CODE( duf_sqlite_bind_null( stmt, pi ) );
    DUF_TEST_R( r );
    DUF_TRACE( sql, 5, "int nz %s='%d'", fldname, value );
  }
  else if ( !r )
  {
    r = DUF_ERROR_BIND_NAME;
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  else
    DUF_TEST_R( r );
  DEBUG_ENDR( r );
}

int
duf_sql_bindn_string( duf_sqlite_stmt_t * stmt, const char *fldname, const char *value )
{
  DEBUG_STARTR( r );
  int pi = 0;

  if ( fldname )
    r = pi = duf_sqlite_bind_parameter_index( stmt, fldname );
  if ( pi > 0 )
  {
    if ( value )
      r = DUF_SQLITE_ERROR_CODE( duf_sqlite_bind_string( stmt, pi, value ) );
    else
      r = DUF_SQLITE_ERROR_CODE( duf_sqlite_bind_null( stmt, pi ) );
    DUF_TEST_R( r );
    DUF_TRACE( sql, 5, "string %s='%s'", fldname, value );
  }
  else if ( !r )
  {
    r = DUF_ERROR_BIND_NAME;
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  else
    DUF_TEST_R( r );
  DEBUG_ENDR( r );
}

int
duf_sql_bind_long_long( duf_sqlite_stmt_t * stmt, const char *fldname, long long value )
{
  DEBUG_STARTR( r );
  int pi;

  r = pi = duf_sqlite_bind_parameter_index( stmt, fldname );
  if ( pi > 0 )
  {
    r = DUF_SQLITE_ERROR_CODE( duf_sqlite_bind_long_long( stmt, pi, value ) );
    DUF_TEST_R( r );
    DUF_TRACE( sql, 5, "long long %s='%lld'", fldname, value );
  }
  else if ( !r )
  {
    r = DUF_ERROR_BIND_NAME;
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  else
    DUF_TEST_R( r );
  DEBUG_ENDR( r );
}

int
duf_sql_bind_long_long_nz( duf_sqlite_stmt_t * stmt, const char *fldname, long long value )
{
  DEBUG_STARTR( r );
  int pi;

  r = pi = duf_sqlite_bind_parameter_index( stmt, fldname );
  if ( pi > 0 )
  {
    if ( value )
      r = DUF_SQLITE_ERROR_CODE( duf_sqlite_bind_long_long( stmt, pi, value ) );
    else
      r = DUF_SQLITE_ERROR_CODE( duf_sqlite_bind_null( stmt, pi ) );
    /* DUF_TRACE( sql, 0, "long long nz %s='%lld'", fldname, value ); */
    /* DUF_TEST_R( r ); */
  }
  else if ( !r )
  {
    r = DUF_ERROR_BIND_NAME;
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  else
    DUF_TEST_R( r );
  DEBUG_ENDR( r );
}

int
duf_sql_bind_int( duf_sqlite_stmt_t * stmt, const char *fldname, int value )
{
  DEBUG_STARTR( r );
  int pi;

  r = pi = duf_sqlite_bind_parameter_index( stmt, fldname );
  if ( pi > 0 )
  {
    r = DUF_SQLITE_ERROR_CODE( duf_sqlite_bind_int( stmt, pi, value ) );
    DUF_TEST_R( r );
    DUF_TRACE( sql, 0, "int %s='%d'", fldname, value );
  }
  else if ( !r )
  {
    r = DUF_ERROR_BIND_NAME;
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  else
    DUF_TEST_R( r );
  DEBUG_ENDR( r );
}

int
duf_sql_bind_int_nz( duf_sqlite_stmt_t * stmt, const char *fldname, int value )
{
  DEBUG_STARTR( r );
  int pi;

  r = pi = duf_sqlite_bind_parameter_index( stmt, fldname );
  if ( pi > 0 )
  {
    if ( value )
      r = DUF_SQLITE_ERROR_CODE( duf_sqlite_bind_int( stmt, pi, value ) );
    else
      r = DUF_SQLITE_ERROR_CODE( duf_sqlite_bind_null( stmt, pi ) );
    DUF_TEST_R( r );
    DUF_TRACE( sql, 0, "int nz %s='%d'", fldname, value );
  }
  else if ( !r )
  {
    r = DUF_ERROR_BIND_NAME;
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  else
    DUF_TEST_R( r );
  DEBUG_ENDR( r );
}

int
duf_sql_bind_string( duf_sqlite_stmt_t * stmt, const char *fldname, const char *value )
{
  DEBUG_STARTR( r );
  int pi;

  r = pi = duf_sqlite_bind_parameter_index( stmt, fldname );
  if ( pi > 0 )
  {
    if ( value )
      r = DUF_SQLITE_ERROR_CODE( duf_sqlite_bind_string( stmt, pi, value ) );
    else
      r = DUF_SQLITE_ERROR_CODE( duf_sqlite_bind_null( stmt, pi ) );
    DUF_TEST_R( r );
    DUF_TRACE( sql, 0, "string %s='%s'", fldname, value );
  }
  else if ( !r )
  {
    r = DUF_ERROR_BIND_NAME;
    /* DUF_SHOW_ERROR( "wrong field name '%s' at %s", fldname, sqlite3_sql( stmt ) ); */
  }
  else
    DUF_TEST_R( r );
  DEBUG_ENDR( r );
}

int
duf_sql_changes( void )
{
  int changes = 0;

  changes = duf_sqlite_changes(  );
  DUF_TRACE( sql, 4, "changes=%d", changes );
  return changes;
}

int
duf_sql_column_int( duf_sqlite_stmt_t * stmt, int icol )
{
  return duf_sqlite_column_int( stmt, icol );
}

long long
duf_sql_column_long_long( duf_sqlite_stmt_t * stmt, int icol )
{
  return duf_sqlite_column_long_long( stmt, icol );
}

const char *
duf_sql_column_string( duf_sqlite_stmt_t * stmt, int icol )
{
  return duf_sqlite_column_string( stmt, icol );
}

const char *
duf_sql_column_name( duf_sqlite_stmt_t * stmt, int index )
{
  return duf_sqlite_column_name( stmt, index );
}

int
duf_sql_column_count( duf_sqlite_stmt_t * stmt )
{
  return duf_sqlite_column_count( stmt );
}

static char *
duf_sql_vmprintf( const char *fmt, va_list args )
{
  char *s = NULL;

  s = duf_sqlite_vmprintf( fmt, args );
  return s;
}

char *
duf_sql_mprintf( const char *fmt, ... )
{
  char *s = NULL;
  va_list args;

  va_start( args, fmt );
  s = duf_sql_vmprintf( fmt, args );
  DUF_TRACE( sql, 0, " [[%s]]", fmt );
  va_end( args );
  return s;
}

/* static void             */
/* duf_sql_free( char *s ) */
/* {                       */
/*   duf_sqlite_free( s ); */
/* }                       */

void
duf_sql_clear_bindings( duf_sqlite_stmt_t * stmt )
{
  duf_sqlite_clear_bindings( stmt );
}
