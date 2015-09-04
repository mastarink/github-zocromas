#define DUF_SQL_PDI_STMT

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_status_ref.h"

#include "duf_sccb_eval.h"

#include "duf_option_names.h"
#include "duf_option_defs.h"
#include "duf_prepare_actions.h"


/* duf_eval_at_config.[ch] is former duf_uni_scan.[ch] */
/* ###################################################################### */
#include "duf_eval_at_config.h"
/* ###################################################################### */



/* make/evaluate «duf_config» configured tasks
 *       - global variable duf_config must be created/inited and set
 * *******************************************
 * 1. prepare sccb's i.e. "convert" «duf_config» representation of tasks/actions
 *             into «sccb» sequence/list (duf_scan_callbacks_t) by calling duf_set_actions
 * 2. prepare «sample» sccb's by calling duf_set_actions_sample
 * 3. make/evaluate sccb sequence/list by calling duf_evaluate_sccb_array
 * */
/* TODO with new interface duf_evaluate_all_at_config is needless; remove also corresponding options */
DUF_WRAPSTATIC int
duf_evaluate_all_at_config( void )
{
  DEBUG_STARTR( r );
  int max_asteps = 210;
  int asteps = 0;
  duf_scan_callbacks_t **ppscan_callbacks;

  /* memory for sccb's */
  ppscan_callbacks = mas_malloc( max_asteps * sizeof( duf_scan_callbacks_t * ) );

  /* prepare sccb's : "convert" «duf_config» representation of tasks/actions into «sccb» list (duf_scan_callbacks_t) */
  asteps += duf_config2sccb_vector( ppscan_callbacks + asteps, max_asteps - asteps );
  /* prepare sccb's 2 */
  asteps += duf_config2sccb_vector_sample( ppscan_callbacks + asteps, max_asteps - asteps );

  if ( asteps )
    DUF_TRACE( action, 0, "%d actions set; %s", asteps, r < 0 ? "FAIL" : "" );
/* <body> */
#if 0
  if ( DUF_ACTG_FLAG( show_sccbs ) )
    DOR( r, duf_show_sccb_sequence( ppscan_callbacks, asteps ) );
#endif
  if ( DUF_ACTG_FLAG( do_sccbs ) )
    DOR( r, duf_evaluate_sccb_array( ppscan_callbacks, asteps, &global_status.actions_done ) );
/* </body> */

  mas_free( ppscan_callbacks );
  if ( !global_status.actions_done )
    DUF_MAKE_ERROR( r, DUF_ERROR_NO_ACTIONS );
  DEBUG_ENDR( r );
}

/*
 * split from duf_evaluate_all_at_config  20140901.213001
 * last function revision ...
 *
 * for each sccb at modules matching preset (options etc.) conditions:
 *   do everything needed from sccb, i.e. :
 *     - call init  from ppscan_callbacks[astep]
 *     - do beginning sql set  from ppscan_callbacks[astep]
 *     - via duf_sccb_each_targv:
 *         - evaluate ppscan_callbacks[astep] for each string  from duf_config->targ[cv] as path
 *     - do final sql set from ppscan_callbacks[astep]
 * */
#ifdef MAS_WRAP_FUNC
int
duf_evaluate_all_at_config_wrap( void )
{
  DEBUG_STARTR( r );

  DUF_TRACE( action, 1, "prep" );
  DUF_TRACE( explain, 0, "scan all; setting actions" );
  DUF_TRACE( explain, 0, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" );

  DOR( r, duf_evaluate_all_at_config(  ) );


  if ( DUF_IS_ERROR( r, DUF_ERROR_NO_ACTIONS ) )
  {
    char *optnames = NULL;
    char *dirent_optnames = NULL;

    DUF_TRACE( action, 0, "no actions set; %s", r < 0 ? "FAIL" : "" );
    optnames = duf_option_names( DUF_OPTION_VAL_FLAG_COLLECT );
    dirent_optnames = duf_option_names( DUF_OPTION_VAL_FLAG_DIRENT );
    DUF_TRACE( explain, 0, "to collect something   use %s", optnames );
    DUF_TRACE( explain, 0, "to collect directories use %s WITH %s AND %s", optnames, DUF_OPT_NAME2( FLAG_DIRS ), dirent_optnames );
    DUF_TRACE( explain, 0, "to collect file data   use %s WITH %s AND %s", optnames, DUF_OPT_NAME2( FLAG_FILEDATA ), dirent_optnames );
    DUF_TRACE( explain, 0, "tO collect file names  use %s WITH %s AND %s", optnames, DUF_OPT_NAME2( FLAG_FILENAMES ), dirent_optnames );
    DUF_TRACE( explain, 0, "to collect md5 names   use %s WITH %s AND %s", optnames, DUF_OPT_NAME2( FLAG_MD5 ), dirent_optnames );
    DUF_TRACE( explain, 0, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" );
    DUF_PUTSL( 0 );
    DUF_PUTSL( 0 );
    mas_free( dirent_optnames );
    mas_free( optnames );
  }

  /* if ( DUF_IS_ERROR( r, DUF_ERROR_MAX_REACHED ) )   */
  /*   DUF_TRACE( action, 0, "Maximum reached ...." ); */
  /* else if ( r < 0 )                                 */
  /*   DUF_SHOW_ERROR( "code: %d", r );                */
  DEBUG_ENDR( r );
}
#endif
