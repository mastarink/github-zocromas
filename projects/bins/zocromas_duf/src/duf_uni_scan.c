#define DUF_SQL_PDI_STMT

#include <assert.h>
/* #include <libgen.h> */


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"


#include "duf_utils.h"          /* duf_strfgmtime */
#include "duf_utils_path.h"     /* duf_realpath */

#include "duf_config_ref.h"
#include "duf_status_ref.h"

#include "duf_levinfo_ref.h"

#include "duf_pdi.h"

#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_sccb.h"
#ifdef MAS_SCCBHANDLE
#  include "duf_sccb_handle.h"
#  include "duf_sccbh_scan.h"
#else
#  include "duf_sccb_begfin.h"
#endif

/* #include "duf_dir_scan1.h" */
#include "duf_dir_scan2.h"


/* #include "duf_uni_scan1.h" */
/* #include "duf_uni_scan2.h" */
#include "duf_path2db.h"

#include "duf_option_names.h"
#include "duf_option_defs.h"
#include "duf_prepare_actions.h"

/* #include "duf_interactive.h" */

/* ###################################################################### */
#include "duf_uni_scan.h"
/* ###################################################################### */


/*
 * run   --uni-scan /home/mastar/a/down/ --max-depth=4  --max-items=70 -R --tree --print  --files
 * run   --uni-scan /home/mastar/a/down/ --max-depth=4  --max-items=70 -R --tree --print
*/

#ifndef MAS_SCCBHANDLE
static unsigned long long
duf_count_total_items( duf_scan_callbacks_t * sccb, int *pr )
{
  DEBUG_STARTULL( cnt );
  int r = DUF_ERROR_TOTALS;

  if ( sccb && sccb->leaf.selector_total2 )
  {
    char *sqlt;

    r = 0;
    sqlt = mas_strdup( "SELECT " );
    sqlt = mas_strcat_x( sqlt, "COUNT(*) AS nf" );
    sqlt = mas_strcat_x( sqlt, " " );
    sqlt = mas_strcat_x( sqlt, sccb->leaf.selector_total2 );
    {
      const char *csql;

      csql = sqlt;
      DUF_SQL_START_STMT_NOPDI( csql, r, pstmt );
      DUF_SQL_STEP( r, pstmt );
      if ( r == DUF_SQL_ROW )
      {
        cnt = duf_sql_column_long_long( pstmt, 0 );
        r = 0;
      }
      DUF_SQL_END_STMT_NOPDI( r, pstmt );
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
#endif

/*
 *   i.e.
 *     1. for <current> dir call sccb->node_scan_before
 *     2. for each leaf in <current> dir call sccb->leaf_scan
 *     3. for <current> dir call sccb->node_scan_middle
 *   recursively from <current> dir (if recursive flag set):
 *     4. for each dir in <current> dir call str_cb + str_cb_udata
 *     5. for <current> dir call sccb->node_scan_after
 */
#ifndef MAS_SCCBHANDLE
static int
duf_sccb_real_path( const char *real_path, duf_ufilter_t * pu, duf_scan_callbacks_t * sccb /*, unsigned long long *pchanges */  )
{
  DEBUG_STARTR( r );

  duf_depthinfo_t *pdi = NULL;

  /* duf_depthinfo_t di = {.depth = -1,         */
  /*   .seq = 0,                                */
  /*   .levinfo = NULL,                         */
  /*   .u = *pu,                                */
  /*   .opendir = sccb ? sccb->def_opendir : 0, */
  /*   (* .name = real_path, *)                 */
  /* };                                         */
  /*                                            */
  /* pdi=&di;                                   */
  pdi = duf_config->pdi;

  DEBUG_STEP(  );

  /* assert( di.depth == -1 ); */
  DOR( r, duf_pdi_reinit( pdi, real_path, pu, DUF_U_FLAG( recursive ) ) );
  DUF_TRACE( scan, 0, "[%llu] #%llu start scan from pdi path: ≪%s≫;", duf_levinfo_dirid( pdi ), pdi->seq_leaf, duf_levinfo_path( pdi ) );
  DOR( r, duf_sccb_pdi( SCCBX ) );
  /* (* xchanges = di.changes; --- needless!? *) */
  /* duf_pdi_close( &di );                       */

  DEBUG_ENDR( r );
}
#endif
/*
 * - evaluate sccb for given path 
 * - store (add!) number of changes to *pchanges
 * */
#ifndef MAS_SCCBHANDLE
static int
duf_sccb_path( const char *path, duf_ufilter_t * pu, duf_scan_callbacks_t * sccb /*, unsigned long long *pchanges */  )
{
  DEBUG_STARTR( r );
  char *real_path = NULL;

  real_path = duf_realpath( path, &r );

  DUF_TRACE( explain, 0, "start scan from path: ≪%s≫; real: ≪%s≫", path, real_path );

  DUF_E_NO( DUF_ERROR_MAX_REACHED, DUF_ERROR_MAX_SEQ_REACHED, DUF_ERROR_TOO_DEEP );
  if ( sccb )
    DOR( r, duf_sccb_real_path( real_path, pu, sccb /*, pchanges */  ) );

  mas_free( real_path );
  DEBUG_ENDR_YES_CLEAR( r, DUF_ERROR_MAX_REACHED, DUF_ERROR_MAX_SEQ_REACHED, DUF_ERROR_TOO_DEEP );
}
#endif

/*
 * split from Duf_evaluate_sccb  20140901.214205
 * last function revision ...
 *
 * - evaluate sccb for each string from duf_config->targ[cv] as path
 * - store number of changes to *pchanges
 *
 * 
 * <<>> for each targv do sccb_path <<>>
 *
 * */
#ifndef MAS_SCCBHANDLE
static int
duf_sccb_each_targv( duf_scan_callbacks_t * sccb /*, unsigned long long *pchanges */  )
{
  DEBUG_STARTR( r );
  /* unsigned long long changes = 0; */
  global_status.changes = 0;
  DUF_TRACE( action, 1, "%" DUF_ACTION_TITLE_FMT ": targc:%u", duf_uni_scan_action_title( sccb ), duf_config->targc );
  for ( int ia = 0; r >= 0 && ia < duf_config->targc; ia++ )
    DUF_TRACE( action, 1, "%" DUF_ACTION_TITLE_FMT ": targv[%d]='%s'", duf_uni_scan_action_title( sccb ), ia, duf_config->targv[ia] );

  if ( duf_config->targc <= 0 )
  {
    /* - evaluate sccb for NULL path */
    DOR( r, duf_sccb_path( NULL, duf_config->pu, sccb /*, &changes */  ) );
  }
  else
  {
    /* - evaluate sccb for each string from duf_config->targ[cv] as path */
    for ( int ia = 0; r >= 0 && ia < duf_config->targc; ia++ )
      DOR( r, duf_sccb_path( duf_config->targv[ia], duf_config->pu, sccb /*, &changes */  ) );
  }

  DUF_TRACE( action, 1, "after scan" );
  if ( DUF_ACT_FLAG( summary ) )
    DUF_PRINTF( 0, " summary; changes:%llu", global_status.changes );
  DEBUG_ENDR( r );
}
#endif

/*
 * make/evaluate sccb
 * ******************
 * 1. «open» sccb handle - incorporate argv to the handle - by calling duf_open_sccb_handle
 * 2. ?? call duf_count_total_items
 * 3. call sccb->init_scan
 * 4. casll duf_sccbh_each_path
 * 5. «close» sccb handle - by calling duf_close_sccb_handle
 *   ** description assumes MAS_SCCBHANDLE is defined
 * */
int
duf_make_sccb( duf_scan_callbacks_t * sccb )
{
  DEBUG_STARTR( r );

#ifdef MAS_SCCBHANDLE
  duf_sccb_handle_t *sccbh = NULL;

  sccbh = duf_open_sccb_handle( duf_config->pdi, sccb, duf_config->targc, duf_config->targv, duf_config->pu );
#else
  {
    {
      int rt = 0;
      unsigned long long total_files;

      total_files = duf_count_total_items( sccb, &rt ); /* reference */
      if ( rt >= 0 )
        global_status.total_files = total_files;
/* total_files for progress bar only :( */
      DUF_SCCB( DUF_TRACE, action, 0, "total_files: %llu", total_files );
      DUF_TRACE( explain, 0, "%llu files registered in db", total_files );
    }

/*
TODO scan mode
  1. direct, like now
  2. place ID's to temporary table, then scan in certain order
*/
    if ( sccb->init_scan )
    {
      DUF_TRACE( explain, 0, "to init scan" );
      DUF_DO_TEST_R( r, sccb->init_scan(  ) );
    }
    else
    {
      DUF_TRACE( explain, 0, "no init scan" );
    }
  }
#endif
  /* DOR( r, duf_scan_beginning_sql( sccb ) ); */

  DUF_TRACE( explain, 0, "scan targ; action title: %s", duf_uni_scan_action_title( sccb ) );
  DUF_TRACE( action, 1, "%" DUF_ACTION_TITLE_FMT ": inited scan", duf_uni_scan_action_title( sccb ) );

  /* - evaluate sccb for each string from duf_config->targ[cv] as path
   * - store number of changes to *pchanges */
#ifdef MAS_SCCBHANDLE
  DOR( r, duf_sccbh_each_path( sccbh ) );
  if ( DUF_ACT_FLAG( summary ) )
    DUF_PRINTF( 0, " summary; changes:%llu", sccbh->changes );
  duf_close_sccb_handle( sccbh );
#else
  DOR( r, duf_sccb_each_targv( sccb ) );
  DOR( r, duf_scan_final_sql( sccb /*, changes */  ) );
#endif

  DEBUG_ENDR( r );
}

int
duf_show_sccb_sequence( duf_scan_callbacks_t ** sccb_sequence, int sccb_num )
{
  DEBUG_STARTR( r );
  DUF_PRINTF( 0, "SCCBS" );
  for ( int astep = 0; r >= 0 && astep < sccb_num; astep++ )
  {
    if ( sccb_sequence[astep] )
    {
      DUF_PRINTF( 0, "%d. %s", astep, sccb_sequence[astep]->title );
    }
  }
  DEBUG_ENDR( r );
}

/*
 * for each sccb from sequence/list (with given size - # of items) make/evaluate the task
 *    by calling duf_make_sccb
 * */
int
duf_make_sccb_sequence( duf_scan_callbacks_t ** sccb_sequence, int sccb_num, int *pcnt )
{
  DEBUG_STARTR( r );

  int cnt = 0;

  for ( int astep = 0; r >= 0 && astep < sccb_num; astep++ )
  {
    if ( sccb_sequence[astep] )
    {
      DUF_TRACE( action, 2, "%" DUF_ACTION_TITLE_FMT ": astep %d", duf_uni_scan_action_title( sccb_sequence[astep] ), astep );

      /* duf_make_sccb:
       * do everything needed from sccb, i.e. :
       * - call init  from sccb_sequence[astep]
       * - do beginning sql set  from sccb_sequence[astep]
       * - via duf_sccb_each_targv:
       *     - evaluate sccb_sequence[astep] for each string  from duf_config->targ[cv] as path
       *     - store number of changes to *pchanges
       * - do final sql set from sccb_sequence[astep]
       * XXX  XXX */ DOR( r, duf_make_sccb( sccb_sequence[astep] /* sccb */  ) );
      cnt++;
    }
  }
  if ( pcnt )
    *pcnt += cnt;
  DEBUG_ENDR( r );
}

/* make/evaluate «duf_config» configured tasks
 *       - global variable duf_config must be created/inited and set
 * *******************************************
 * 1. prepare sccb's i.e. "convert" «duf_config» representation of tasks/actions
 *             into «sccb» sequence/list (duf_scan_callbacks_t) by calling duf_set_actions
 * 2. prepare «sample» sccb's by calling duf_set_actions_sample
 * 3. make/evaluate sccb sequence/list by calling duf_make_sccb_sequence
 * */
int
duf_make_all_sccbs( void )
{
  DEBUG_STARTR( r );
  int max_asteps = 210;
  int asteps = 0;
  duf_scan_callbacks_t **ppscan_callbacks;

  /* memory for sccb's */
  ppscan_callbacks = mas_malloc( max_asteps * sizeof( duf_scan_callbacks_t * ) );

  /* prepare sccb's : "convert" «duf_config» representation of tasks/actions into «sccb» list (duf_scan_callbacks_t) */
  asteps += duf_set_actions( ppscan_callbacks + asteps, max_asteps - asteps );
  /* prepare sccb's 2 */
  asteps += duf_set_actions_sample( ppscan_callbacks + asteps, max_asteps - asteps );
#if 0
  for ( int i = 0; i < asteps; i++ )
  {
    T( "########### %d. %p", i, ppscan_callbacks[i] );
  }
#endif
  if ( asteps )
    DUF_TRACE( action, 0, "%d actions set; %s", asteps, r < 0 ? "FAIL" : "" );
/* <body> */
  if ( DUF_ACT_FLAG( show_sccbs ) )
    DOR( r, duf_show_sccb_sequence( ppscan_callbacks, asteps ) );
  if ( DUF_ACT_FLAG( do_sccbs ) )
    DOR( r, duf_make_sccb_sequence( ppscan_callbacks, asteps, &global_status.actions_done ) );
/* </body> */

  mas_free( ppscan_callbacks );
  if ( !global_status.actions_done )
    DOR( r, DUF_ERROR_NO_ACTIONS );
  DEBUG_ENDR( r );
}

/*
 * split from duf_make_all_sccbs  20140901.213001
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
int
duf_make_all_sccbs_wrap( void )
{
  DEBUG_STARTR( r );

  DUF_TRACE( action, 1, "prep" );
  DUF_TRACE( explain, 0, "scan all; setting actions" );
  DUF_TRACE( explain, 0, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" );

  DOR( r, duf_make_all_sccbs(  ) );


  if ( r == DUF_ERROR_NO_ACTIONS )
  {
    char *optnames = NULL;
    char *dirent_optnames = NULL;

    DUF_TRACE( action, 0, "no actions set; %s", r < 0 ? "FAIL" : "" );
    optnames = duf_option_names( DUF_OPTION_FLAG_COLLECT );
    dirent_optnames = duf_option_names( DUF_OPTION_FLAG_DIRENT );
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

  /* if ( r == DUF_ERROR_MAX_REACHED )                 */
  /*   DUF_TRACE( action, 0, "Maximum reached ...." ); */
  /* else if ( r < 0 )                                 */
  /*   DUF_SHOW_ERROR( "code: %d", r );                     */
  DEBUG_ENDR( r );
}
