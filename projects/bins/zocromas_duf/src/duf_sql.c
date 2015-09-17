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
duf_sql_exec_c( const char *sql, int constraint_ignore, int *pchanges )
{
  DEBUG_STARTR( r );

  DOR( r, DUF_SQLITE2R_ERROR_CODE( mas_sqlite_exec_c( sql, constraint_ignore, pchanges ) ) );

  DEBUG_ENDR( r );
}

static int
duf_vsql_c( const char *fmt, int constraint_ignore, int *pchanges, va_list args )
{
  DEBUG_STARTR( r );


  DUF_TRACE( sql, 1, " [[%s]]", fmt );
  DOR( r, DUF_SQLITE2R_ERROR_CODE( mas_vsqlite_c( fmt, constraint_ignore, pchanges, args ) ) );
  DUF_TRACE( sql, 1, " [[%s]] : %d", fmt, r );
  DEBUG_ENDR( r );
}

static int
duf_sql_vselect( duf_sel_cb_t sel_cb, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata, duf_sccb_handle_t * sccbh,
                 const char *sqlfmt, va_list args )
{
  DEBUG_STARTR( r );

  DOR( r, DUF_SQLITE2R_ERROR_CODE( duf_sqlite_vselect( sel_cb, sel_cb_udata, str_cb, str_cb_udata, sccbh, sqlfmt, args ) ) );
  DEBUG_ENDR( r );
}

/************************************************************************/
/************************************************************************/
/************************************************************************/

static int
duf_sql_exec_c_msg( const char *sql, const char *msg, int constraint_ignore )
{
  DEBUG_STARTR( r );
  DOR( r, duf_sql_exec_c( sql, constraint_ignore, ( int * ) NULL ) );
  DUF_TRACE( sql, 1, "[%-40s] %s (%d)", msg, r != SQLITE_OK ? "FAIL" : "OK", r );
  DEBUG_ENDR( r );
}

int
duf_sql_exec_msg( const char *sql, const char *msg )
{
  DEBUG_STARTR( r );
  DOR( r, duf_sql_exec_c_msg( sql, msg, DUF_CONSTRAINT_IGNORE_NO ) );
  if ( r )
    DUF_SHOW_ERROR( "SQL EXEC ERROR in [%s]", sql );

  DEBUG_ENDR( r );
}

int
duf_sql( const char *fmt, int *pchanges, ... )
{
  DEBUG_STARTR( r );
  va_list args;

  va_start( args, pchanges );
  DOR( r, duf_vsql_c( fmt, DUF_CONSTRAINT_IGNORE_NO, pchanges, args ) );
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
duf_sql_select( duf_sel_cb_t sel_cb, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata, duf_sccb_handle_t * sccbh, const char *sqlfmt, ... )
{
  DEBUG_STARTR( r );
  va_list args;

  va_start( args, sqlfmt );
  DOR( r, duf_sql_vselect( sel_cb, sel_cb_udata, str_cb, str_cb_udata, sccbh, sqlfmt, args ) );
  va_end( args );
  DEBUG_ENDR( r );
}

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

static char *
duf_sql_vmprintf( const char *fmt, va_list args )
{
  char *s = NULL;

  s = mas_sqlite_vmprintf( fmt, args );
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
/*   mas_sqlite_free( s ); */
/* }                       */
