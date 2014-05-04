/* #include <stdio.h> */
/* #include <stdlib.h> */
/* #include <stdarg.h> */
#include <string.h>
/* #include <unistd.h> */
/* #include <sys/stat.h> */

/* #include <sys/types.h> */
/* #include <fcntl.h> */
/* #include <unistd.h> */
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"


#include "duf_sql_field.h"
#include "duf_sql1.h"

#include "duf_path.h"

#include "duf_dir_scan.h"

#include "duf_dbg.h"


/* ###################################################################### */
#include "duf_uni_scan.h"
/* ###################################################################### */


/*
 * run   --uni-scan /home/mastar/a/down/ --max-depth=4  --max-items=70 -R --tree --print  --files
 * run   --uni-scan /home/mastar/a/down/ --max-depth=4  --max-items=70 -R --tree --print
*/


/* duf_str_cb_uni_scan_dir:
 * this is callback of type: duf_str_cb_t (second range):
 *
 * if recursive flag set, call duf_scan_dirs_by_parentid + pdi (built from str_cb_udata) with duf_str_cb_uni_scan_dir
 *       for each <dir> record by dirid (i.e. children of dirid) with corresponding args 
 * otherwise do nothing
 *
 *   i.e.
 *     1. for <current> dir call sccb->node_scan_before
 *     2. for each leaf in <current> dir call sccb->leaf_scan
 *     3. for <current> dir call sccb->node_scan_middle
 *   recursively from <current> dir (if recursive flag set):
 *     4. for each dir in <current> dir call duf_str_cb_uni_scan_dir + &di as str_cb_udata
 *     5. for <current> dir call sccb->node_scan_after
 * */
int
duf_str_cb_uni_scan_dir( void *str_cb_udata, duf_depthinfo_t * xpdi, duf_scan_callbacks_t * sccb, duf_record_t * precord )
{
  int r = 0;
  duf_depthinfo_t *pdi;

  pdi = ( duf_depthinfo_t * ) str_cb_udata;
  /* DUF_UFIELD( dirid ); */
  /* assert( dirid == duf_levinfo_dirid( pdi ) ); */
  /* DUF_SFIELD( dfname ); */
  DEBUG_START(  );

  assert( pdi );
  DUF_OINV_OPENED( pdi-> );


  if ( r >= 0 )
  {
    if ( pdi->u.recursive && ( !pdi->u.maxreldepth || duf_pdi_reldepth( pdi ) < pdi->u.maxreldepth ) )
    {
/* duf_scan_fil_by_pi:
 * call duf_str_cb_uni_scan_dir + pdi (also) as str_cb_udata for each <dir> record by dirid (i.e. children of dirid) with corresponding args
 *
 * i.e. if recursive, call duf_scan_dirs_by_parentid + pdi (built from str_cb_udata) with duf_str_cb_uni_scan_dir
 *       for each <dir> record by dirid (i.e. children of dirid) with corresponding args 
 *         otherwise do nothing
 *
 *   i.e.
 *     1. for <current> dir call sccb->node_scan_before
 *     2. for each leaf in <current> dir call sccb->leaf_scan
 *     3. for <current> dir call sccb->node_scan_middle
 *   recursively from <current> dir (if recursive flag set):
 *     4. for each dir in <current> dir call str_cb + str_cb_udata
 *     5. for <current> dir call sccb->node_scan_after
 * */

      DUF_OINV_OPENED( pdi-> );
      r = duf_scan_dirs_by_parentid( duf_str_cb_uni_scan_dir, pdi, sccb, precord );
      DUF_OINV_OPENED( pdi-> );
      /* if ( r == DUF_ERROR_MAX_REACHED )                   */
      /* {                                                   */
      /*   if ( pdi->depth == 0 )                            */
      /*     DUF_TRACE( action, 0, "Maximum reached ...." ); */
      /* }                                                   */
      /* else if ( r < 0 )                                   */
      /* {                                                   */
      /*   DUF_ERROR( "r=%d", r );                           */
      /* }                                                   */
    }
  }
  DUF_TEST_R( r );
  DUF_OINV_OPENED( pdi-> );
  DEBUG_END(  );
  return r;
}

int
duf_str_cb2_uni_scan_dir( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = 0;

  DEBUG_START(  );

  assert( pdi );
  DUF_OINV_OPENED( pdi-> );

  if ( r >= 0 )
  {
    if ( pdi->u.recursive && ( !pdi->u.maxreldepth || duf_pdi_reldepth( pdi ) < pdi->u.maxreldepth ) )
    {
/* duf_scan_fil_by_pi:
 * call duf_str_cb_uni_scan_dir + pdi (also) as str_cb_udata for each <dir> record by dirid (i.e. children of dirid) with corresponding args
 *
 * i.e. if recursive, call duf_scan_dirs_by_parentid + pdi (built from str_cb_udata) with duf_str_cb_uni_scan_dir
 *       for each <dir> record by dirid (i.e. children of dirid) with corresponding args 
 *         otherwise do nothing
 *
 *   i.e.
 *     1. for <current> dir call sccb->node_scan_before
 *     2. for each leaf in <current> dir call sccb->leaf_scan
 *     3. for <current> dir call sccb->node_scan_middle
 *   recursively from <current> dir (if recursive flag set):
 *     4. for each dir in <current> dir call str_cb + str_cb_udata
 *     5. for <current> dir call sccb->node_scan_after
 * */

      DUF_OINV_OPENED( pdi-> );
      r = duf_scan_dirs_by_parentid2( pstmt, duf_str_cb2_uni_scan_dir, pdi, sccb );
      DUF_OINV_OPENED( pdi-> );
      /* if ( r == DUF_ERROR_MAX_REACHED )                   */
      /* {                                                   */
      /*   if ( pdi->depth == 0 )                            */
      /*     DUF_TRACE( action, 0, "Maximum reached ...." ); */
      /* }                                                   */
      /* else if ( r < 0 )                                   */
      /* {                                                   */
      /*   DUF_ERROR( "r=%d", r );                           */
      /* }                                                   */
    }
  }
  DUF_TEST_R( r );
  DUF_OINV_OPENED( pdi-> );
  DEBUG_END(  );
  return r;
}

/*
 *   i.e. 
 *     1. for <current> dir call sccb->node_scan_before
 *     2. for each leaf in <current> dir call sccb->leaf_scan
 *     3. for <current> dir call sccb->node_scan_middle
 *   recursively from <current> dir (if recursive flag set):
 *     4. for each dir in <current> dir call str_cb + str_cb_udata
 *     5. for <current> dir call sccb->node_scan_after
 */
int
duf_uni_scan( const char *path, duf_ufilter_t u, duf_scan_callbacks_t * sccb )
{
  int r = 0;

  DEBUG_START(  );


  DUF_TRACE( action, 0, "%" DUF_ACTION_TITLE_FMT ": sccb        %c", duf_uni_scan_action_title( sccb ), sccb ? '+' : '-' );
  if ( sccb )
  {
    duf_depthinfo_t di = {.depth = -1,
      .seq = 0,
      .levinfo = NULL,
      .u = u,
      .opendir = sccb ? sccb->opendir : 0,
      /* .name = path, */
    };

    DEBUG_STEP(  );

    DUF_OINV( di. );



/* create level-control array, open 0 level */
    r = duf_levinfo_create( &di, path );
    DUF_TRACE( action, 0, "%" DUF_ACTION_TITLE_FMT ": di.levinfo  %c", duf_uni_scan_action_title( sccb ), di.levinfo ? '+' : '-' );
    DUF_OINV( di. );

    {
      unsigned long long top_dirid;

      top_dirid = duf_path_to_pathid( path, &di, &r );
      assert( top_dirid == duf_levinfo_dirid( &di ) );
      DUF_TEST_R( r );
      DUF_TRACE( action, 0, "%" DUF_ACTION_TITLE_FMT ":top_dirid %llu for %s", duf_uni_scan_action_title( sccb ), top_dirid, path );
      DUF_TEST_R( r );

      if ( r >= 0 && ( top_dirid || !path ) )
      {
        DUF_OINV_OPENED( di. );
        DUF_OINV( di. );
        DUF_TRACE( scan, 5, "%llu:%s  duf_scan_dirs_by_parentid(2?) with str_cb=duf_str_cb_uni_scan_dir(%p)", top_dirid,
                   path, ( void * ) ( unsigned long long ) duf_str_cb_uni_scan_dir );
/* duf_str_cb_uni_scan_dir:
 * if recursive, call duf_scan_dirs_by_parentid + pdi (built from str_cb_udata)
 *       for each <dir> record by top_dirid (i.e. children of top_dirid) with corresponding args 
 * otherwise do nothing
 *
 * call duf_str_cb_uni_scan_dir with pdi for each dir at db by top_dirid (i.e. children of top_dirid) 
 *
 *   i.e.
 *     1. for <current> dir call sccb->node_scan_before
 *     2. for each leaf in <current> dir call sccb->leaf_scan
 *     3. for <current> dir call sccb->node_scan_middle
 *   recursively from <current> dir (if recursive flag set):
 *     4. for each dir in <current> dir call duf_str_cb_uni_scan_dir + &di as str_cb_udata
 *     5. for <current> dir call sccb->node_scan_after
 * */
        DUF_OINV( di. );
        DUF_OINV_OPENED( di. );

        if ( r >= 0 )
        {
          if ( sccb->scan_mode_step )
            r = duf_scan_dirs_by_parentid2( ( duf_sqlite_stmt_t * ) NULL, duf_str_cb2_uni_scan_dir, &di, sccb );
          else
            r = duf_scan_dirs_by_parentid( duf_str_cb_uni_scan_dir, &di, sccb, ( duf_record_t * ) NULL /* precord */  );
        }
        DUF_OINV( di. );
        DUF_TEST_R( r );

        /* if ( r == DUF_ERROR_MAX_REACHED )                 */
        /*   DUF_TRACE( action, 0, "Maximum reached ...." ); */
        /* else if ( r < 0 )                                 */
        /*   DUF_ERROR( "code: %d", r );                     */
      }
/* delete level-control array, close 0 level */
      DUF_OINV( di. );
      duf_levinfo_delete( &di );
      DUF_OINV( di. );
      /* if ( r == DUF_ERROR_MAX_REACHED )                 */
      /*   DUF_TRACE( action, 0, "Maximum reached ...." ); */
      /* else if ( r < 0 )                                 */
      /*   DUF_ERROR( "code: %d", r );                     */
    }

    if ( duf_config->cli.act.totals )
    {
      DUF_PRINTF( 0, " seq:%llu", di.seq );
      if ( duf_config->u.maxseq )
        DUF_PRINTF( 0, " of %llu", duf_config->u.maxseq );
      DUF_PRINTF( 0, "\n" );
    }

    /* if ( !duf_config->cli.noopenat ) */
    /*   duf_close_dh( pdh );           */
    /* pdh->dfd = 0;                    */
    DUF_TRACE( action, 0, "%" DUF_ACTION_TITLE_FMT ": end scan ; totals:%d", duf_uni_scan_action_title( sccb ),
               duf_config->cli.act.totals );
  }
  /* if ( r == DUF_ERROR_MAX_REACHED )                 */
  /*   DUF_TRACE( action, 0, "Maximum reached ...." ); */
  /* else if ( r < 0 )                                 */
  /*   DUF_ERROR( "code: %d", r );                     */
  DUF_TEST_R( r );
  DEBUG_END(  );
  return r;
}

int
duf_uni_scan_targ( duf_scan_callbacks_t * sccb )
{
  int r = 0;

  DEBUG_START(  );



  if ( duf_config )
  {
    if ( r >= 0 && sccb && sccb->init_scan )
      r = sccb->init_scan( sccb );
    DUF_TEST_R( r );
    DUF_TRACE( action, 0, "%" DUF_ACTION_TITLE_FMT ": inited scan", duf_uni_scan_action_title( sccb ) );
    if ( r >= 0
         && ( sccb->node_scan_before || sccb->node_scan_middle || sccb->node_scan_after || sccb->leaf_scan || sccb->leaf_scan_fd
              || sccb->entry_file_scan_before || sccb->entry_dir_scan_before ) )
    {
      DUF_TRACE( action, 0, "%" DUF_ACTION_TITLE_FMT ": targc:%u", duf_uni_scan_action_title( sccb ), duf_config->targc );
      for ( int ia = 0; r >= 0 && ia < duf_config->targc; ia++ )
        DUF_TRACE( action, 0, "%" DUF_ACTION_TITLE_FMT ": targv[%d]='%s'", duf_uni_scan_action_title( sccb ), ia, duf_config->targv[ia] );
      if ( duf_config->targc > 0 )
        for ( int ia = 0; r >= 0 && ia < duf_config->targc; ia++ )
          r = duf_uni_scan( duf_config->targv[ia], duf_config->u, sccb );
      else
        r = duf_uni_scan( NULL, duf_config->u, sccb );
      DUF_TEST_R( r );
    }
    if ( sccb )
    {
      char **psql = sccb->final_sql_argv;

      if ( psql )
      {
        while ( r >= 0 && psql && *psql )
        {
          int changes = 0;

          /* DUF_TRACE( action, 0, "final psql : %s", *p ); */
          /* r = duf_sql( *p, &changes ); */
          r = duf_sql_exec( *psql, &changes );
          DUF_TEST_R( r );
          /* DUF_TRACE( action, 0, "(%d) final psql %s; changes:%d", r, *psql, changes ); */
          DUF_TRACE( action, 0, "%" DUF_ACTION_TITLE_FMT ": final SQL %lu; [%s] changes:%d; %s", duf_uni_scan_action_title( sccb ),
                     psql - sccb->final_sql_argv, *psql, changes, r < 0 ? "FAIL" : "OK" );
          psql++;
        }
      }
    }
  }
  DEBUG_END(  );
  return r;
}

int
duf_uni_scan_all( void )
{
  int r = 0;
  duf_scan_callbacks_t **ppscan_callbacks;
  int max_asteps = 100;
  int asteps = 0;


  DEBUG_START(  );

  DUF_TRACE( action, 0, "prep" );
  ppscan_callbacks = mas_malloc( max_asteps * sizeof( duf_scan_callbacks_t * ) );

  assert( asteps < max_asteps );
  if ( duf_config->cli.act.integrity )
  {
    extern duf_scan_callbacks_t duf_integrity_callbacks /* __attribute( ( weak ) ) */ ;

    DUF_TRACE( action, 0, "prep integrity ..." );
    ppscan_callbacks[asteps++] = &duf_integrity_callbacks;
  }
  assert( asteps < max_asteps );
  {
    extern duf_scan_callbacks_t duf_collect_openat_callbacks __attribute( ( weak ) );
    extern duf_scan_callbacks_t duf_collect_noopenat_callbacks __attribute( ( weak ) );

    if ( &duf_collect_openat_callbacks && !duf_config->cli.noopenat && ( duf_config->cli.act.collect && duf_config->cli.act.dirent ) )
    {
      DUF_TRACE( action, 0, "prep collect ..." );
      ppscan_callbacks[asteps++] = &duf_collect_openat_callbacks;
    }
    else if ( &duf_collect_noopenat_callbacks && duf_config->cli.noopenat && ( duf_config->cli.act.collect && duf_config->cli.act.dirent ) )
    {
      DUF_TRACE( action, 0, "prep collect ..." );
      ppscan_callbacks[asteps++] = &duf_collect_noopenat_callbacks;
    }
#ifdef DUF_COMPILE_EXPIRED
    else if ( ( ( duf_config->cli.act.collect && duf_config->cli.act.dirent ) ) )
    {
      DUF_TRACE( action, 0, "prep fill ..." );
      extern duf_scan_callbacks_t duf_fill_callbacks /* __attribute( ( weak ) ) */ ;

      ppscan_callbacks[asteps++] = &duf_fill_callbacks;
    }
#endif
  }
  assert( asteps < max_asteps );
  {
    extern duf_scan_callbacks_t duf_collect_openat_md5_callbacks __attribute( ( weak ) );
    extern duf_scan_callbacks_t duf_collect_noopenat_md5_callbacks __attribute( ( weak ) );

    if ( &duf_collect_openat_md5_callbacks && !duf_config->cli.noopenat && duf_config->cli.act.md5 && ( duf_config->cli.act.collect ) )
    {

      DUF_TRACE( action, 0, "prep fill md5" );
      ppscan_callbacks[asteps++] = &duf_collect_openat_md5_callbacks;
    }
    else if ( &duf_collect_noopenat_md5_callbacks && duf_config->cli.noopenat && duf_config->cli.act.md5
              && ( duf_config->cli.act.collect ) )
    {
      DUF_TRACE( action, 0, "prep fill md5" );
      ppscan_callbacks[asteps++] = &duf_collect_noopenat_md5_callbacks;
    }
#ifdef DUF_COMPILE_EXPIRED
    else if ( duf_config->cli.act.md5 && ( duf_config->cli.act.collect ) )
    {
      extern duf_scan_callbacks_t duf_fill_md5_callbacks /* __attribute( ( weak ) ) */ ;

      DUF_TRACE( action, 0, "prep fill md5" );
      ppscan_callbacks[asteps++] = &duf_fill_md5_callbacks;
    }
#endif
  }
  assert( asteps < max_asteps );
  if ( duf_config->cli.act.mdpath && ( duf_config->cli.act.collect ) )
  {
    extern duf_scan_callbacks_t duf_collect_mdpath_callbacks /* __attribute( ( weak ) ) */ ;

    DUF_TRACE( action, 0, "prep mdpath" );
    ppscan_callbacks[asteps++] = &duf_collect_mdpath_callbacks;
  }
  assert( asteps < max_asteps );
  if ( duf_config->cli.act.md5 && duf_config->cli.act.print )
  {
    extern duf_scan_callbacks_t duf_print_md5_callbacks /* __attribute( ( weak ) ) */ ;

    DUF_TRACE( action, 0, "prep print md5" );
    ppscan_callbacks[asteps++] = &duf_print_md5_callbacks;
  }

  assert( asteps < max_asteps );
  if ( duf_config->cli.act.print && duf_config->cli.act.tree && !duf_config->cli.act.md5 )
  {
    extern duf_scan_callbacks_t duf_print_tree_callbacks /* __attribute( ( weak ) ) */ ;

    DUF_TRACE( action, 0, "prep print tree" );
    ppscan_callbacks[asteps++] = &duf_print_tree_callbacks;
  }

  assert( asteps < max_asteps );
  if ( duf_config->cli.act.print && !duf_config->cli.act.tree && !duf_config->cli.act.md5 )
  {
    extern duf_scan_callbacks_t duf_print_dir_callbacks /* __attribute( ( weak ) ) */ ;

    ppscan_callbacks[asteps++] = &duf_print_dir_callbacks;
  }
  if ( duf_config->cli.act.sample )
  {
    extern duf_scan_callbacks_t duf_sample_callbacks /* __attribute( ( weak ) ) */ ;

    assert( asteps + duf_config->cli.act.sample < max_asteps );
    for ( int i = 0; i < duf_config->cli.act.sample && asteps < max_asteps; i++ )
    {
      ppscan_callbacks[asteps++] = &duf_sample_callbacks;
    }
  }

  assert( asteps < max_asteps );
  if ( duf_config->cli.act.sampupd )
  {
    extern duf_scan_callbacks_t duf_sampupd_callbacks /* __attribute( ( weak ) ) */ ;

    ppscan_callbacks[asteps++] = &duf_sampupd_callbacks;
  }
  if ( asteps )
    DUF_TRACE( action, 0, "%d actions set; %s", asteps, r < 0 ? "FAIL" : "" );
  else
    DUF_TRACE( action, 0, "no actions set; %s", r < 0 ? "FAIL" : "" );
  for ( int astep = 0; r >= 0 && astep < asteps; astep++ )
  {
    if ( ppscan_callbacks[astep] )
    {
      DUF_TRACE( action, 0, "%" DUF_ACTION_TITLE_FMT ": astep %d", duf_uni_scan_action_title( ppscan_callbacks[astep] ), astep );
      r = duf_uni_scan_targ( ppscan_callbacks[astep] );
    }
  }
  /* if ( r == DUF_ERROR_MAX_REACHED )                 */
  /*   DUF_TRACE( action, 0, "Maximum reached ...." ); */
  /* else if ( r < 0 )                                 */
  /*   DUF_ERROR( "code: %d", r );                     */
  mas_free( ppscan_callbacks );
  DEBUG_ENDR( r );
  return r;
}