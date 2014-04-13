#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <sqlite3.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_dbg.h"

#include "duf_config.h"

#include "duf_file_scan.h"
#include "duf_uni_scan.h"
#include "duf_path.h"


/* ###################################################################### */
#include "duf_sql.h"
/* ###################################################################### */


int duf_constraint = SQLITE_CONSTRAINT;
sqlite3 *pDb;

int
duf_sql_pos_by_name( const char *name, duf_record_t * precord, int optional )
{
  int pos = -1;

  for ( int i = 0; i < precord->ncolumns; i++ )
  {
    if ( 0 == strcmp( name, precord->pnames[i] ) )
    {
      pos = i;
      break;
    }
  }
  if ( !optional && pos < 0 )
  {
    for ( int i = 0; i < precord->ncolumns; i++ )
    {
      fprintf( stderr, "  Have '%s'\n", precord->pnames[i] );
    }
    fprintf( stderr, "Error - no such field '%s'\n", name );
    exit( 4 );
  }
  return pos;
}

const char *
duf_sql_str_by_name( const char *name, duf_record_t * precord, int optional )
{
  const char *ptr = NULL;
  int pos = duf_sql_pos_by_name( name, precord, optional );

  if ( pos >= 0 )
    ptr = precord->presult[pos];

  return ptr;
}

const char *
duf_sql_val_by_name( const char *name, duf_record_t * precord, int optional )
{
  const char *ptr = NULL;
  int pos = duf_sql_pos_by_name( name, precord, optional );

  if ( pos >= 0 )
    ptr = precord->presult[pos];
  return ptr;
}

unsigned long long
duf_sql_ull_by_name( const char *name, duf_record_t * precord, int optional )
{
  const char *ptr = NULL;

  ptr = duf_sql_val_by_name( name, precord, optional );
  return ptr ? strtoll( ptr, NULL, 10 ) : 0;
}

int
duf_sql_open( const char *dbpath )
{
  int r = 0;

/*										*/ duf_dbgfunc( DBG_START, __func__, __LINE__ );
  r = sqlite3_initialize(  );
  if ( r != SQLITE_OK )
    fprintf( stderr, "SQL DB Initialize ERROR\n" );
  else
  {
    if ( duf_config->cli.trace.sql )
      fprintf( stderr, "[SQL  ] %20s: DB Initialize OK\n", __func__ );

    r = sqlite3_open( dbpath, &pDb );
    if ( r != SQLITE_OK )
      fprintf( stderr, "SQL DB Open ERROR\n" );
    else if ( duf_config->cli.trace.sql )
      fprintf( stderr, "[SQL  ] %20s: DB Open OK\n", __func__ );
  }
/*										*/ duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}

int
duf_sql_close( void )
{
  int r = 0;

/*										*/ duf_dbgfunc( DBG_START, __func__, __LINE__ );
  r = sqlite3_close( pDb );
  if ( r != SQLITE_OK )
    fprintf( stderr, "SQL DB Close ERROR\n" );
  else if ( duf_config->cli.trace.sql )
    fprintf( stderr, "[SQL  ] %20s: DB Close OK\n", __func__ );

  r = sqlite3_shutdown(  );
  if ( r != SQLITE_OK )
    fprintf( stderr, "SQL DB Shutdown ERROR\n" );
  else if ( duf_config->cli.trace.sql )
    fprintf( stderr, "[SQL  ] %20s: DB Shutdown OK\n", __func__ );

/*										*/ duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

int
duf_sql_exec_c( const char *sql, int constraint_ignore )
{
  int r;
  char *emsg = ( char * ) NULL;

/*										*/ duf_dbgfunc( DBG_START, __func__, __LINE__ );
  r = sqlite3_exec( pDb, sql, NULL, NULL, &emsg );
/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( r == SQLITE_CONSTRAINT )
  {
    if ( !constraint_ignore )
      fprintf( stderr, "SQL CONSTRAINT '%s' [%s]\n", emsg ? emsg : "-", sql ? sql : "?" );
  }
  else if ( r != SQLITE_OK )
    fprintf( stderr, "SQL ERROR '%s' [%s]\n", emsg ? emsg : "-", sql ? sql : "?" );
  /* else                                  */
  /*   fprintf( stderr, "SQL exec OK\x1b[K\n" ); */
/*										*/ duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}

int
duf_sql_exec( const char *sql )
{
  int r = 0;

/*										*/ duf_dbgfunc( DBG_START, __func__, __LINE__ );
  r = duf_sql_exec_c( sql, DUF_CONSTRAINT_IGNORE_NO );
/*										*/ duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}

int
duf_sql_exec_c_msg( const char *sql, const char *msg, int constraint_ignore )
{
  int r;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  r = duf_sql_exec_c( sql, constraint_ignore );
  if ( r == SQLITE_OK && duf_config->cli.dbg.verbose )
    fprintf( stderr, "SQL %s OK\x1b[K\n", msg );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

int
duf_sql_exec_msg( const char *sql, const char *msg )
{
  int r;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  r = duf_sql_exec_c_msg( sql, msg, DUF_CONSTRAINT_IGNORE_NO );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

int
duf_vsql_c( const char *fmt, int constraint_ignore, va_list args )
{
  int r = 0;
  char *sql;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  sql = sqlite3_vmprintf( fmt, args );
  r = duf_sql_exec_c( sql, constraint_ignore );
  if ( duf_config->cli.trace.sql > 1 )
    fprintf( stderr, "[SQL  ] %20s: %s : %d\n", __func__, sql, r );
  sqlite3_free( sql );
  duf_dbgfunc( DBG_ENDRS, __func__, __LINE__, r, sql );
  return r;
}

int
duf_sql_c( const char *fmt, int constraint_ignore, ... )
{
  int r = 0;
  va_list args;

  /* char *sql; */

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  va_start( args, constraint_ignore );
  r = duf_vsql_c( fmt, constraint_ignore, args );
  /* {                                               */
  /*   sql = sqlite3_vmprintf( fmt, args );          */
  /*   r = duf_sql_exec_c( sql, constraint_ignore ); */
  /*   sqlite3_free( sql );                          */
  /* }                                               */
  va_end( args );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

int
duf_sql( const char *fmt, ... )
{
  int r = 0;
  va_list args;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  va_start( args, fmt );
  r = duf_vsql_c( fmt, DUF_CONSTRAINT_IGNORE_NO, args );
  /* {                                                 */
  /*   sql = sqlite3_vmprintf( fmt, args );            */
  /*   if ( trace )                                    */
  /*     fprintf( stderr, "trace:[%s]\x1b[K\n", sql ); */
  /*   r = duf_sql_exec( sql );                        */
  /*   sqlite3_free( sql );                            */
  /* }                                                 */
  va_end( args );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

/*
 *  sel_cb -- callback called for each row with:
 *    sel_cb_udata	     passed
 *    str_cb + str_cb_udata  passed to be called there
 *  fmt + args - sql
 * */
int
duf_sql_vselect( duf_sql_select_cb_t sel_cb, void *sel_cb_udata, duf_scan_callback_file_t str_cb, void *str_cb_udata,
                 duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb, const char *fmt, va_list args )
{
  int r = 0;
  int row, column;
  char *sql, **presult = NULL;

  const char *const *pcresult;
  char *emsg = NULL;

  va_list qargs;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  va_copy( qargs, args );

  sql = sqlite3_vmprintf( fmt, args );
  r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &emsg );
  if ( duf_config->cli.trace.sql > 1 )
    fprintf( stderr, "[SQL  ] %20s: %s = %d; %u rows\n", __func__, sql, r, row );
  else if ( duf_config->cli.trace.sql )
    fprintf( stderr, "[SQL  ] %20s:\n", __func__ );
  duf_dbgfunc( DBG_STEPS, __func__, __LINE__, sql );
  /* if ( trace )                                            */
  /*   fprintf( stderr, "(%d) trace:[%s]\x1b[K\n", r, sql ); */
  pcresult = ( const char *const * ) presult;
  if ( r == SQLITE_OK )
  {
    duf_dbgfunc( DBG_STEPIS, __func__, __LINE__, row, "rows SQLITE_OK" );
    if ( row )
    {
      for ( int ir = 1; ir <= row; ir++ )
      {
        va_list cargs;

        va_copy( cargs, qargs );
/* 
 * sql must select pathid, filenameid, filename, md5id, size
 * sel_cb is duf_sql_select_cb_t:
 *             int fun( int nrow, int nrows, const char *const *presult, va_list args, void *sel_cb_udata, duf_scan_callback_file_t str_cb );
 * */
        if ( duf_config->cli.dbg.verbose > 1 )
        {
          const char *name = NULL;
          const char *name2 = NULL;

          if ( sel_cb == duf_sql_path_to_pathid )
            name = "duf_sql_path_to_pathid";
          if ( str_cb == duf_uni_scan_dir )
            name2 = "duf_uni_scan_dir";

          fprintf( stderr, "call sel_cb> %p:%s; pass %p:%s\n", ( void * ) ( unsigned long long ) sel_cb, name ? name : "-",
                   ( void * ) ( unsigned long long ) str_cb, name2 ? name2 : "-" );
        }
        duf_record_t rrecord;

        rrecord.nrow = ir - 1;
        rrecord.nrows = row;
        rrecord.ncolumns = column;
        rrecord.pnames = &pcresult[0 * column];
        rrecord.presult = &pcresult[ir * column];
        {
          int rcb = ( sel_cb ) ( &rrecord, cargs, sel_cb_udata, str_cb, str_cb_udata, pdi, sccb );

          if ( duf_config->cli.trace.sql )
            fprintf( stderr, "[SQL  ] %20s: row %u; <sel_cb(%p) = %d\n", __func__, ir, ( void * ) ( unsigned long long ) sel_cb, rcb );
        }
      }
    }
    else
    {
      if ( duf_config->cli.trace.sql )
        fprintf( stderr, "[SQL  ] %20s: Nothing by '%s'\n", __func__, sql );
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
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

/*
 *  sel_cb -- callback called for each row with:
 *    sel_cb_udata	     passed
 *    str_cb + str_cb_udata  passed to be called there
 *  sqlfmt + ... - sql
 * */
int
duf_sql_select( duf_sql_select_cb_t sel_cb, void *sel_cb_udata, duf_scan_callback_file_t str_cb, void *str_cb_udata,
                duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb, const char *sqlfmt, ... )
{
  va_list args;
  int r;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  va_start( args, sqlfmt );
  r = duf_sql_vselect( sel_cb, sel_cb_udata, str_cb, str_cb_udata, pdi, sccb, sqlfmt, args );
  va_end( args );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

unsigned long long
duf_last_insert_rowid( void )
{
  unsigned long long r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  r = ( unsigned long long ) sqlite3_last_insert_rowid( pDb );
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, r );
  return r;
}
