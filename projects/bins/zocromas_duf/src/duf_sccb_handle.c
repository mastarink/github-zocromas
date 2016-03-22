/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

/* #include "duf_tracen_defs.h"                                         (* T; TT; TR ✗ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ✗ *) */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ✗ *) */
/* #include "duf_dodefs.h"                                              (* DOR ✗ *) */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config.h"                                              /* duf_get_config ✗ */

#include "duf_pdi_credel.h"                                          /* duf_pdi_create; duf_pdi_kill ✗ */
#include "duf_pdi_reinit.h"
#include "duf_pdi_filters.h"                                         /* duf_pdi_pu; etc. ✗ */
#include "duf_pdi_ref.h"
#include "duf_pdi_pi_ref.h"
#include "duf_pdi_stmt.h"                                            /* duf_pdi_find_statement_by_id; etc. ✗ */

#include "duf_utils.h"                                               /* duf_percent;  etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
/* #include "duf_config_ref.h" */
/* #include "duf_config_defs.h"                                         (* DUF_CONF... ✗ *) */

#include "duf_config_output_util.h"

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */

/* #include "duf_sql_stmt_defs.h"                                       (* DUF_SQL_BIND_S_OPT etc. ✗ *) */
#include "duf_sql_se_stmt_defs.h"                                    /* DUF_SQL_SE_BIND_S_OPT etc. ✗ */
#include "duf_sql_prepared.h"                                        /* duf_sql_(prepare|step|finalize) ✗ */
#include "duf_sql_bind.h"                                            /* duf_sql_... for DUF_SQL_BIND_... etc. ✗ */

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_UFIELD2 etc. ✗ */

#include "duf_sql_positional.h"                                      /* duf_sql_column_long_long etc. ✗ */

#include "duf_sccb_def.h"
#include "duf_sccb.h"
#include "duf_sccb_row.h"
#include "duf_sccb_begfin.h"

#include "duf_evsql_selector_new.h"

#include "std_mod_sets.h"

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
  duf_sql_set_pair_t set_pair = {.orderid = 0, NULL, NULL };
  const duf_sql_set_t *set = NULL;

  assert( H_SCCB );
  {
    if ( !set && force_leaf_set_name )
      for ( unsigned i = 0; i < ( unsigned ) std_leaf_nsets; i++ )
        if ( std_leaf_sets[i].name && 0 == strcmp( std_leaf_sets[i].name, force_leaf_set_name ) )
          set = &std_leaf_sets[i];

    if ( !set && force_set_leaf_index > 0 )
      set = ( force_set_leaf_index <= ( unsigned ) std_leaf_nsets ) ? &std_leaf_sets[force_set_leaf_index - 1] : NULL;

    if ( !set && H_SCCB->std_leaf_set_name )
      for ( unsigned i = 0; i < ( unsigned ) std_leaf_nsets; i++ )
        if ( std_leaf_sets[i].name && 0 == strcmp( std_leaf_sets[i].name, H_SCCB->std_leaf_set_name ) )
          set = &std_leaf_sets[i];

    if ( !set && H_SCCB->use_std_leaf_set_num > 0 )
      set = ( H_SCCB->use_std_leaf_set_num <= std_leaf_nsets ) ? &std_leaf_sets[H_SCCB->use_std_leaf_set_num - 1] : NULL;
    if ( !set )
      set = H_SCCB->leaf.type == DUF_NODE_NONE ? NULL : &H_SCCB->leaf;
  }
/* sccbh->active_leaf_set = setr; */
/* sccbh->second_leaf_set = &H_SCCB->leaf; */
  assert( set );
  set_pair.active = set;
  set_pair.second = H_SCCB->leaf.type == DUF_NODE_NONE ? NULL : &H_SCCB->leaf;

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
  duf_sql_set_pair_t set_pair = {.orderid = 0, NULL, NULL };
  const duf_sql_set_t *set = NULL;

  assert( H_SCCB );
  {
    if ( !set && force_node_set_name )
      for ( unsigned i = 0; i < ( unsigned ) std_node_nsets; i++ )
        if ( std_node_sets[i].name && 0 == strcmp( std_node_sets[i].name, force_node_set_name ) )
          set = &std_node_sets[i];

    if ( !set && force_set_node_index > 0 )
      set = ( force_set_node_index <= ( unsigned ) std_node_nsets ) ? &std_node_sets[force_set_node_index - 1] : NULL;

    if ( !set && H_SCCB->std_node_set_name )
      for ( unsigned i = 0; i < ( unsigned ) std_node_nsets; i++ )
        if ( std_node_sets[i].name && 0 == strcmp( std_node_sets[i].name, H_SCCB->std_node_set_name ) )
          set = &std_node_sets[i];

    if ( !set && H_SCCB->use_std_node_set_num > 0 )
      set = ( H_SCCB->use_std_node_set_num <= std_node_nsets ) ? &std_node_sets[H_SCCB->use_std_node_set_num - 1] : NULL;
    if ( !set )
      set = H_SCCB->node.type == DUF_NODE_NONE ? NULL : &H_SCCB->node;
  }
/* sccbh->active_node_set = setr; */
/* sccbh->second_node_set = &H_SCCB->node; */
  assert( set );
/* set_pair.orderid = H_PU->orderid; */
  set_pair.active = set;
  set_pair.second = H_SCCB->node.type == DUF_NODE_NONE ? NULL : &H_SCCB->node;

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
    .orderid = 0, NULL, NULL
  };

  assert( H_SCCB );
  switch ( node_type )
  {
  case DUF_NODE_LEAF:
    set_pair = duf_sccbh_get_leaf_sql_set( sccbh, H_PU->std_leaf_set_num, H_PU->std_leaf_set_name );
    break;
  case DUF_NODE_NODE:
    set_pair = duf_sccbh_get_node_sql_set( sccbh, H_PU->std_node_set_num, H_PU->std_node_set_name );
    break;
  case DUF_NODE_NONE:
    break;
  default:
    break;
  }
  set_pair.orderid = H_PU->orderid;
  assert( !set_pair.active || set_pair.active->type == node_type );
  assert( !set_pair.second || set_pair.second->type == node_type );
  return set_pair;
}

/* 20160212.130712 */
static
/*   unsigned long long                                        */
/* duf_count_total_items( duf_sccb_handle_t * sccbh, int *pr ) */
SRP( SCCBH, unsigned long long, cnt, 0, count_total_items, duf_sccb_handle_t * sccbh )
{
/* DUF_STARTULL( cnt ); */
/* int rpr = 0; */
  unsigned long long cnt1 MAS_UNUSED = 0;

/* const char *leaf_selector_total2 = NULL; */

  assert( H_SCCB );
/* leaf_selector_total2 = duf_get_leaf_sql_set( H_SCCB )->selector_total2; */

  if ( H_SCCB )
  {
    char *sqlt = NULL;
    duf_sql_set_pair_t sql_set_pair = {.orderid = 0, NULL, NULL };

    sql_set_pair = duf_sccbh_get_sql_set_f( sccbh, H_SCCB->count_nodes ? DUF_NODE_NODE : DUF_NODE_LEAF );
#if 0
  /* XXX TODO XXX */
    sqlt = duf_selector2sql_new( sql_set, sql_set_pair.orderid, H_PDI->pdi_name, 1, &rpr );
#else
    sqlt = duf_selector2sql_2new( sql_set_pair.active, sql_set_pair.second, sql_set_pair.orderid, H_PDI->pdi_name, 1, QPERRIND );
#endif
    assert( QNOERR );
    if ( QNOERR && sqlt )
    {
      const char *csql;

/*
 * TODO: counting by 'dry' pass, by db is bad
 * for instance: selected_paths does NOT contain "fileless" directories
 */
      csql = sqlt;
#if 0
    /* 20160205.121213 */
      DUF_SQL_SE_START_STMT_NOPDI( csql, pstmt );
#else
      DUF_SQL_SE_START_STMT_LOCAL( H_PDI, csql, pstmt );
#endif
    /* (* (* assert( QNOERR ); *) *) */
    /* if ( !H_PY )                                 */
    /* {                                          */
    /*   T( "path:%s", duf_levinfo_path( H_PDI ) ); */
    /*   assert( H_PY );                            */
    /* }                                          */
      CR( bind_ufilter_uni, pstmt, H_PU, H_PY, NULL, NULL /* ptr */  );
    /* (* (* assert( QNOERR ); *) *) */
      DUF_SQL_SE_STEP( pstmt );
      if ( QISERR1_N( SQL_ROW ) )
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
        if ( cntfull > 0 && !sql_set_pair.active->cte && H_SCCB->count_nodes )
        {
          cnt += duf_pdi_reldepth( H_PDI ) - duf_pdi_depth( H_PDI ) /* - 1 20160118.153828 */ ;
        }
      /* rpr = 0; */
      }
      MAST_TRACE( sql, 1, "@@@counted B %llu by %s", cnt, csql );
    /* T( "@@counted B %llu:%llu by %s (%llu)", cnt, cnt1, csql, H_PY->topdirid ); */
#if 0
    /* 20160205.121213 */
      DUF_SQL_SE_END_STMT_NOPDI( pstmt );
#else
      DUF_SQL_SE_END_STMT_LOCAL( H_PDI, pstmt );
#endif
    /* (* (* assert( QNOERR ); *) *) */
    /* if ( !cnt )                                                                                         */
    /* {                                                                                                   */
    /*   T( "@%llu:%llu:%llu; %s; %s", H_PY->topdirid, cnt1, cnt, duf_uni_scan_action_title( H_SCCB ), csql ); */
    /* }                                                                                                   */
    }
  /* T( "%llu : %s", cnt, sqlt ); */
    mas_free( sqlt );
  }
  else
  {
    ERRMAKE( TOTALS );
    MAST_TRACE( explain, 10, "didn't count files in db" );
  }
/* if ( pr )    */
/*   *pr = rpr; */
/* T( "@(%llu) %llu:%d;", cnt1, cnt, duf_pdi_reldepth( H_PDI ) - duf_pdi_depth( H_PDI ) ); */
/* DUF_ENDULL( cnt ); */
  ERP( SCCBH, unsigned long long, cnt, 0, count_total_items, duf_sccb_handle_t * sccbh );
}

static
SR( SCCBH, sccbh_eval_sqlsq, const duf_sccb_handle_t * sccbh )
{
/* DUF_STARTR( r ); */
  if ( !duf_pdi_root( H_PDI )->sql_beginning_done )
  {
    CR( sccb_eval_sqlsq, H_SCCB, H_PU, ( duf_yfilter_t * ) NULL, H_PDI->pdi_name );
    if ( QNOERR )
    {
      duf_pdi_root( H_PDI )->sql_beginning_done = 1;
    }
  }
/* DUF_ENDR( r ); */
  ER( SCCBH, sccbh_eval_sqlsq, const duf_sccb_handle_t * sccbh );
}

static duf_sccb_handle_t *
duf_sccb_handle_create( const duf_scan_callbacks_t * const *psccb, const mas_cargvc_t * ptarg )
{
  duf_sccb_handle_t *sccbh = NULL;

  sccbh = mas_malloc( sizeof( duf_sccb_handle_t ) );
  memset( sccbh, 0, sizeof( duf_sccb_handle_t ) );
  sccbh->sccb_array = psccb;
  sccbh->parg.argc = ptarg->argc;
  sccbh->parg.argv = ptarg->argv;
  assert( H_SCCB == ( *psccb ) );
  return sccbh;
}

void
duf_sccbh_node_progress( duf_sccb_handle_t * sccbh )
{
  if ( H_SCCB->count_nodes && !H_SCCB->no_count && !H_SCCB->no_progress && H_TOTITEMS > 0 )
  {
    long long m;

#if 0
    m = H_TOTITEMS + duf_pdi_reldepth( H_PDI ) - duf_pdi_depth( H_PDI ) - 1;
#else
    m = H_TOTITEMS;
#endif
    DUF_SCCB( MAST_TRACE, action, 0, "total_items: %llu; m: %llu rd:%d; d:%d", H_TOTITEMS, m, duf_pdi_reldepth( H_PDI ), duf_pdi_depth( H_PDI ) );
  /* assert( H_PDI->seq_node <= m ); FIXME counters! */
  /*@ 2. progress bar */
    if ( m > 0 )
      duf_percent( H_PDI->seq_node, H_PDI->total_bytes, H_PDI->total_files, m, duf_uni_scan_action_title( H_SCCB ) );
  }
}

void
duf_sccbh_leaf_progress( duf_sccb_handle_t * sccbh )
{
  if ( !H_SCCB->count_nodes && !H_SCCB->no_count && !H_SCCB->no_progress && H_TOTITEMS > 0 )
  {
    long long m;

    m = H_TOTITEMS;
    DUF_SCCB( MAST_TRACE, action, 0, "total_items: %llu; m: %llu rd:%d; d:%d", H_TOTITEMS, m, duf_pdi_reldepth( H_PDI ), duf_pdi_depth( H_PDI ) );
  /* assert( H_PDI->seq_node <= m ); FIXME counters! */
    if ( m > 0 )
    {
      duf_percent( H_PDI->seq_leaf, H_PDI->total_bytes, H_PDI->total_files, m, duf_uni_scan_action_title( H_SCCB ) );
      MAST_TRACE( seq, 0, "PROGRESS: seq:%llu; seq_leaf:%llu OF %llu", H_PDI->seq, H_PDI->seq_leaf, m );
    }
  }
}

/* 20151027.144501 */
/* void ( *duf_rsccbh_fun_t )
 *                ( const struct duf_sccb_handle_s *, duf_stmnt_t *, duf_scanstage_t, duf_scanner_t, duf_node_type_t, int ) */
static void
duf_sccbh_atom_cb( const struct duf_sccb_handle_s *sccbh MAS_UNUSED, duf_stmnt_t * pstmt MAS_UNUSED, duf_scanstage_t scanstage MAS_UNUSED,
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
/* duf_sccb_handle_t *                                                                                                      */
/* duf_sccb_handle_open( duf_depthinfo_t * pdi, const duf_scan_callbacks_t * sccb, int targc, char *const *targv, int *pr ) */
SRP( SCCBH, duf_sccb_handle_t *, sccbh, NULL, sccb_handle_open, duf_depthinfo_t * pdi, const duf_scan_callbacks_t * const *psccb,
     const mas_cargvc_t * ptarg )
{
/* duf_sccb_handle_t *sccbh = NULL; */
/* int rpr = 0; */

/* assert( pdi->pyp ); */
  if ( *psccb && pdi && duf_levinfo_dirid( pdi ) )
  {
    MAST_TRACE( fs, 2, "set def. opendir: %d", ( *psccb )->def_opendir );
    duf_pdi_set_opendir( pdi, ( *psccb )->def_opendir );

    assert( pdi );
    assert( pdi->pathinfo.levinfo );
    assert( duf_levinfo_dirid( pdi ) );
    assert( duf_levinfo_path( pdi ) );
    sccbh = duf_sccb_handle_create( psccb, ptarg );

#if 0
    H_SCCBA = psccb;
# if 0
    H_SCCB = ( *psccb );
# else
    assert( H_SCCB == ( *psccb ) );
# endif
    H_PARGC = targc;
    H_PARGV = targv;
#endif
#if 0
    H_PU = pu;
#endif
#if 1
    H_PDI = duf_pdi_clone( pdi, 0 /* no_li */  );
    assert( H_PDI->pathinfo.levinfo );
    H_PDICLONED = 1;
#else
    H_PDI = pdi;
#endif
    H_PDI->total_bytes = 0;
    H_PDI->total_files = 0;
    if ( H_SCCB->beginning_sql_seq )
      H_PDI->set_selected_db = H_SCCB->beginning_sql_seq->set_selected_db;
  /* duf_scan_qbeginning_sql( sccb ); */
    MAST_TRACE( sql, 1, "@@beginning_sql for '%s'", H_SCCB->title );

    CR( sccbh_eval_sqlsq, sccbh /* , H_PU */  );
    MAST_TRACE( sql, 1, "@@/beginning_sql for '%s'", H_SCCB->title );
    if ( QNOERR )
    {
      if ( !H_SCCB->no_count )
      {
        H_TOTITEMS = duf_count_total_items( sccbh, QPERRIND );       /* reference */
        if ( QNOERR )
          H_TOTCOUNTED = 1;
      }
    /* T( "H_TOTCOUNTED:%d; H_TOTITEMS:%llu for %s", H_TOTCOUNTED, H_TOTITEMS, duf_uni_scan_action_title( H_SCCB ) ); */
      MAST_TRACE( temporary, 0, "counted for %s... %lld", H_SCCB->title, H_TOTITEMS );
/* total_files for progress bar only :( */
    /* assert(H_TOTITEMS=38); */
      DUF_SCCB( MAST_TRACE, action, 0, "total_items: %llu", H_TOTITEMS );
      MAST_TRACE( temporary, 0, "@@@@ %llu items registered in db", H_TOTITEMS );
      MAST_TRACE( explain, 10, "%llu items registered in db", H_TOTITEMS );
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
    if ( QNOERR )
    {
      if ( H_SCCB->init_scan )
      {
        MAST_TRACE( explain, 10, "to init scan" );
        CRV( H_SCCB->init_scan, NULL /* pstmt */ , pdi, sccbh );
      }
      else
      {
        MAST_TRACE( explain, 10, "no init scan" );
      }
      assert( H_PDI->pathinfo.levinfo );
    /* T(">>> %llu : %llu", H_PU->std_leaf_set_num,  H_PU->std_node_set_num); */
      CR( pdi_reinit_anypath, H_PDI, duf_levinfo_path( H_PDI ), duf_pdi_pu( H_PDI ),
          duf_sccbh_get_sql_set_f( sccbh, DUF_NODE_NODE ).active, 0 /* caninsert */ ,
          duf_pdi_recursive( H_PDI ), duf_pdi_allow_dirs( H_PDI ), duf_pdi_linear( H_PDI ) );
    }
  }
  else
  {
    QT( "sccb:%d; dirid:%llu - %s", H_SCCB ? 1 : 0, duf_levinfo_dirid( pdi ), duf_levinfo_path( pdi ) );
  /* assert(0); */
  }
/* if ( pr )     */
/*   *pr = rpr;  */
/* return sccbh; */
  ERP( SCCBH, duf_sccb_handle_t *, sccbh, NULL, sccb_handle_open, duf_depthinfo_t * pdi, const duf_scan_callbacks_t * const *psccb,
       const mas_cargvc_t * ptarg );
}

SR( SCCBH, sccb_handle_close, duf_sccb_handle_t * sccbh )
{
/* DUF_STARTR( r ); */
  if ( sccbh )
  {
  /* final */
    MAST_TRACE( scan, 6, "final sql %s", H_SCCB->title );
    duf_sccb_row_delete( sccbh->row );
    duf_sccb_row_delete( sccbh->previous_row );

    for ( H_SCCBI = 0; H_SCCB; H_SCCBI++ )
    {
      CR( sccb_eval_final_sqlsq, H_SCCB, ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL );
    }
    H_SCCBI = 0;

    if ( H_PDICLONED )
      duf_pdi_delete( H_PDI );
    mas_free( sccbh );
  }
/* DUF_ENDR( r ); */
  ER( SCCBH, sccb_handle_close, duf_sccb_handle_t * sccbh );
}
