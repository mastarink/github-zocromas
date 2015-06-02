#define DUF_SQL_PDI_STMT

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"


#include "duf_config_ref.h"

#include "duf_sccb.h"
#include "duf_sccb_handle.h"
#include "duf_sccbh_scan.h"

#include "duf_option_names.h"
#include "duf_option_defs.h"


#include "duf_sccbh_shortcuts.h"
/* ###################################################################### */
#include "duf_sccb_eval.h"
/* ###################################################################### */


static int
duf_evaluate_sccb_handle( duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
  if ( sccbh && SCCB )
  {
    /* DOR( r, duf_scan_beginning_sql( sccb ) ); */

    DUF_TRACE( explain, 0, "scan targ; action title: %s", duf_uni_scan_action_title( SCCB ) );
    DUF_TRACE( action, 1, "%" DUF_ACTION_TITLE_FMT ": inited scan", duf_uni_scan_action_title( SCCB ) );

    if ( !( TARGC && TARGV ) )
    {
    }
    {
      /* - evaluate sccb for each string from duf_config->targ[cv] as path
       * - store number of changes to *pchanges */
      DOR( r, duf_sccbh_eval_each_path( sccbh ) );
      if ( DUF_ACT_FLAG( summary ) )
        DUF_PRINTF( 0, " summary; changes:%llu", HCHANGES );
    }
  }
  else
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_SCCB );
  }
  DEBUG_ENDR( r );
}

/*
 * make/evaluate sccb
 * ******************
 * 1. «open» sccb handle - incorporate argv to the handle - by calling duf_open_sccb_handle
 * 2. ?? call duf_count_total_items
 * 3. call sccb->init_scan
 * 4. casll duf_sccbh_eval_each_path
 * 5. «close» sccb handle - by calling duf_close_sccb_handle
 * */
int
duf_evaluate_sccb( duf_scan_callbacks_t * sccb )
{
  DEBUG_STARTR( r );

  duf_sccb_handle_t *sccbh = NULL;

  sccbh = duf_open_sccb_handle( duf_config->pdi, sccb, duf_config->targ.argc, duf_config->targ.argv, duf_config->pu );
  DOR( r, duf_evaluate_sccb_handle( sccbh ) );
  duf_close_sccb_handle( sccbh );

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
