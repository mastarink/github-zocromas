#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <sqlite3.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_config.h"


/* ###################################################################### */
#include "duf_sql.h"
/* ###################################################################### */


int duf_constraint = SQLITE_CONSTRAINT;
sqlite3 *pDb;

int
duf_sql_open( const char *dbpath )
{
  int r = 0;

  r = sqlite3_initialize(  );
  if ( r != SQLITE_OK )
    fprintf( stderr, "SQL DB Initialize ERROR\n" );
  else
  {
    if ( duf_config->verbose )
      fprintf( stderr, "SQL DB Initialize OK\n" );

    r = sqlite3_open( dbpath, &pDb );
    if ( r != SQLITE_OK )
      fprintf( stderr, "SQL DB Open ERROR\n" );
    else if ( duf_config->verbose )
      fprintf( stderr, "SQL DB Open OK\n" );
  }
  return r;
}

int
duf_sql_close( void )
{
  int r = 0;

  r = sqlite3_close( pDb );
  if ( r != SQLITE_OK )
    fprintf( stderr, "SQL DB Close ERROR\n" );
  else if ( duf_config->verbose )
    fprintf( stderr, "SQL DB Close OK\x1b[K\n" );

  r = sqlite3_shutdown(  );
  if ( r != SQLITE_OK )
    fprintf( stderr, "SQL DB Shutdown ERROR\n" );
  else if ( duf_config->verbose )
    fprintf( stderr, "SQL DB Shutdown OK\x1b[K\n" );

  return 0;
}

int
duf_sql_exec_c( const char *sql, int constraint_ignore )
{
  int r;
  char *emsg = NULL;

  r = sqlite3_exec( pDb, sql, NULL, NULL, &emsg );
  if ( r == SQLITE_CONSTRAINT )
  {
    if ( !constraint_ignore )
      fprintf( stderr, "SQL CONSTRAINT '%s' [%s]\n", emsg ? emsg : "-", sql ? sql : "?" );
  }
  else if ( r != SQLITE_OK )
    fprintf( stderr, "SQL ERROR '%s' [%s]\n", emsg ? emsg : "-", sql ? sql : "?" );
  /* else                                  */
  /*   fprintf( stderr, "SQL exec OK\x1b[K\n" ); */
  return r;
}

int
duf_sql_exec( const char *sql )
{
  return duf_sql_exec_c( sql, DUF_CONSTRAINT_IGNORE_NO );
}

int
duf_sql_exec_c_msg( const char *sql, const char *msg, int constraint_ignore )
{
  int r;

  r = duf_sql_exec_c( sql, constraint_ignore );
  if ( r == SQLITE_OK )
    fprintf( stderr, "SQL %s OK\x1b[K\n", msg );
  return r;
}

int
duf_sql_exec_msg( const char *sql, const char *msg )
{
  return duf_sql_exec_c_msg( sql, msg, DUF_CONSTRAINT_IGNORE_NO );
}

int
duf_sql_c( const char *fmt, int constraint_ignore, ... )
{
  int r = 0;
  va_list args;
  char *sql;

  va_start( args, constraint_ignore );
  {
    sql = sqlite3_vmprintf( fmt, args );
    r = duf_sql_exec_c( sql, constraint_ignore );
    sqlite3_free( sql );
  }
  va_end( args );
  return r;
}

int
duf_sql( const char *fmt, ... )
{
  int r = 0;
  va_list args;
  char *sql;

  va_start( args, fmt );
  {
    sql = sqlite3_vmprintf( fmt, args );
    r = duf_sql_exec( sql );
    sqlite3_free( sql );
  }
  va_end( args );
  return r;
}

int
duf_sql_vselect( duf_sql_select_cb_t sel_cb, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata, int trace, const char *fmt,
                 va_list args )
{
  int r = 0;
  int row, column;
  char *sql, **presult = NULL;
  const char *const *pcresult;
  char *emsg = NULL;

  va_list qargs;

  va_copy( qargs, args );

  sql = sqlite3_vmprintf( fmt, args );
  if ( trace )
    fprintf( stderr, "trace:[%s]\n", sql );
  r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &emsg );
  pcresult = ( const char *const * ) presult;
  if ( r == SQLITE_OK )
  {
    if ( row )
      for ( int ir = 1; ir <= row; ir++ )
      {
        va_list cargs;

        va_copy( cargs, qargs );
/* 
 * sql must select pathid, filenameid, filename, md5id, size
 * sel_cb is duf_sql_select_cb_t:
 *             int fun( int nrow, int nrows, const char *const *presult, va_list args, void *sel_cb_udata, duf_str_cb_t str_cb );
 * */
        ( sel_cb ) ( ir - 1, row, &pcresult[ir * column], cargs, sel_cb_udata, str_cb, str_cb_udata );
      }
  }
  else if ( r == SQLITE_CONSTRAINT )
  {
    fprintf( stderr, "SQL CONSTRAINT\n" );
  }
  else
    fprintf( stderr, "SQL error %d %s\n", r, emsg );
  sqlite3_free_table( presult );
  sqlite3_free( sql );
  return r;
}

int
duf_sql_select( duf_sql_select_cb_t sel_cb, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata, int trace, const char *fmt, ... )
{
  va_list args;
  int r;

  va_start( args, fmt );
  r = duf_sql_vselect( sel_cb, sel_cb_udata, str_cb, str_cb_udata, trace, fmt, args );
  va_end( args );
  return r;
}

unsigned long long
duf_last_insert_rowid( void )
{
  return ( unsigned long long ) sqlite3_last_insert_rowid( pDb );
}
