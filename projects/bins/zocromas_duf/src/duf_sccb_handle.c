#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_pdi_credel.h"
#include "duf_pdi_reinit.h"
#include "duf_pdi_ref.h"

#include "duf_option_defs.h"    /* for DUF_ACTG_FLAG( progress ) !ONLY! */
#include "duf_utils.h"          /* duf_percent */

#include "duf_config_util.h"
#include "duf_config_ref.h"
#include "duf_config_defs.h"

#include "duf_levinfo_ref.h"

#include "duf_sql_stmt_defs.h"
#include "duf_sql_prepared.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"

#include "duf_sql_positional.h"

#include "duf_sccb_def.h"
#include "duf_sccb.h"
#include "evsql_selector.h"

#include "duf_sccb_begfin.h"
#include "duf_ufilter_bind.h"

#include "duf_sccb_scanstage.h"

#include "duf_sccbh_shortcuts.h"
/* ###################################################################### */
#include "duf_sccb_handle.h"
/* ###################################################################### */

/* 20151006.150004 */
static unsigned long long
duf_count_total_items( const duf_sccb_handle_t * sccbh, int *pr )
{
  DEBUG_STARTULL( cnt );
  int rpr = 0;
  unsigned long long cnt1 = 0;

  /* const char *leaf_selector_total2 = NULL; */

  assert( SCCB );
  /* leaf_selector_total2 = duf_get_leaf_sql_set( SCCB )->selector_total2; */

  if ( SCCB )
  {
    char *sqlt = NULL;
    const duf_sql_set_t *sql_set = NULL;

#if 0
    sqlt = mas_strdup( "SELECT " );
    sqlt = mas_strcat_x( sqlt, "COUNT(*) AS nf" );
    sqlt = mas_strcat_x( sqlt, " " );
    sqlt = mas_strcat_x( sqlt, leaf_selector_total2 );
#else

#  if 0
    if ( SCCB->count_nodes )
      sql_set = duf_sccb_get_sql_set_f( SCCB, DUF_NODE_NODE, PU->std_leaf_set, PU->std_node_set );
    else
      sql_set = duf_sccb_get_sql_set_f( SCCB, DUF_NODE_LEAF, PU->std_leaf_set, PU->std_node_set );
#  else
    sql_set = duf_sccb_get_sql_set_f( SCCB, SCCB->count_nodes ? DUF_NODE_NODE : DUF_NODE_LEAF, PU->std_leaf_set, PU->std_node_set );
#  endif
    sqlt = duf_selector_total2sql( sql_set, PDI->pdi_name, &rpr );
    assert( DUF_NOERROR( rpr ) );
#endif
    if ( DUF_NOERROR( rpr ) && sqlt )
    {
      const char *csql;

/*
 * TODO: counting by 'dry' pass, by db is bad
 * for instance: selected_paths does NOT contain "fileless" directories
 */
      csql = sqlt;
      DUF_SQL_START_STMT_NOPDI( csql, rpr, pstmt );
      assert( DUF_NOERROR( rpr ) );
      /* if ( !PY )                                 */
      /* {                                          */
      /*   T( "path:%s", duf_levinfo_path( PDI ) ); */
      /*   assert( PY );                            */
      /* }                                          */
      DOR( rpr, duf_bind_ufilter_uni( pstmt, PU, PY, NULL ) );
      assert( DUF_NOERROR( rpr ) );
      DUF_SQL_STEP( rpr, pstmt );
      if ( DUF_IS_ERROR_N( rpr, DUF_SQL_ROW ) )
      {
#if 1
        cnt = duf_sql_column_long_long( pstmt, 0 );
        cnt1 = DUF_GET_UFIELD2( nf );
#else
        cnt = DUF_GET_UFIELD2( CNT );
#endif
        DUF_TRACE( sql, 1, "@@counted A %llu : %llu by %s", cnt, cnt1, csql );
        /* with .cte sql counts all childs recursively, without .cte counts ALL nodes, so need subtract upper... */
        if ( cnt > 0 && !sql_set->cte && SCCB->count_nodes )
          cnt += duf_pdi_reldepth( PDI ) - duf_pdi_depth( PDI ) - 1;
        /* rpr = 0; */
      }
      DUF_TRACE( sql, 1, "@@counted B %llu by %s", cnt, csql );
      /* T( "@@counted B %llu:%llu by %s (%llu)", cnt, cnt1, csql, PY->topdirid ); */
      DUF_SQL_END_STMT_NOPDI( rpr, pstmt );
      assert( DUF_NOERROR( rpr ) );
      DUF_TRACE( temp, 5, "counted %llu SIZED files in db", cnt );
      DUF_TRACE( explain, 0, "counted %llu SIZED files in db", cnt );
    }
    mas_free( sqlt );
  }
  else
  {
    DUF_MAKE_ERROR( rpr, DUF_ERROR_TOTALS );
    DUF_TRACE( explain, 0, "didn't count files in db" );
  }
  DUF_TEST_R( rpr );
  if ( pr )
    *pr = rpr;
  /* T( "%s cnt:%llu", duf_uni_scan_action_title( SCCB ), cnt ); */
  DEBUG_ENDULL( cnt );
}

static int
duf_sccbh_eval_sqlsq( const duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
  if ( !duf_pdi_root( PDI )->sql_beginning_done )
  {
    DOR( r, duf_sccb_eval_sqlsq( SCCB, PU, ( duf_yfilter_t * ) NULL, PDI->pdi_name ) );
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

void
duf_sccbh_node_progress( duf_sccb_handle_t * sccbh )
{
  if ( SCCB->count_nodes && !SCCB->no_count && !SCCB->no_progress && TOTITEMS > 0 )
  {
    long long m;

#if 0
    m = TOTITEMS + duf_pdi_reldepth( PDI ) - duf_pdi_depth( PDI ) - 1;
#else
    m = TOTITEMS;
#endif
    DUF_SCCB( DUF_TRACE, action, 0, "total_items: %llu; m: %llu rd:%d; d:%d", TOTITEMS, m, duf_pdi_reldepth( PDI ), duf_pdi_depth( PDI ) );
    /* assert( PDI->seq_node <= m ); FIXME counters! */
    /*@ 2. progress bar */
    if ( m > 0 )
      duf_percent( PDI->seq_node, m, duf_uni_scan_action_title( SCCB ) );
  }
}

void
duf_sccbh_leaf_progress( duf_sccb_handle_t * sccbh )
{
  if ( !SCCB->count_nodes && !SCCB->no_count && !SCCB->no_progress && TOTITEMS > 0 )
  {
    long long m;

    m = TOTITEMS;
    DUF_SCCB( DUF_TRACE, action, 0, "total_items: %llu; m: %llu rd:%d; d:%d", TOTITEMS, m, duf_pdi_reldepth( PDI ), duf_pdi_depth( PDI ) );
    /* assert( PDI->seq_node <= m ); FIXME counters! */
    if ( m > 0 )
    {
      duf_percent( PDI->seq_leaf, m, duf_uni_scan_action_title( SCCB ) );
      DUF_TRACE( seq, 0, "PROGRESS: seq:%llu; seq_leaf:%llu OF %llu", PDI->seq, PDI->seq_leaf, m );
    }
  }
}

/* 20151027.144501 */
static void
duf_sccbh_atom_cb( const struct duf_sccb_handle_s *sccbh DUF_UNUSED, duf_scanstage_t scanstage DUF_UNUSED, duf_stmnt_t * pstmt DUF_UNUSED,
                   duf_scanner_t scanner DUF_UNUSED, duf_node_type_t node_type DUF_UNUSED, int r DUF_UNUSED )
{
  static unsigned n = 0;
  char c = '-';

  if ( n > 3 )
    n = 0;
  switch ( n++ )
  {
  case 0:
    c = '-';
    break;
  case 1:
    c = '\\';
    break;
  case 2:
    c = '|';
    break;
  case 3:
    c = '/';
    break;
  }
  fprintf( stderr, "\r%c%1s\r", c, duf_scanstage_shortname( scanstage ) );
}

/* 20151027.144450 */
/*
 * create & open duf_sccb_handle_t from pdi & sccb (+targ)
 * */
duf_sccb_handle_t *
duf_sccb_handle_open( duf_depthinfo_t * pdi, const duf_scan_callbacks_t * sccb, int targc, char *const *targv, int *pr )
{
  duf_sccb_handle_t *sccbh = NULL;
  int rpr = 0;

  /* assert( pdi->pyp ); */
  if ( sccb && pdi && duf_levinfo_dirid( pdi ) )
  {
    DUF_TRACE( fs, 2, "set def. opendir: %d", sccb->def_opendir );
    duf_pdi_set_opendir( pdi, sccb->def_opendir );

    assert( pdi );
    assert( pdi->pathinfo.levinfo );
    assert( duf_levinfo_dirid( pdi ) );
    assert( duf_levinfo_path( pdi ) );
    sccbh = duf_sccb_handle_create(  );

    PARGC = targc;
    PARGV = targv;
    SCCB = sccb;
#if 0
    PU = pu;
#endif
#if 1
    PDI = duf_pdi_clone( pdi, 0 /* no_li */  );
    assert( PDI->pathinfo.levinfo );
    PDICLONED = 1;
#else
    PDI = pdi;
#endif
    if ( SCCB->beginning_sql_seq )
      PDI->sql_selected_done = SCCB->beginning_sql_seq->set_selected_db;
    /* duf_scan_qbeginning_sql( sccb ); */
    DUF_TRACE( sql, 1, "@@beginning_sql for '%s'", sccb->title );

    DOR( rpr, duf_sccbh_eval_sqlsq( sccbh /* , PU */  ) );

    DUF_TRACE( sql, 1, "@@/beginning_sql for '%s'", sccb->title );
    if ( DUF_NOERROR( rpr ) )
    {
      if ( !SCCB->no_count )
      {
        TOTITEMS = duf_count_total_items( sccbh, &rpr ); /* reference */
        if ( DUF_NOERROR( rpr ) )
          TOTCOUNTED = 1;
      }
      /* T( "TOTCOUNTED:%d; TOTITEMS:%llu for %s", TOTCOUNTED, TOTITEMS, duf_uni_scan_action_title( SCCB ) ); */
      DUF_TRACE( temporary, 0, "counted for %s... %lld", SCCB->title, TOTITEMS );
/* total_files for progress bar only :( */
      /* assert(TOTITEMS=38); */
      DUF_SCCB( DUF_TRACE, action, 0, "total_items: %llu", TOTITEMS );
      DUF_TRACE( temporary, 0, "@@@@ %llu items registered in db", TOTITEMS );
      DUF_TRACE( explain, 0, "%llu items registered in db", TOTITEMS );
      if ( DUF_ACTG_FLAG( progress ) )
      {
        sccbh->progress_node_cb = duf_sccbh_node_progress;
        sccbh->progress_leaf_cb = duf_sccbh_leaf_progress;
      }
      sccbh->atom_cb = duf_sccbh_atom_cb;
    }
/*
TODO scan mode
  1. direct, like now
  2. place ID's to temporary table, then scan in certain order
*/
    if ( DUF_NOERROR( rpr ) )
    {
      if ( sccb->init_scan )
      {
        DUF_TRACE( explain, 0, "to init scan" );
        DOR( rpr, sccb->init_scan( NULL /* pstmt */ , pdi ) );
      }
      else
      {
        DUF_TRACE( explain, 0, "no init scan" );
      }
      assert( PDI->pathinfo.levinfo );
      /* T(">>> %llu : %llu", PU->std_leaf_set,  PU->std_node_set); */
      DOR( rpr,
           duf_pdi_reinit_anypath( PDI, duf_levinfo_path( PDI ), duf_pdi_pu( PDI ),
                                   duf_sccb_get_sql_set_f( SCCB, DUF_NODE_NODE, PU->std_leaf_set, PU->std_node_set ), 0 /* caninsert */ ,
                                   duf_pdi_recursive( PDI ) ) );
    }
  }
  else
  {
    T( "sccb:%d; dirid:%llu - %s", sccb ? 1 : 0, duf_levinfo_dirid( pdi ), duf_levinfo_path( pdi ) );
    /* assert(0); */
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
    DOR( r, duf_sccb_eval_final_sqlsq( SCCB, ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL ) );
    if ( PDICLONED )
      duf_pdi_delete( PDI );
    mas_free( sccbh );
  }
  DEBUG_ENDR( r );
}
