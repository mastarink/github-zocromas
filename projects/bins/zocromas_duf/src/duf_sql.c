#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"
#include "duf_utils.h"
#include "duf_service.h"

#include "duf_dbg.h"

#include "duf_config.h"

#include "duf_uni_scan.h"
#include "duf_path.h"

#include "duf_sqlite.h"

/* ###################################################################### */
#include "duf_sql.h"
/* ###################################################################### */



int
duf_sql_open( const char *dbpath )
{
  return duf_sqlite_open( dbpath );
}

int
duf_sql_close( void )
{
  return duf_sqlite_close(  );
}

int
duf_sql_exec_c( const char *sql, int constraint_ignore, int *pchanges )
{
  return duf_sqlite_error_code( duf_sqlite_exec_c( sql, constraint_ignore, pchanges ) );
}

int
duf_sql_exec( const char *sql, int *pchanges )
{
  return duf_sqlite_error_code( duf_sqlite_exec_c( sql, DUF_CONSTRAINT_IGNORE_NO, pchanges ) );
}

static int
duf_vsql_c( const char *fmt, int constraint_ignore, int *pchanges, va_list args )
{
  int r = 0;


  DUF_TRACE( sql, 1, "[%s]", fmt );
  r = duf_sqlite_error_code( duf_vsqlite_c( fmt, constraint_ignore, pchanges, args ) );
  DUF_TRACE( sql, 1, "[%s] : %d", fmt, r );
  return r;
}

int
duf_sql_vselect( duf_sql_select_cb_t sel_cb, void *sel_cb_udata, duf_scan_callback_file_t str_cb, void *str_cb_udata,
                 duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, const duf_dirhandle_t * pdhu, const char *sqlfmt, va_list args )
{
  int r;

  r = duf_sqlite_error_code( duf_sqlite_vselect( sel_cb, sel_cb_udata, str_cb, str_cb_udata, pdi, sccb, pdhu, sqlfmt, args ) );
      DUF_TEST_R( r );
  return r;
}

unsigned long long
duf_sql_last_insert_rowid( void )
{
  return duf_sqlite_last_insert_rowid(  );
}

/************************************************************************/
/************************************************************************/
/************************************************************************/

int
duf_sql_exec_c_msg( const char *sql, const char *msg, int constraint_ignore )
{
  int r;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  r = duf_sql_exec_c( sql, constraint_ignore, ( int * ) NULL );
  DUF_TRACE( sql, 1, "[%-40s] %s (%d)", msg, r != SQLITE_OK ? "FAIL" : "OK", r );

  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

int
duf_sql_exec_msg( const char *sql, const char *msg )
{
  int r;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  r = duf_sql_exec_c_msg( sql, msg, DUF_CONSTRAINT_IGNORE_NO );
  if ( r )
    DUF_ERROR( "SQL EXEC ERROR in [%s]", sql );

  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}


int
duf_sql_c( const char *fmt, int constraint_ignore, int *pchanges, ... )
{
  int r = 0;
  va_list args;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  va_start( args, pchanges );
  r = duf_vsql_c( fmt, constraint_ignore, pchanges, args );
  DUF_TRACE( sql, 1, "[%s] : %d", fmt, r );
  va_end( args );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return ( r );
}

int
duf_sql( const char *fmt, int *pchanges, ... )
{
  int r = 0;
  va_list args;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  va_start( args, pchanges );
  r = duf_vsql_c( fmt, DUF_CONSTRAINT_IGNORE_NO, pchanges, args );
  DUF_TRACE( sql, 1, "[%s] : %d", fmt, r );
  va_end( args );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return ( r );
}



/*
 *  sel_cb -- callback called for each row with:
 *    sel_cb_udata	     passed
 *    str_cb + str_cb_udata  passed to be called there
 *  sqlfmt + ... - sql
 * */
int
duf_sql_select( duf_sql_select_cb_t sel_cb, void *sel_cb_udata, duf_scan_callback_file_t str_cb, void *str_cb_udata,
                duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, const duf_dirhandle_t * pdhu, const char *sqlfmt, ... )
{
  va_list args;
  int r;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  va_start( args, sqlfmt );
  r = duf_sql_vselect( sel_cb, sel_cb_udata, str_cb, str_cb_udata, pdi, sccb, pdhu, sqlfmt, args );
  va_end( args );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return ( r );
}
