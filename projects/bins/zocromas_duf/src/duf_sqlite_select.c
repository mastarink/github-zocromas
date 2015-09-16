#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"


#include "duf_service.h"

#include "duf_config_ref.h"

#include "duf_sql_error.h"

#include <mastar/sqlite/mas_sqlite.h>

/* ###################################################################### */
/* #include "duf_sqlite.h" */
#include "duf_sqlite_select.h"
/* ###################################################################### */


/*
 *  sel_cb -- callback called for each row with:
 *    sel_cb_udata	     passed
 *    str_cb + str_cb_udata  passed to be called there
 *  fmt + args - sql
 * */
int
duf_sqlite_vselect( duf_sel_cb_t sel_cb, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata,
                    duf_sccb_handle_t * sccbh /*, const duf_dirhandle_t * pdhu_off */ , const char *fmt,
                    va_list args )
{
  int r3 = 0;
  int row, column;
  char *sql, **presult = NULL;

  const char *const *pcresult = NULL;
  va_list qargs;

  DEBUG_START(  );
  assert( mas_sqlite_pdb() );
  va_copy( qargs, args );
  {
    char *emsg = NULL;

    DUF_TRACE( sqlite, 2, "in for %s", fmt );

    sql = sqlite3_vmprintf( fmt, args );
    if ( !sql )
    {
      DUF_SHOW_ERROR( "what happend to sql? [%s] => [%s]", fmt, sql );
    }
    r3 = sqlite3_get_table( mas_sqlite_pdb(), sql, &presult, &row, &column, &emsg );
    assert( r3 != SQLITE_CORRUPT );

    DUF_TRACE( sqlite, 0, "  [%s]", sql );
    DUF_TRACE( sqlite, 1, "r3=%d;  %u rows", r3, row );
    DEBUG_STEPS( sql );
    /* if ( trace )                                            */
    /*   printf(  "(%d) trace:[%s]\x1b[K\n", r3, sql ); */
    pcresult = ( const char *const * ) presult;
    if ( r3 == SQLITE_OK )
    {
      DEBUG_STEPIS( row, "rows SQLITE_OK" );
      if ( row )
      {
        int rcb = 0;

        for ( int ir = 1; ir <= row && rcb == 0; ir++ )
        {
          va_list cargs;
          duf_record_t rrecord;

          va_copy( cargs, qargs );
/* 
 * sel_cb is of duf_sel_cb_t:
 * */

#ifdef DUF_RECORD_WITH_NROWS
          rrecord.nrow = ir - 1;
          rrecord.nrows = row;
#endif
          rrecord.ncolumns = column;
          rrecord.pnames = &pcresult[0 * column];
          rrecord.presult = &pcresult[ir * column];
          {

            rcb = ( sel_cb ) ( &rrecord, sel_cb_udata, str_cb, str_cb_udata, sccbh );

            DUF_TEST_R( rcb );
            DUF_TRACE( sqlite, 2, "row #%u; <sel_cb(%p) = %d", ir, ( void * ) ( unsigned long long ) sel_cb, rcb );
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
            /* DUF_SHOW_ERROR( "rcb:%d", rcb ); */
          }
        }
        DUF_TEST_R( rcb );
        DUF_TEST_R3( r3 );
        DUF_TRACE( sqlite, 4, "rcb:%d; r3:%d sel_cb:%s; str_cb:%s", rcb, r3, DUF_FUNN( sel_cb ), DUF_FUNN( str_cb ) );
        r3 = rcb ? rcb : r3;
        DUF_TEST_R3( r3 );
      }
      else
      {
        DUF_TRACE( sqlite, 0, "Nothing by [%s]", sql );
      }
    }
    else if ( r3 == SQLITE_CONSTRAINT )
    {
      DUF_TEST_R3( r3 );
      DUF_SHOW_ERROR( "SQL : %s [%s]", emsg ? emsg : "no error", sql );
    }
    else
    {
      DUF_TEST_R3( r3 );
      DUF_SHOW_ERROR( "SQL : %s [%s]", emsg ? emsg : "no error", sql );
    }
    if ( emsg )
      sqlite3_free( emsg );
    emsg = NULL;
    sqlite3_free_table( presult );
    presult = NULL;
    sqlite3_free( sql );
    sql = NULL;
  }
  DEBUG_ENDR3( r3 );
  return ( r3 );
}
