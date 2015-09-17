#define DUF_SQL_PDI_STMT

#include <assert.h>
#include <string.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_pdi_credel.h"
#include "duf_pdi_reinit.h"
#include "duf_pdi_ref.h"

#include "duf_levinfo_ref.h"

#include "duf_sql_stmt_defs.h"
#include "duf_sql_prepared.h"
#include "duf_sql_defs.h"
#include "duf_sql_field.h"
/* #include "duf_sql2.h" */

#include "duf_sccb_def.h"
#include "duf_sccb.h"
#include "evsql_selector.h"

#include "duf_sccb_begfin.h"
#include "duf_ufilter_bind.h"


#include "duf_sccbh_shortcuts.h"
/* ###################################################################### */
#include "duf_sccb_handle.h"
/* ###################################################################### */

static unsigned long long
duf_count_total_items( const duf_sccb_handle_t * sccbh, int *pr )
{
  DEBUG_STARTULL( cnt );
  int rpr = DUF_ERROR_TOTALS;

  /* const char *leaf_selector_total2 = NULL; */

  assert( SCCB );
  /* leaf_selector_total2 = duf_get_leaf_sql_set( SCCB )->selector_total2; */

  if ( SCCB )
  {
    char *sqlt = NULL;
    const duf_sql_set_t *sql_set = NULL;

    rpr = 0;
#if 0
    sqlt = mas_strdup( "SELECT " );
    sqlt = mas_strcat_x( sqlt, "COUNT(*) AS nf" );
    sqlt = mas_strcat_x( sqlt, " " );
    sqlt = mas_strcat_x( sqlt, leaf_selector_total2 );
#else
    if ( SCCB->count_nodes )
      sql_set = duf_sccb_get_sql_set( SCCB, DUF_NODE_NODE );
    else
      sql_set = duf_sccb_get_sql_set( SCCB, DUF_NODE_LEAF );
    sqlt = duf_selector_total2sql( sql_set, PDI->pdi_name, &rpr );
#endif
    if ( DUF_NOERROR( rpr ) && sqlt )
    {
      const char *csql;

/*
 * TODO: counting by 'dry' pass, by db is bad
 * for instance: selected_paths does NOT contain "fileless" directories
 */
      csql = sqlt;
      DUF_TRACE( temp, 5, "count by %s", csql );
      DUF_SQL_START_STMT_NOPDI( csql, rpr, pstmt );
      duf_bind_ufilter_uni( pstmt, PU, NULL );
      DUF_SQL_STEP( rpr, pstmt );
      if ( rpr == MAS_SQL_ROW )
      {
#if 1
        cnt = duf_sql_column_long_long( pstmt, 0 );
#else
        cnt = DUF_GET_UFIELD2( CNT );
#endif
        rpr = 0;
      }
      DUF_SQL_END_STMT_NOPDI( rpr, pstmt );
      DUF_TRACE( temp, 5, "counted %llu SIZED files in db", cnt );
      DUF_TRACE( explain, 0, "counted %llu SIZED files in db", cnt );
    }
    mas_free( sqlt );
  }
  else
  {
    DUF_TRACE( explain, 0, "didn't count files in db" );
  }
  if ( pr )
    *pr = rpr;
  DEBUG_ENDULL( cnt );
}

static int
duf_sccbh_eval_sql_sequence( const duf_sccb_handle_t * sccbh /*, const duf_ufilter_t * pu_unused */  )
{
  DEBUG_STARTR( r );
  if ( !duf_pdi_root( PDI )->sql_beginning_done )
  {
    DOR( r, duf_sccb_eval_sql_sequence( SCCB, PU, PDI->pdi_name ) );
    if ( DUF_NOERROR( r ) )
    {
      duf_pdi_root( PDI )->sql_beginning_done = 1;
    }
  }
  DEBUG_ENDR( r );
}

duf_sccb_handle_t *
duf_sccb_handle_create( void )
{
  duf_sccb_handle_t *sccbh = NULL;

  sccbh = mas_malloc( sizeof( duf_sccb_handle_t ) );
  memset( sccbh, 0, sizeof( duf_sccb_handle_t ) );
  return sccbh;
}

/*
 * create & open duf_sccb_handle_t from pdi & sccb (+targ)
 * */
duf_sccb_handle_t *
duf_sccb_handle_open( duf_depthinfo_t * pdi, const duf_scan_callbacks_t * sccb, int targc, char *const *targv /*, const duf_ufilter_t * pu_unused */ ,
                      int *pr )
{
  duf_sccb_handle_t *sccbh = NULL;
  int rpr = 0;

  if ( sccb )
  {
    DUF_TRACE( fs, 2, "set def. opendir: %d", sccb->def_opendir );
    duf_pdi_set_opendir( pdi, sccb->def_opendir );

    sccbh = duf_sccb_handle_create(  );
    PARGC = targc;
    PARGV = targv;
    SCCB = sccb;
#if 0
    PU = pu;
#endif
#if 1
    PDI = duf_pdi_clone( pdi );
    PDICLONED = 1;
#else
    PDI = pdi;
#endif
    /* duf_scan_qbeginning_sql( sccb ); */
    DUF_TRACE( sql, 0, "@@beginning_sql for '%s'", sccb->title );
    
    DOR( rpr, duf_sccbh_eval_sql_sequence( sccbh /* , PU */  ) );
    
    DUF_TRACE( sql, 0, "@@/beginning_sql for '%s'", sccb->title );
    if ( DUF_NOERROR( rpr ) )
    {
      int rt = 0;

      TOTITEMS = duf_count_total_items( sccbh, &rt ); /* reference */
      DUF_TRACE( temporary, 0, "counted for %s... %lld", SCCB->title, TOTITEMS );
/* total_files for progress bar only :( */
      /* assert(TOTITEMS=38); */
      DUF_SCCB( DUF_TRACE, action, 0, "total_items: %llu", TOTITEMS );
      DUF_TRACE( temporary, 0, "@@@@ %llu items registered in db", TOTITEMS );
      DUF_TRACE( explain, 0, "%llu items registered in db", TOTITEMS );
    }
/*
TODO scan mode
  1. direct, like now
  2. place ID's to temporary table, then scan in certain order
*/
    if ( sccb->init_scan )
    {
      DUF_TRACE( explain, 0, "to init scan" );
      DOR( rpr, sccb->init_scan( NULL /* pstmt */ , pdi ) );
    }
    else
    {
      DUF_TRACE( explain, 0, "no init scan" );
    }
    DOR( rpr, duf_pdi_reinit_anypath( PDI, duf_levinfo_path( PDI ), duf_sccb_get_sql_set( SCCB, DUF_NODE_NODE ), 0 /* caninsert */ ,
                                      duf_pdi_recursive( PDI ) ) );
  }
  if ( pr )
    *pr = rpr;
  return sccbh;
}

int
duf_sccb_handle_close( duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
  if ( sccbh )
  {
    /* final */
    DUF_TRACE( scan, 6, "final sql %s", SCCB->title );
    DOR( r, duf_sccb_eval_final_sql_sequence( SCCB, ( duf_ufilter_t * ) NULL /* pu */  ) );
    if ( PDICLONED )
      duf_pdi_delete( PDI );
    mas_free( sccbh );
  }
  DEBUG_ENDR( r );
}
