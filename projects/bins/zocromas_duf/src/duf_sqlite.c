#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"


#include "duf_service.h"

#include "duf_config_ref.h"

#include "duf_sql_error.h"

/* ###################################################################### */
#include "duf_sqlite.h"
/* ###################################################################### */


/* int duf_constraint = SQLITE_CONSTRAINT; */
static sqlite3 *pDb = NULL;

sqlite3 *
duf_sqlite_pdb(void  )
{
  return pDb;
}

int
duf_sqlite_open( const char *dbpath )
{
  int r3 = 0;

/*										*/ DEBUG_START(  );
  if ( !pDb )
  {
    r3 = sqlite3_initialize(  );
    DUF_TRACE( action, 0, "DB Initialize %s (%d)", r3 == SQLITE_OK ? "OK" : "FAIL", r3 );
    if ( r3 == SQLITE_OK )
    {
      /* r3 = sqlite3_open( dbpath, &pDb ); */
      r3 = sqlite3_open_v2( dbpath, &pDb, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL );
      sqlite3_extended_result_codes( pDb, 1 );
      DUF_TRACE( action, 0, "DB Open %s %s (%d)", dbpath, r3 == SQLITE_OK ? "OK" : "FAIL", r3 );
    }
  }
  /*          */ DEBUG_END(  );
  return ( r3 );
}

int
duf_sqlite_close( void )
{
  int r3 = 0;

/*										*/ DEBUG_START(  );
  if ( pDb )
    r3 = sqlite3_close_v2( pDb );
  pDb = NULL;
  DUF_TRACE( action, 0, "DB Close %s (%d)", r3 == SQLITE_OK ? "OK" : "FAIL", r3 );

  r3 = sqlite3_shutdown(  );
  DUF_TRACE( action, 0, "DB Shutdown %s (%d)", r3 == SQLITE_OK ? "OK" : "FAIL", r3 );

/*										*/ DEBUG_END(  );
  return ( r3 );
}

static int
duf_sqlite_execcb( const char *sql, duf_sqexe_cb_t sqexe_cb, void *sqexe_data, int *pchanges, char **pemsg )
{
  int r3 = SQLITE_OK;
  char *emsg = ( char * ) NULL;

  assert( pDb );
  if ( pemsg )
    *pemsg = NULL;
/*										*/ DEBUG_START(  );
  DUF_TRACE( sqlite, 2, "[%s] ", sql );
  if ( pchanges )
    *pchanges = 0;
  if ( duf_config->cli.dbg.nosqlite )
  {
    DUF_TRACE( sqlite, 0, "SKIP %s", sql );
  }
  else
  {
    r3 = sqlite3_exec( pDb, sql, sqexe_cb, sqexe_data, &emsg );
  }
  if ( r3 == SQLITE_OK && pchanges )
    *pchanges = sqlite3_changes( pDb );
  DUF_TRACE( sqlite, 0, "  [%s]", sql );
  DUF_TRACE( sqlite, 1, "r3:%d; changes:%d", r3, pchanges ? *pchanges : -1 );
/*										*/ DEBUG_STEP(  );
  if ( pemsg )
    *pemsg = emsg;
  else if ( emsg )
    sqlite3_free( emsg );
/*										*/ DEBUG_ENDR3( r3 );
  DUF_TRACE( sqlite, 3, "[%s] : %d", sql, r3 );
  return r3;
}

/* static int                                                                                       */
/* duf_sqlite_execcb_e( const char *sql, duf_sqexe_cb_t sqexe_cb, void *sqexe_data, int *pchanges ) */
/* {                                                                                                */
/*   int r3;                                                                                        */
/*   char *emsg = NULL;                                                                             */
/*                                                                                                  */
/*   r3 = duf_sqlite_execcb( sql, sqexe_cb, sqexe_data, pchanges, &emsg );                          */
/*   if ( r3 != SQLITE_OK )                                                                         */
/*     DUF_SHOW_ERROR( "(%d) SQL '%s' in [%s]", r3, emsg ? emsg : "-", sql ? sql : "?" );           */
/*   if ( emsg )                                                                                    */
/*     sqlite3_free( emsg );                                                                        */
/*   emsg = NULL;                                                                                   */
/*                                                                                                  */
/*   return r3;                                                                                     */
/* }                                                                                                */

static int
duf_sqlite_exec( const char *sql, int *pchanges, char **pemsg )
{
  return duf_sqlite_execcb( sql, ( duf_sqexe_cb_t ) NULL, ( void * ) NULL, pchanges, pemsg );
}

int
duf_sqlite_exec_c( const char *sql, int constraint_ignore, int *pchanges )
{
  int r3;
  char *emsg = NULL;

  r3 = duf_sqlite_exec( sql, pchanges, &emsg );
  if ( r3 != SQLITE_OK && !( r3 == SQLITE_CONSTRAINT && constraint_ignore ) )
    DUF_SHOW_ERROR( "(%d) SQL '%s' in [%s]", r3, emsg ? emsg : "-", sql ? sql : "?" );

  if ( emsg )
    sqlite3_free( emsg );
  emsg = NULL;
  return r3;
}

/* static int                                                                             */
/* duf_sqlite_exec_e( const char *sql, int *pchanges )                                    */
/* {                                                                                      */
/*   int r3;                                                                              */
/*   char *emsg = NULL;                                                                   */
/*                                                                                        */
/*   r3 = duf_sqlite_exec( sql, pchanges, &emsg );                                        */
/*   if ( r3 != SQLITE_OK )                                                               */
/*     DUF_SHOW_ERROR( "(%d) SQL '%s' in [%s]", r3, emsg ? emsg : "-", sql ? sql : "?" ); */
/*   if ( emsg )                                                                          */
/*     sqlite3_free( emsg );                                                              */
/*   emsg = NULL;                                                                         */
/*                                                                                        */
/*   return r3;                                                                           */
/* }                                                                                      */


int
duf_vsqlite_c( const char *fmt, int constraint_ignore, int *pchanges, va_list args )
{
  int r3 = 0;
  char *sql;

  DEBUG_START(  );

  DUF_TRACE( sqlite, 2, "[%s] ", fmt );
  sql = duf_sqlite_vmprintf( fmt, args );
  DUF_TRACE( sqlite, 2, "[%s] ", sql );
  r3 = duf_sqlite_exec_c( sql, constraint_ignore, pchanges );

  DUF_TRACE( sqlite, 3, "[%s] : %d", sql, r3 );
  sqlite3_free( sql );
  sql = NULL;
  DUF_TRACE( sqlite, 3, "r3: %d", r3 );

  DEBUG_ENDR3( r3 );
  return ( r3 );
}

/* static int                                                    */
/* duf_vsqlite_e( const char *fmt, int *pchanges, va_list args ) */
/* {                                                             */
/*   int r3 = 0;                                                 */
/*   char *sql;                                                  */
/*                                                               */
/*   DEBUG_START(  );                                            */
/*                                                               */
/*   DUF_TRACE( sqlite, 2, "[%s] ", fmt );                       */
/*   sql = sqlite3_vmprintf( fmt, args );                        */
/*   DUF_TRACE( sqlite, 2, "[%s] ", sql );                       */
/*   r3 = duf_sqlite_exec_e( sql, pchanges );                    */
/*                                                               */
/*   DUF_TRACE( sqlite, 3, "[%s] : %d", sql, r3 );               */
/*   sqlite3_free( sql );                                        */
/*   sql = NULL;                                                 */
/*   DUF_TRACE( sqlite, 3, "r3: %d", r3 );                       */
/*                                                               */
/*   DEBUG_ENDR3( r3 );                                          */
/*   return ( r3 );                                              */
/* }                                                             */


/* static int                                                                              */
/* duf_vsqlite( const char *fmt, int *pchanges, va_list args )                             */
/* {                                                                                       */
/*   int r3 = 0;                                                                           */
/*   char *sql;                                                                            */
/*   char *emsg = NULL;                                                                    */
/*                                                                                         */
/*   DEBUG_START(  );                                                                      */
/*                                                                                         */
/*   DUF_TRACE( sqlite, 2, "[%s] ", fmt );                                                 */
/*   sql = sqlite3_vmprintf( fmt, args );                                                  */
/*   DUF_TRACE( sqlite, 2, "[%s] ", sql );                                                 */
/*   r3 = duf_sqlite_exec( sql, pchanges, &emsg );                                         */
/*   if ( r3 != SQLITE_OK )                                                                */
/*     DUF_SHOW_ERROR( "(%d) SQL '%s' in [%s]", r3, emsg ? emsg : "-", sql ? sql : "?" );  */
/*                                                                                         */
/*   DUF_TRACE( sqlite, 3, "(%d) SQL '%s' [%s]", r3, emsg ? emsg : "-", sql ? sql : "?" ); */
/*   if ( emsg )                                                                           */
/*     sqlite3_free( emsg );                                                               */
/*                                                                                         */
/*   sqlite3_free( sql );                                                                  */
/*   sql = NULL;                                                                           */
/*   DUF_TRACE( sqlite, 3, "r3: %d", r3 );                                                 */
/*                                                                                         */
/*   DEBUG_ENDR3( r3 );                                                                    */
/*   return ( r3 );                                                                        */
/* }                                                                                       */


unsigned long long
duf_sqlite_last_insert_rowid( void )
{
  unsigned long long li = 0;

  DEBUG_START(  );
  assert( pDb );
  li = ( unsigned long long ) sqlite3_last_insert_rowid( pDb );
  DUF_TRACE( sqlite, 2, "  last_insert_rowid" );
  DEBUG_ENDULL( li );
  return ( li );
}

int
duf_sqlite_prepare( const char *sql, duf_sqlite_stmt_t ** pstmt )
{
  int r3 = 0;
  const char *tail = NULL;

  assert( pDb );
  r3 = sqlite3_prepare_v2( pDb, sql, strlen( sql ), pstmt, &tail );
  DUF_TRACE( sqlite, 2, "  [%s]", sql );
  if ( r3 == SQLITE_ERROR )
  {
    const char *em = sqlite3_errmsg( pDb );

    DUF_SHOW_ERROR( "{%d:%d} can't prepare SQL:[%s] - %s", sqlite3_errcode( pDb ), sqlite3_extended_errcode( pDb ), sql, em );
    if ( sqlite3_strglob( "no such table: *", sqlite3_errmsg( pDb ) ) == 0 )
    {
/* changed DUF_ERROR_SQL_NO_TABLE => duf_r2sqlite_error_code(DUF_ERROR_SQL_NO_TABLE)
 * Not tested */
      r3 = duf_r2sqlite_error_code( DUF_ERROR_SQL_NO_TABLE ); /* FIXME : this is r3, not r; so DUF_ERROR_SQL_NO_TABLE is wrong */
      assert( duf_sqlite2r_error_code( r3 ) == DUF_ERROR_SQL_NO_TABLE );
      assert( 0 );
    }
    else
    {
      assert( 0 );
    }
  }
  /* assert( r3 == 0 ); */
  /* assert( r3 != SQLITE_MISUSE ); */
  DUF_TEST_R3S( r3, sql );
  return r3;
}

/**
#define SQLITE_OK           0   // Successful result
// beginning-of-error-codes //    
#define SQLITE_ERROR        1   // SQL error or missing database
#define SQLITE_INTERNAL     2   // Internal logic error in SQLite
#define SQLITE_PERM         3   // Access permission denied
#define SQLITE_ABORT        4   // Callback routine requested an abort
#define SQLITE_BUSY         5   // The database file is locked
#define SQLITE_LOCKED       6   // A table in the database is locked
#define SQLITE_NOMEM        7   // A malloc() failed
#define SQLITE_READONLY     8   // Attempt to write a readonly database
#define SQLITE_INTERRUPT    9   // Operation terminated by sqlite3_interrupt()
#define SQLITE_IOERR       10   // Some kind of disk I/O error occurred
#define SQLITE_CORRUPT     11   // The database disk image is malformed
#define SQLITE_NOTFOUND    12   // Unknown opcode in sqlite3_file_control()
#define SQLITE_FULL        13   // Insertion failed because database is full
#define SQLITE_CANTOPEN    14   // Unable to open the database file
#define SQLITE_PROTOCOL    15   // Database lock protocol error
#define SQLITE_EMPTY       16   // Database is empty
#define SQLITE_SCHEMA      17   // The database schema changed
#define SQLITE_TOOBIG      18   // String or BLOB exceeds size limit
#define SQLITE_CONSTRAINT  19   // Abort due to constraint violation
#define SQLITE_MISMATCH    20   // Data type mismatch
#define SQLITE_MISUSE      21   // Library used incorrectly
#define SQLITE_NOLFS       22   // Uses OS features not supported on host
#define SQLITE_AUTH        23   // Authorization denied
#define SQLITE_FORMAT      24   // Auxiliary database format error
#define SQLITE_RANGE       25   // 2nd parameter to sqlite3_bind out of range
#define SQLITE_NOTADB      26   // File opened that is not a database file
#define SQLITE_NOTICE      27   // Notifications from sqlite3_log()
#define SQLITE_WARNING     28   // Warnings from sqlite3_log()
#define SQLITE_ROW         100  // sqlite3_step() has another row ready
#define SQLITE_DONE        101  // sqlite3_step() has finished executing
**/
int
duf_sqlite_step( duf_sqlite_stmt_t * stmt )
{
  int r3;

  r3 = sqlite3_step( stmt );
  DUF_TRACE( sqlite, 2, "  step" );
  /* assert( r3 != SQLITE_LOCKED ); */
  DUF_TEST_R3( r3 );
  if ( !( r3 == SQLITE_OK || r3 == SQLITE_DONE || r3 == SQLITE_ROW ) )
  {
    int r = 0;
    const char *DUF_UNUSED t;

    t = duf_error_name( r );
    DUF_TRACE( temp, 0, "%s", duf_error_name( r ) );
  }
  /* assert( r3 != SQLITE_MISUSE );                                      */
  /* assert( r3 == SQLITE_OK || r3 == SQLITE_DONE || r3 == SQLITE_ROW ); */
  return r3;
}

int
duf_sqlite_finalize( duf_sqlite_stmt_t * stmt )
{
  int r3;

  r3 = sqlite3_finalize( stmt );
  DUF_TRACE( sqlite, 2, "  finalize" );
  DUF_TEST_R3( r3 );
  return r3;
}

int
duf_sqlite_reset( duf_sqlite_stmt_t * stmt )
{
  int r3;

  r3 = sqlite3_reset( stmt );
  DUF_TRACE( sqlite, 2, "  reset" );
  DUF_TEST_R3( r3 );
  return r3;
}

int
duf_sqlite_bind_parameter_index( duf_sqlite_stmt_t * stmt, const char *name )
{
  int r3 = 0;

  r3 = sqlite3_bind_parameter_index( stmt, name );
  /* DUF_TRACE( sqlite, 2, "  index of %s is %d", name, r3 ); */
  return r3;
}



int
duf_sqlite_bind_long_long( duf_sqlite_stmt_t * stmt, int num, long long val )
{
  int r3 = 0;
  sqlite3_int64 val64;

  val64 = val;
  DUF_TRACE( sqlite, 2, "  bind ll #%d=%lld [%s]", num, val64, sqlite3_sql( stmt ) );
  r3 = sqlite3_bind_int64( stmt, num, val64 );
  DUF_TEST_R3( r3 );
  return r3;
}

int
duf_sqlite_bind_int( duf_sqlite_stmt_t * stmt, int num, int val )
{
  int r3 = 0;

  DUF_TRACE( sqlite, 2, "  bind i #%d=%d", num, val );
  r3 = sqlite3_bind_int( stmt, num, val );
  DUF_TEST_R3( r3 );
  return r3;
}

int
duf_sqlite_bind_null( duf_sqlite_stmt_t * stmt, int num )
{
  int r3 = 0;

  DUF_TRACE( sqlite, 2, "  bind null #%d", num );
  r3 = sqlite3_bind_null( stmt, num );
  DUF_TEST_R3( r3 );
  return r3;
}

int
duf_sqlite_bind_double( duf_sqlite_stmt_t * stmt, int num, double val )
{
  int r3 = 0;

  DUF_TRACE( sqlite, 2, "  bind double #%d=%g", num, val );
  r3 = sqlite3_bind_double( stmt, num, val );
  DUF_TEST_R3( r3 );
  return r3;
}

int
duf_sqlite_bind_string( duf_sqlite_stmt_t * stmt, int num, const char *val )
{
  int r3 = 0;

  DUF_TRACE( sqlite, 2, "  bind string #%d=%s", num, val );
  r3 = sqlite3_bind_text( stmt, num, val, strlen( val ), SQLITE_TRANSIENT );
  DUF_TEST_R3( r3 );
  return r3;
}

int
duf_sqlite_changes( void )
{
  int changes = 0;

  changes = sqlite3_changes( pDb );
  DUF_TRACE( sqlite, 2, "  changes=%d", changes );
  return changes;
}

long long
duf_sqlite_column_long_long( duf_sqlite_stmt_t * stmt, int icol )
{
  return sqlite3_column_int64( stmt, icol );
}

int
duf_sqlite_column_int( duf_sqlite_stmt_t * stmt, int icol )
{
  return sqlite3_column_int( stmt, icol );
}

const char *
duf_sqlite_column_string( duf_sqlite_stmt_t * stmt, int icol )
{
  return ( const char * ) sqlite3_column_text( stmt, icol );
}

const char *
duf_sqlite_column_name( duf_sqlite_stmt_t * stmt, int index )
{
  const char *str;

  str = sqlite3_column_name( stmt, index );
  return str;
}

int
duf_sqlite_column_count( duf_sqlite_stmt_t * stmt )
{
  int cnt;

  cnt = sqlite3_column_count( stmt );
  return cnt;
}

char *
duf_sqlite_vmprintf( const char *fmt, va_list args )
{
  char *s = NULL;

  s = sqlite3_vmprintf( fmt, args );
  return s;
}

/* static char *                              */
/* duf_sqlite_mprintf( const char *fmt, ... ) */
/* {                                          */
/*   char *s = NULL;                          */
/*   va_list args;                            */
/*                                            */
/*   va_start( args, fmt );                   */
/*   s = sqlite3_vmprintf( fmt, args );       */
/*   va_end( args );                          */
/*   return s;                                */
/* }                                          */

/* static void                */
/* duf_sqlite_free( char *s ) */
/* {                          */
/*   sqlite3_free( s );       */
/* }                          */

void
duf_sqlite_clear_bindings( duf_sqlite_stmt_t * stmt )
{
  sqlite3_clear_bindings( stmt );
}
