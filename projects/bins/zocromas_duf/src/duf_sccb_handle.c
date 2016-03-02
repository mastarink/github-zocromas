/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */
#include <mastar/trace/mas_trace.h>

#include "duf_tracen_defs.h"                                         /* MAST_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_config.h"                                              /* duf_get_config ♠ */

#include "duf_pdi_credel.h"
#include "duf_pdi_reinit.h"
#include "duf_pdi_filters.h"
#include "duf_pdi_ref.h"
#include "duf_pdi_pi_ref.h"
#include "duf_pdi_stmt.h"

#include "duf_utils.h"                                               /* duf_percent;  etc. ♠ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */
#include "duf_config_ref.h"
#include "duf_config_defs.h"                                         /* DUF_CONF... ♠ */

#include "duf_config_output_util.h"

#include "duf_levinfo_ref.h"

#include "duf_sql_stmt_defs.h"                                       /* DUF_SQL_BIND_S_OPT etc. ♠ */
#include "duf_sql_prepared.h"
#include "duf_sql_bind.h"                                            /* duf_sql_... for DUF_SQL_BIND_... etc. ♠ */

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ♠ */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_UFIELD2 etc. ♠ */

#include "duf_sql_positional.h"

#include "duf_sccb_def.h"
#include "duf_sccb.h"

#include "duf_evsql_selector_new.h"

#include "std_mod_sets.h"

#include "duf_sccb_begfin.h"
#include "duf_ufilter_bind.h"

#include "duf_sccb_scanstage.h"

#include "duf_sccbh_shortcuts.h"
/* ###################################################################### */
#include "duf_sccb_handle.h"
/* ###################################################################### */

/* 20160212.130632 */
static duf_sql_set_pair_t
duf_sccbh_get_leaf_sql_set( duf_sccb_handle_t * sccbh, unsigned force_set_leaf_index, const char *force_leaf_set_name )
{
  duf_sql_set_pair_t set_pair = { NULL, NULL };
  const duf_sql_set_t *set = NULL;

  assert( SCCB );
  {
    if ( !set && force_leaf_set_name )
      for ( unsigned i = 0; i < ( unsigned ) std_leaf_nsets; i++ )
        if ( std_leaf_sets[i].name && 0 == strcmp( std_leaf_sets[i].name, force_leaf_set_name ) )
          set = &std_leaf_sets[i];

    if ( !set && force_set_leaf_index > 0 )
      set = ( force_set_leaf_index <= ( unsigned ) std_leaf_nsets ) ? &std_leaf_sets[force_set_leaf_index - 1] : NULL;

    if ( !set && SCCB->std_leaf_set_name )
      for ( unsigned i = 0; i < ( unsigned ) std_leaf_nsets; i++ )
        if ( std_leaf_sets[i].name && 0 == strcmp( std_leaf_sets[i].name, SCCB->std_leaf_set_name ) )
          set = &std_leaf_sets[i];

    if ( !set && SCCB->use_std_leaf_set_num > 0 )
      set = ( SCCB->use_std_leaf_set_num <= std_leaf_nsets ) ? &std_leaf_sets[SCCB->use_std_leaf_set_num - 1] : NULL;
    if ( !set )
      set = SCCB->leaf.type == DUF_NODE_NONE ? NULL : &SCCB->leaf;
  }
/* sccbh->active_leaf_set = setr; */
/* sccbh->second_leaf_set = &SCCB->leaf; */
  assert( set );
  set_pair.active = set;
  set_pair.second = SCCB->leaf.type == DUF_NODE_NONE ? NULL : &SCCB->leaf;

  assert( !set_pair.active || set_pair.active->type == DUF_NODE_LEAF );
  assert( !set_pair.second || set_pair.second->type == DUF_NODE_LEAF );
  assert( set_pair.active );
/* assert( set_pair.second ); */

/* assert( sccbh->second_leaf_set ); */
  return set_pair;
}

/* 20160212.130653 */
static duf_sql_set_pair_t
duf_sccbh_get_node_sql_set( duf_sccb_handle_t * sccbh, unsigned force_set_node_index, const char *force_node_set_name MAS_UNUSED )
{
  duf_sql_set_pair_t set_pair = { NULL, NULL };
  const duf_sql_set_t *set = NULL;

  assert( SCCB );
  {
    if ( !set && force_node_set_name )
      for ( unsigned i = 0; i < ( unsigned ) std_node_nsets; i++ )
        if ( std_node_sets[i].name && 0 == strcmp( std_node_sets[i].name, force_node_set_name ) )
          set = &std_node_sets[i];

    if ( !set && force_set_node_index > 0 )
      set = ( force_set_node_index <= ( unsigned ) std_node_nsets ) ? &std_node_sets[force_set_node_index - 1] : NULL;

    if ( !set && SCCB->std_node_set_name )
      for ( unsigned i = 0; i < ( unsigned ) std_node_nsets; i++ )
        if ( std_node_sets[i].name && 0 == strcmp( std_node_sets[i].name, SCCB->std_node_set_name ) )
          set = &std_node_sets[i];

    if ( !set && SCCB->use_std_node_set_num > 0 )
      set = ( SCCB->use_std_node_set_num <= std_node_nsets ) ? &std_node_sets[SCCB->use_std_node_set_num - 1] : NULL;
    if ( !set )
      set = SCCB->node.type == DUF_NODE_NONE ? NULL : &SCCB->node;
  }
/* sccbh->active_node_set = setr; */
/* sccbh->second_node_set = &SCCB->node; */
  assert( set );
  set_pair.active = set;
  set_pair.second = SCCB->node.type == DUF_NODE_NONE ? NULL : &SCCB->node;

  assert( !set_pair.active || set_pair.active->type == DUF_NODE_NODE );
  assert( !set_pair.second || set_pair.second->type == DUF_NODE_NODE );
  assert( set_pair.active );
/* assert( set_pair.second ); */
  return set_pair;
}

/* 20160212.130701 */
duf_sql_set_pair_t
duf_sccbh_get_sql_set_f( duf_sccb_handle_t * sccbh, duf_node_type_t node_type )
{
  duf_sql_set_pair_t set_pair = {
    NULL, NULL
  };

  assert( SCCB );
  switch ( node_type )
  {
  case DUF_NODE_LEAF:
    set_pair = duf_sccbh_get_leaf_sql_set( sccbh, PU->std_leaf_set_num, PU->std_leaf_set_name );
    break;
  case DUF_NODE_NODE:
    set_pair = duf_sccbh_get_node_sql_set( sccbh, PU->std_node_set_num, PU->std_node_set_name );
    break;
  case DUF_NODE_NONE:
    break;
  default:
    break;
  }
  assert( !set_pair.active || set_pair.active->type == node_type );
  assert( !set_pair.second || set_pair.second->type == node_type );
  return set_pair;
}

/* 20160212.130712 */
static unsigned long long
duf_count_total_items( duf_sccb_handle_t * sccbh, int *pr )
{
  DUF_STARTULL( cnt );
  int rpr = 0;
  unsigned long long cnt1 MAS_UNUSED = 0;

/* const char *leaf_selector_total2 = NULL; */

  assert( SCCB );
/* leaf_selector_total2 = duf_get_leaf_sql_set( SCCB )->selector_total2; */

  if ( SCCB )
  {
    char *sqlt = NULL;
    duf_sql_set_pair_t sql_set_pair = { NULL, NULL };

    sql_set_pair = duf_sccbh_get_sql_set_f( sccbh, SCCB->count_nodes ? DUF_NODE_NODE : DUF_NODE_LEAF );
#if 0
  /* XXX TODO XXX */
    sqlt = duf_selector2sql_new( sql_set, PDI->pdi_name, 1, &rpr );
#else
    sqlt = duf_selector2sql_2new( sql_set_pair.active, sql_set_pair.second, PDI->pdi_name, 1, &rpr );
#endif
    assert( DUF_NOERROR( rpr ) );
    if ( DUF_NOERROR( rpr ) && sqlt )
    {
      const char *csql;

/*
 * TODO: counting by 'dry' pass, by db is bad
 * for instance: selected_paths does NOT contain "fileless" directories
 */
      csql = sqlt;
#if 0
    /* 20160205.121213 */
      DUF_SQL_START_STMT_NOPDI( csql, rpr, pstmt );
#else
      DUF_SQL_START_STMT_LOCAL( PDI, csql, rpr, pstmt );
#endif
      assert( DUF_NOERROR( rpr ) );
    /* if ( !PY )                                 */
    /* {                                          */
    /*   T( "path:%s", duf_levinfo_path( PDI ) ); */
    /*   assert( PY );                            */
    /* }                                          */
      DOR( rpr, duf_bind_ufilter_uni( pstmt, PU, PY, NULL, NULL /* ptr */  ) );
      assert( DUF_NOERROR( rpr ) );
      DUF_SQL_STEP( rpr, pstmt );
      if ( DUF_IS_ERROR_N( rpr, DUF_SQL_ROW ) )
      {
        long long cntfull = 0;

#if 1
        cntfull = duf_sql_column_long_long( pstmt, 0 );
        cnt1 = DUF_GET_UFIELD2( nf );
#else
        cntfull = DUF_GET_UFIELD2( CNT );
#endif
        MAST_TRACE( sql, 1, "@@@counted A %llu : %llu by %s", cntfull, cnt1, csql );
      /* with .cte sql counts all childs recursively, without .cte counts ALL nodes, so need subtract upper... */
        cnt = cntfull;
        if ( cntfull > 0 && !sql_set_pair.active->cte && SCCB->count_nodes )
        {
          cnt += duf_pdi_reldepth( PDI ) - duf_pdi_depth( PDI ) /* - 1 20160118.153828 */ ;
        }
      /* rpr = 0; */
      }
      MAST_TRACE( sql, 1, "@@@counted B %llu by %s", cnt, csql );
    /* T( "@@counted B %llu:%llu by %s (%llu)", cnt, cnt1, csql, PY->topdirid ); */
#if 0
    /* 20160205.121213 */
      DUF_SQL_END_STMT_NOPDI( rpr, pstmt );
#else
      DUF_SQL_END_STMT_LOCAL( PDI, rpr, pstmt );
#endif
      assert( DUF_NOERROR( rpr ) );
    /* if ( !cnt )                                                                                         */
    /* {                                                                                                   */
    /*   T( "@%llu:%llu:%llu; %s; %s", PY->topdirid, cnt1, cnt, duf_uni_scan_action_title( SCCB ), csql ); */
    /* }                                                                                                   */
    }
  /* T( "%llu : %s", cnt, sqlt ); */
    mas_free( sqlt );
  }
  else
  {
    DUF_MAKE_ERROR( rpr, DUF_ERROR_TOTALS );
    MAST_TRACE( explain, 0, "didn't count files in db" );
  }
  DUF_TEST_R( rpr );
  if ( pr )
    *pr = rpr;
/* T( "@(%llu) %llu:%d;", cnt1, cnt, duf_pdi_reldepth( PDI ) - duf_pdi_depth( PDI ) ); */
  DUF_ENDULL( cnt );
}

static int
duf_sccbh_eval_sqlsq( const duf_sccb_handle_t * sccbh )
{
  DUF_STARTR( r );
  if ( !duf_pdi_root( PDI )->sql_beginning_done )
  {
    DOR( r, duf_sccb_eval_sqlsq( SCCB, PU, ( duf_yfilter_t * ) NULL, PDI->pdi_name ) );
    if ( DUF_NOERROR( r ) )
    {
      duf_pdi_root( PDI )->sql_beginning_done = 1;
    }
  }
  DUF_ENDR( r );
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
    DUF_SCCB( MAST_TRACE, action, 0, "total_items: %llu; m: %llu rd:%d; d:%d", TOTITEMS, m, duf_pdi_reldepth( PDI ), duf_pdi_depth( PDI ) );
  /* assert( PDI->seq_node <= m ); FIXME counters! */
  /*@ 2. progress bar */
    if ( m > 0 )
      duf_percent( PDI->seq_node, PDI->total_bytes, PDI->total_files, m, duf_uni_scan_action_title( SCCB ) );
  }
}

void
duf_sccbh_leaf_progress( duf_sccb_handle_t * sccbh )
{
  if ( !SCCB->count_nodes && !SCCB->no_count && !SCCB->no_progress && TOTITEMS > 0 )
  {
    long long m;

    m = TOTITEMS;
    DUF_SCCB( MAST_TRACE, action, 0, "total_items: %llu; m: %llu rd:%d; d:%d", TOTITEMS, m, duf_pdi_reldepth( PDI ), duf_pdi_depth( PDI ) );
  /* assert( PDI->seq_node <= m ); FIXME counters! */
    if ( m > 0 )
    {
      duf_percent( PDI->seq_leaf, PDI->total_bytes, PDI->total_files, m, duf_uni_scan_action_title( SCCB ) );
      MAST_TRACE( seq, 0, "PROGRESS: seq:%llu; seq_leaf:%llu OF %llu", PDI->seq, PDI->seq_leaf, m );
    }
  }
}

/* 20151027.144501 */
static void
duf_sccbh_atom_cb( const struct duf_sccb_handle_s *sccbh MAS_UNUSED, duf_scanstage_t scanstage MAS_UNUSED, duf_stmnt_t * pstmt MAS_UNUSED,
                   duf_scanner_t scanner MAS_UNUSED, duf_node_type_t node_type MAS_UNUSED, int r MAS_UNUSED )
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
    MAST_TRACE( fs, 2, "set def. opendir: %d", sccb->def_opendir );
    duf_pdi_set_opendir( pdi, sccb->def_opendir );

    assert( pdi );
    assert( pdi->pathinfo.levinfo );
    assert( duf_levinfo_dirid( pdi ) );
    assert( duf_levinfo_path( pdi ) );
    sccbh = duf_sccb_handle_create(  );

    SCCB = sccb;
    PARGC = targc;
    PARGV = targv;
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
    PDI->total_bytes = 0;
    PDI->total_files = 0;
    if ( SCCB->beginning_sql_seq )
      PDI->sql_selected_done = SCCB->beginning_sql_seq->set_selected_db;
  /* duf_scan_qbeginning_sql( sccb ); */
    MAST_TRACE( sql, 1, "@@beginning_sql for '%s'", SCCB->title );

    DOR( rpr, duf_sccbh_eval_sqlsq( sccbh /* , PU */  ) );
    MAST_TRACE( sql, 1, "@@/beginning_sql for '%s'", SCCB->title );
    if ( DUF_NOERROR( rpr ) )
    {
      if ( !SCCB->no_count )
      {
        TOTITEMS = duf_count_total_items( sccbh, &rpr );             /* reference */
        if ( DUF_NOERROR( rpr ) )
          TOTCOUNTED = 1;
      }
    /* T( "TOTCOUNTED:%d; TOTITEMS:%llu for %s", TOTCOUNTED, TOTITEMS, duf_uni_scan_action_title( SCCB ) ); */
      MAST_TRACE( temporary, 0, "counted for %s... %lld", SCCB->title, TOTITEMS );
/* total_files for progress bar only :( */
    /* assert(TOTITEMS=38); */
      DUF_SCCB( MAST_TRACE, action, 0, "total_items: %llu", TOTITEMS );
      MAST_TRACE( temporary, 0, "@@@@ %llu items registered in db", TOTITEMS );
      MAST_TRACE( explain, 0, "%llu items registered in db", TOTITEMS );
      if ( duf_output_progress(  ) )
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
      if ( SCCB->init_scan )
      {
        MAST_TRACE( explain, 0, "to init scan" );
        DOR( rpr, SCCB->init_scan( NULL /* pstmt */ , pdi ) );
      }
      else
      {
        MAST_TRACE( explain, 0, "no init scan" );
      }
      assert( PDI->pathinfo.levinfo );
    /* T(">>> %llu : %llu", PU->std_leaf_set_num,  PU->std_node_set_num); */
      DOR( rpr,
           duf_pdi_reinit_anypath( PDI, duf_levinfo_path( PDI ), duf_pdi_pu( PDI ),
                                   duf_sccbh_get_sql_set_f( sccbh, DUF_NODE_NODE ).active, 0 /* caninsert */ ,
                                   duf_pdi_recursive( PDI ), duf_pdi_allow_dirs( PDI ), duf_pdi_linear( PDI ) ) );
    }
  }
  else
  {
    T( "sccb:%d; dirid:%llu - %s", SCCB ? 1 : 0, duf_levinfo_dirid( pdi ), duf_levinfo_path( pdi ) );
  /* assert(0); */
  }
  if ( pr )
    *pr = rpr;
  return sccbh;
}

int
duf_sccb_handle_close( duf_sccb_handle_t * sccbh )
{
  DUF_STARTR( r );
  if ( sccbh )
  {
  /* final */
    MAST_TRACE( scan, 6, "final sql %s", SCCB->title );
    DOR( r, duf_sccb_eval_final_sqlsq( SCCB, ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL ) );
    if ( PDICLONED )
      duf_pdi_delete( PDI );
    mas_free( sccbh );
  }
  DUF_ENDR( r );
}
