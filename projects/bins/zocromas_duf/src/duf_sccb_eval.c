#define DUF_SQL_PDI_STMT

#include <assert.h>
#include <string.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"


#include "duf_config_ref.h"

#include "duf_sccb.h"
#include "duf_sccb_eval_dirs.h"
#include "duf_sccb_handle.h"

#include "duf_levinfo_ref.h"

#include "duf_option_defs.h"

#include "duf_sccbh_shortcuts.h"
/* ###################################################################### */
#include "duf_sccb_eval.h"
/* ###################################################################### */

/* 20150819.164652 */
static int
duf_eval_sccbh_all_and_summary( duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );

  DUF_E_NO( DUF_ERROR_TOO_DEEP );
  assert( sccbh );
  assert( SCCB );

  DUF_TRACE( scan, 2, "scan a+s by %5llu:%s; %s", duf_levinfo_dirid( PDI ), duf_uni_scan_action_title( SCCB ), duf_levinfo_path( PDI ) );

  DORF( r, DUF_WRAPPED( duf_eval_sccbh_all ), ( duf_sqlite_stmt_t * ) NULL, sccbh ); /* XXX XXX XXX XXX XXX XXX */

  if ( r >= 0 && DUF_ACT_FLAG( summary ) )
  {
    DUF_PRINTF( 0, "%s", duf_uni_scan_action_title( SCCB ) );

    DUF_PRINTF( 0, " summary; seq:     %llu", PDI->seq );
    DUF_PRINTF( 0, " summary; seq-leaf:%llu", PDI->seq_leaf );
    DUF_PRINTF( 0, " summary; seq-node:%llu", PDI->seq_node );
    if ( PU->max_seq )
      DUF_PRINTF( 0, " of %llu (max-seq)", PU->max_seq );
    if ( DUF_ACT_FLAG( summary ) )
      DUF_PRINTF( 0, " summary; changes:%llu", HCHANGES );
  }
  DUF_TRACE( scan, 2, "/scan a+s by %5llu:%s; %s", duf_levinfo_dirid( PDI ), duf_uni_scan_action_title( SCCB ), duf_levinfo_path( PDI ) );
  DUF_E_YES( DUF_ERROR_TOO_DEEP );
  DEBUG_ENDR( r );
}

#ifdef MAS_WRAP_FUNC
/* 20150819.164643 */
static int DUF_WRAPPED( duf_eval_sccbh_all_and_summary ) ( duf_sccb_handle_t * sccbh ) /* duf_eval_sccbh_all */
{
  DEBUG_STARTR( r );
  if ( sccbh && SCCB )
  {
    /* DOR( r, duf_scan_beginning_sql( sccb ) ); */

    DUF_TRACE( explain, 0, "scan targ; action title: %s", duf_uni_scan_action_title( SCCB ) );
    DUF_TRACE( action, 1, "%" DUF_ACTION_TITLE_FMT ": inited scan", duf_uni_scan_action_title( SCCB ) );

    {
      HCHANGES = 0;
      /* - evaluate sccb for each string from duf_config->targ[cv] as path
       * - store number of changes to *pchanges */
#  if 0
      DOR( r, duf_sccbh_eval_each_path( sccbh ) );
#  else
      if ( duf_levinfo_path( PDI ) )
        DOR( r, duf_eval_sccbh_all_and_summary( sccbh ) ); /* XXX XXX XXX XXX XXX XXX */
#  endif
    }
  }
  else
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_SCCB );
  }
  DEBUG_ENDR( r );
}
#endif
/*
 * make/evaluate sccb
 * ******************
 * 1. «open» sccb handle - incorporate argv to the handle - by calling duf_open_sccb_handle
 * 2. ?? call duf_count_total_items
 * 3. call sccb->init_scan
 * 4. calls duf_sccbh_eval_each_path
 * 5. «close» sccb handle - by calling duf_close_sccb_handle
 * */
int
duf_evaluate_sccb( duf_scan_callbacks_t * sccb )
{
  DEBUG_STARTR( r );


  DUF_TRACE( path, 0, "@ (to open sccbh) levinfo_path: %s", duf_levinfo_path( duf_config->pdi ) );
  {
    duf_sccb_handle_t *sccbh = NULL;

    sccbh = duf_open_sccb_handle( duf_config->pdi, sccb, duf_config->targ.argc, duf_config->targ.argv, duf_config->pu, &r );

    DOR( r, DUF_WRAPPED( duf_eval_sccbh_all_and_summary ) ( sccbh ) ); /* XXX XXX XXX XXX XXX XXX */

    duf_close_sccb_handle( sccbh );
  }
  DEBUG_ENDR( r );
}

const duf_action_table_t *
duf_find_sccb_by_name( const char *name, size_t namelen, const duf_action_table_t * table )
{
  for ( const duf_action_table_t * act = table; act->sccb; act++ )
  {
    if ( 0 == strncmp( name, act->sccb->name, namelen ) )
      return act;
  }
  return NULL;
}

int
duf_evaluate_sccb_named_list( const char *names, const duf_action_table_t * table )
{
  DEBUG_STARTR( r );

  int ok = 0;
  const char *pnames;

  pnames = names;
  DUF_TRACE( path, 0, "@levinfo_path: %s", duf_levinfo_path( duf_config->pdi ) );
  while ( r >= 0 && pnames && *pnames )
  {
    size_t len = 0;
    const char *ename = NULL;
    const duf_action_table_t *act;

    ename = strchr( pnames, ',' );
    if ( ename )
      len = ename - pnames;
    else
      len = strlen( pnames );
    if ( r >= 0 )
      act = duf_find_sccb_by_name( pnames, len, table ); /* XXX XXX */
    if ( act )
    {
      DUF_TRACE( path, 0, "@ (to duf_evaluate_sccb) [%s] levinfo_path: %s", act->sccb->name, duf_levinfo_path( duf_config->pdi ) );


      DOR( r, duf_evaluate_sccb( act->sccb ) ); /* XXX XXX XXX XXX */
      ok++;
    }
    /* DUF_PRINTF( 0, "pnames:%s; ename:%s", pnames, ename ); */
    if ( ename && *ename )
      pnames = ename + 1;
    else
      pnames = NULL;
    ename = NULL;
  }
  if ( !ok )
  {
    DUF_SHOW_ERROR( "sccb not found: %s", names );
  }
  DEBUG_ENDR( r );
}

#if 0
static int
duf_show_sccb_sequence( duf_scan_callbacks_t ** psccbs, int sccb_num )
{
  DEBUG_STARTR( r );
  DUF_PRINTF( 0, "SCCBS" );
  for ( int astep = 0; r >= 0 && astep < sccb_num; astep++ )
  {
    if ( psccbs[astep] )
    {
      DUF_PRINTF( 0, "%d. %s", astep, psccbs[astep]->title );
    }
  }
  DEBUG_ENDR( r );
}
#endif
/*
 * for each sccb from sequence/list (with given size - # of items) make/evaluate the task
 *    by calling duf_evaluate_sccb
 * */
int
duf_evaluate_sccb_array( duf_scan_callbacks_t ** psccbs, int sccb_num, int *pcnt )
{
  DEBUG_STARTR( r );

  int cnt = 0;

  DUF_TRACE( path, 0, "@levinfo_path: %s", duf_levinfo_path( duf_config->pdi ) );

  for ( int astep = 0; r >= 0 && astep < sccb_num; astep++ )
  {
    if ( psccbs[astep] )
    {
      DUF_TRACE( action, 2, "%" DUF_ACTION_TITLE_FMT ": astep %d", duf_uni_scan_action_title( psccbs[astep] ), astep );

      /* duf_evaluate_sccb:
       * do everything needed from sccb, i.e. :
       * - call init  from psccbs[astep]
       * - do beginning sql set  from psccbs[astep]
       * - via duf_sccb_each_targv:
       *     - evaluate psccbs[astep] for each string  from duf_config->targ[cv] as path
       *     - store number of changes to *pchanges
       * - do final sql set from psccbs[astep]
       * XXX  XXX */ DOR( r, duf_evaluate_sccb( psccbs[astep] /* sccb */  ) );
      cnt++;
    }
  }
  if ( pcnt )
    *pcnt += cnt;
  DEBUG_ENDR( r );
}
