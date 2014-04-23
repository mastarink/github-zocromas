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

#include "duf_sql_def.h"

/* ###################################################################### */
#include "duf_sqlite.h"
/* ###################################################################### */


/* int duf_constraint = SQLITE_CONSTRAINT; */
static sqlite3 *pDb = NULL;


int
duf_sqlite_error_code( int r3 )
{
  int r;

  r = ( r3 == SQLITE_OK ? 0 : ( r3 > 0 ? DUF_SQLITE_ERROR_BASE + r3 : r3 ) );
  /* if ( r3 && r3 != SQLITE_CONSTRAINT (* && r != DUF_ERROR_NO_FIELD_OPTIONAL *)  && r != DUF_ERROR_MAX_REACHED ) */
  /*   DUF_ERROR( "CODE %d => %d", r3, r );                                                                        */
  return r;
}


int
duf_sqlite_open( const char *dbpath )
{
  int r3 = 0;

/*										*/ duf_dbgfunc( DBG_START, __func__, __LINE__ );
  r3 = sqlite3_initialize(  );
  DUF_TRACE( action, 0, "DB Initialize %s (%d)", r3 == SQLITE_OK ? "OK" : "FAIL", r3 );
  if ( r3 == SQLITE_OK )
  {
    r3 = sqlite3_open( dbpath, &pDb );
    DUF_TRACE( action, 0, "DB Open %s %s (%d)", dbpath, r3 == SQLITE_OK ? "OK" : "FAIL", r3 );
  }
  /*          */ duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return ( r3 );
}

int
duf_sqlite_close( void )
{
  int r3 = 0;

/*										*/ duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( pDb )
    r3 = sqlite3_close( pDb );
  DUF_TRACE( action, 0, "DB Close %s (%d)", r3 == SQLITE_OK ? "OK" : "FAIL", r3 );

  r3 = sqlite3_shutdown(  );
  pDb = NULL;
  DUF_TRACE( action, 0, "DB Shutdown %s (%d)", r3 == SQLITE_OK ? "OK" : "FAIL", r3 );

/*										*/ duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return ( r3 );
}

int
duf_sqlite_exec_c( const char *sql, int constraint_ignore, int *pchanges )
{
  int r3 = SQLITE_OK;
  char *emsg = ( char * ) NULL;

/*										*/ duf_dbgfunc( DBG_START, __func__, __LINE__ );
  DUF_TRACE( sql, 2, "[%s] ", sql );
  if ( pchanges )
    *pchanges = 0;
  if ( pDb )
  {
    if ( duf_config->cli.dbg.nosqlite )
    {
      DUF_TRACE( current, 0, "SKIP %s", sql );
    }
    else
    {
      r3 = sqlite3_exec( pDb, sql, NULL, NULL, &emsg );
      DUF_TRACE( sql, 0, "[%s]   r3:%d", sql, r3 );
    }
    if ( r3 == SQLITE_OK && pchanges )
      *pchanges = sqlite3_changes( pDb );

/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
    if ( r3 == SQLITE_CONSTRAINT )
    {
      if ( !constraint_ignore )
        DUF_ERROR( "SQL CONSTRAINT '%s' [%s]", emsg ? emsg : "-", sql ? sql : "?" );
    }
    else if ( r3 != SQLITE_OK )
      DUF_ERROR( "(%d) SQL '%s' in [%s]", r3, emsg ? emsg : "-", sql ? sql : "?" );
    /* else                                  */
    /*   fprintf( stderr, "SQL exec OK\x1b[K\n" ); */
  }
  else
  {
  }
  if ( emsg )
    sqlite3_free( emsg );
/*										*/ duf_dbgfunc( DBG_END, __func__, __LINE__ );
  DUF_TRACE( sql, 3, "[%s] : %d", sql, r3 );
  return ( r3 );
}

int
duf_vsqlite_c( const char *fmt, int constraint_ignore, int *pchanges, va_list args )
{
  int r3 = 0;
  char *sql;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  DUF_TRACE( sql, 2, "[%s] ", fmt );
  sql = sqlite3_vmprintf( fmt, args );
  DUF_TRACE( sql, 2, "[%s] ", sql );
  r3 = duf_sqlite_exec_c( sql, constraint_ignore, pchanges );


  DUF_TRACE( sql, 3, "[%s] : %d", sql, r3 );
  sqlite3_free( sql );
  sql = NULL;
  DUF_TRACE( sql, 3, "r3: %d", r3 );

  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r3 );
  return ( r3 );
}



/*
 *  sel_cb -- callback called for each row with:
 *    sel_cb_udata	     passed
 *    str_cb + str_cb_udata  passed to be called there
 *  fmt + args - sql
 * */
int
duf_sqlite_vselect( duf_sql_select_cb_t sel_cb, void *sel_cb_udata, duf_scan_callback_file_t str_cb, void *str_cb_udata,
                    duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, const duf_dirhandle_t * pdhu, const char *fmt, va_list args )
{
  int r3 = 0;
  int row, column;
  char *sql, **presult = NULL;

  const char *const *pcresult = NULL;

  va_list qargs;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  va_copy( qargs, args );
  if ( pDb )
  {
    char *emsg = NULL;

    DUF_TRACE( sql, 2, "in for %s", fmt );

    sql = sqlite3_vmprintf( fmt, args );
    r3 = sqlite3_get_table( pDb, sql, &presult, &row, &column, &emsg );

    DUF_TRACE( sql, 0, "[%s] r3=%d;  %u rows", sql, r3, row );

    duf_dbgfunc( DBG_STEPS, __func__, __LINE__, sql );
    /* if ( trace )                                            */
    /*   printf(  "(%d) trace:[%s]\x1b[K\n", r3, sql ); */
    pcresult = ( const char *const * ) presult;
    if ( r3 == SQLITE_OK )
    {
      duf_dbgfunc( DBG_STEPIS, __func__, __LINE__, row, "rows SQLITE_OK" );
      if ( row )
      {
        int rcb = 0;

        for ( int ir = 1; ir <= row && rcb == 0; ir++ )
        {
          va_list cargs;

          va_copy( cargs, qargs );
/* 
 * sel_cb is duf_sql_select_cb_t:
 *             int fun( int nrow, int nrows, const char *const *presult, va_list args, void *sel_cb_udata, duf_scan_callback_file_t str_cb );
 * */
          if ( duf_config->cli.dbg.verbose > 1 )
          {
            const char *name = NULL;
            const char *name2 = NULL;

            /* if ( sel_cb == duf_sel_cb_field_by_sccb ) */
            /*   name = "duf_sel_cb_field_by_sccb";      */
            /* if ( str_cb == duf_uni_scan_dir )         */
            /*   name2 = "duf_uni_scan_dir";             */

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
            rcb = ( sel_cb ) ( &rrecord, cargs, sel_cb_udata, str_cb, str_cb_udata, pdi, sccb, pdhu );

            DUF_TEST_R( rcb );
            DUF_TRACE( sql, 1, "row %u; <sel_cb(%p) = %d", ir, ( void * ) ( unsigned long long ) sel_cb, rcb );
          }
        }
        if ( rcb )
        {
          if ( rcb == DUF_ERROR_MAX_REACHED )
          {
            /* DUF_TRACE( action, 0, "Maximum reached" ); */
          }
          else
          {
            /* DUF_ERROR( "rcb:%d", rcb ); */
          }
        }
        DUF_TEST_R( rcb );
        DUF_TEST_R( duf_sqlite_error_code( r3 ) );
        DUF_TRACE( fill, 0, "rcb:%d; r3:%d sel_cb:%s; str_cb:%s", rcb, r3, DUF_FUNN( sel_cb ), DUF_FUNN( str_cb ) );
        r3 = rcb ? rcb : r3;
        DUF_TEST_R( duf_sqlite_error_code( r3 ) );
      }
      else
      {
        DUF_TRACE( sql, 0, "Nothing by [%s]", sql );
      }
    }
    else if ( r3 == SQLITE_CONSTRAINT )
    {
      fprintf( stderr, "SQL CONSTRAINT\n" );
      DUF_TEST_R( duf_sqlite_error_code( r3 ) );
    }
    else
      fprintf( stderr, "SQL error %d %s; sql:[%s]\n", r3, emsg, sql );
    if ( emsg )
      sqlite3_free( emsg );
    sqlite3_free_table( presult );
    sqlite3_free( sql );
    /* DUF_ERROR( "r3:%d", r3 ); */
  }
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r3 );
  DUF_TEST_R( duf_sqlite_error_code( r3 ) );
  return ( r3 );
}



unsigned long long
duf_sqlite_last_insert_rowid( void )
{
  unsigned long long li = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( pDb )
  {
    li = ( unsigned long long ) sqlite3_last_insert_rowid( pDb );
  }
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, li );
  return ( li );
}
