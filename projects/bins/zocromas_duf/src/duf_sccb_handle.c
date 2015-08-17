#define DUF_SQL_PDI_STMT

#include <assert.h>
#include <string.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_pdi.h"
#include "duf_levinfo_ref.h"

#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_sccb_def.h"
#include "duf_sccb.h"

#include "duf_sccb_begfin.h"
#include "duf_ufilter_bind.h"

#include "duf_item_scan2.h"

#include "duf_sccbh_shortcuts.h"
/* ###################################################################### */
#include "duf_sccb_handle.h"
/* ###################################################################### */

static unsigned long long
duf_count_total_items( const duf_scan_callbacks_t * sccb, int *pr )
{
  DEBUG_STARTULL( cnt );
  int r = DUF_ERROR_TOTALS;

  /* const char *leaf_selector_total2 = NULL; */

  assert( sccb );
  /* leaf_selector_total2 = duf_get_leaf_sql_set( sccb )->selector_total2; */

  if ( sccb )
  {
    char *sqlt = NULL;
    const duf_sql_set_t *sql_set = NULL;

    r = 0;
#if 0
    sqlt = mas_strdup( "SELECT " );
    sqlt = mas_strcat_x( sqlt, "COUNT(*) AS nf" );
    sqlt = mas_strcat_x( sqlt, " " );
    sqlt = mas_strcat_x( sqlt, leaf_selector_total2 );
#else
    if ( sccb->count_nodes )
      sql_set = duf_get_node_sql_set( sccb );
    else
      sql_set = duf_get_leaf_sql_set( sccb );

    sqlt = duf_selector_total2sql( sql_set );
#endif
    if ( sqlt )
    {
      const char *csql;
/*
 * TODO: counting by 'dry' pass, by db is bad
 * for instance: selected_paths does NOT contain "fileless" directories
 */
      csql = sqlt;
      DUF_TRACE( temp, 5, "count by %s", csql );
      DUF_SQL_START_STMT_NOPDI( csql, r, pstmt );
      duf_bind_ufilter_uni( pstmt );
      DUF_SQL_STEP( r, pstmt );
      if ( r == DUF_SQL_ROW )
      {
        cnt = duf_sql_column_long_long( pstmt, 0 );
        r = 0;
      }
      DUF_SQL_END_STMT_NOPDI( r, pstmt );
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
    *pr = r;
  DEBUG_ENDULL( cnt );
}

#if 0
int
duf_count_db_items2( duf_sel_cb2_match_t match_cb2, duf_sccb_handle_t * sccbh, const duf_sql_set_t * sql_set )
{
  DEBUG_STARTR( r );
  unsigned long long cnt = 0;

  /* match_cb2 = duf_match_leaf2; ... */

/* calling duf_sel_cb_(node|leaf) for each record by sql */
  if ( sql_set->selector2 )
  {
    char *sql = NULL;

    if ( !sql_set->fieldset )
      DUF_MAKE_ERROR( r, DUF_ERROR_SQL_NO_FIELDSET );
    if ( r >= 0 )
      sql = duf_selector2sql( sql_set );
    if ( r >= 0 )
    {
      const char *csql;

      csql = sql;
      DUF_SQL_START_STMT_NOPDI( csql, r, pstmt );

      DUF_TRACE( select, 0, "S:%s %llu/%llu/%u/%u", csql, PU ? PU->size.min : 0, PU ? PU->size.max : 0, PU ? PU->same.min : 0,
                 PU ? PU->same.max : 0 );

      DUF_SQL_BIND_LL( parentdirID, duf_levinfo_dirid( PDI ), r, pstmt );
      duf_bind_ufilter_uni( pstmt );

      DUF_SQL_EACH_ROW( r, pstmt, if ( !match_cb2 || ( match_cb2 ) ( pstmt ) ) cnt++; r = 0 );
      DUF_SQL_END_STMT_NOPDI( r, pstmt );
    }
    if ( sql )
      mas_free( sql );
    sql = NULL;
    if ( r >= 0 )
      duf_levinfo_set_items_files( PDI, cnt );
  }
  /* else                 */
  /*   DUF_MAKE_ERROR(r, DUF_ERROR_PTR); */

  DEBUG_ENDR( r );
}
#endif
duf_sccb_handle_t *
duf_open_sccb_handle( duf_depthinfo_t * pdi, const duf_scan_callbacks_t * sccb, int targc, char *const *targv, const duf_ufilter_t * pu )
{
  duf_sccb_handle_t *sccbh = NULL;
  int r = 0;

  if ( sccb )
  {
    duf_pdi_set_opendir( pdi, sccb->def_opendir );

    sccbh = mas_malloc( sizeof( duf_sccb_handle_t ) );
    memset( sccbh, 0, sizeof( duf_sccb_handle_t ) );
    PARGC = targc;
    PARGV = targv;

    PU = pu;
    PDI = pdi;
    SCCB = sccb;
    /* duf_scan_qbeginning_sql( sccb ); */
    DOR( r, duf_scan_beginning_sql( sccb ) );
    {
      int rt = 0;

      TOTITEMS = duf_count_total_items( SCCB, &rt ); /* reference */
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
      DUF_DO_TEST_R( r, sccb->init_scan( pdi ) );
    }
    else
    {
      DUF_TRACE( explain, 0, "no init scan" );
    }
    DOR( r, duf_pdi_reinit_anypath( PDI, duf_levinfo_path( PDI ), 0 /* caninsert */ , duf_get_node_sql_set( SCCB )->selector2 ) );
  }
  return sccbh;
}

int
duf_close_sccb_handle( duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
  if ( sccbh )
  {
    /* final */
    DUF_TRACE( scan, 0, "final sql %s", SCCB->title );
    DOR( r, duf_scan_final_sql( SCCB ) );
    mas_free( sccbh );
  }
  DEBUG_ENDR( r );
}
