#define DUF_SQL_PDI_STMT

#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_trace_defs.h"
#include "duf_debug_defs.h"


#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"

#include "duf_options.h"
/* #include "duf_cli_options.h" */


#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_path.h"

#include "duf_sccb.h"
#include "duf_dir_scan1.h"
#include "duf_dir_scan2.h"

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
  assert( pdi );
  /* DUF_UFIELD( dirid ); */
  /* assert( dirid == duf_levinfo_dirid( pdi ) ); */
  /* DUF_SFIELD( dfname ); */
  DEBUG_START(  );

  DUF_TRACE( scan, 0, "+" );
  if ( r >= 0 && DUF_U_FLAG( recursive ) && ( !pdi->u.maxreldepth || duf_pdi_reldepth( pdi ) < pdi->u.maxreldepth ) )
  {
    if ( duf_pdi_reldepth( pdi ) <= 1 )
    {
      DUF_TRACE( explain, 2, "cb; option %s; option %s value %d; depth %d",
                 duf_option_cnames( DUF_OPTION_FLAG_RECURSIVE ), duf_option_cnames( DUF_OPTION_MAXDEPTH ), pdi->u.maxreldepth,
                 duf_pdi_reldepth( pdi ) );
    }
    else
    {
      DUF_TRACE( explain, 3, "cb; option %s; option %s value %d; depth %d",
                 duf_option_cnames( DUF_OPTION_FLAG_RECURSIVE ), duf_option_cnames( DUF_OPTION_MAXDEPTH ), pdi->u.maxreldepth,
                 duf_pdi_reldepth( pdi ) );
    }
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
    DUF_TEST_R( r );
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option or depth condition by %s value %d", duf_option_cnames( DUF_OPTION_FLAG_RECURSIVE ),
               duf_option_cnames( DUF_OPTION_MAXDEPTH ), pdi->u.maxreldepth );
  }

  DEBUG_END(  );
  return r;
}

int
duf_str_cb2_uni_scan_dir( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = 0;

  DEBUG_START(  );

  assert( pdi );

  DUF_TRACE( scan, 0, "+" );
  if ( r >= 0 )
  {
    if ( DUF_U_FLAG( recursive ) && ( !pdi->u.maxreldepth || duf_pdi_reldepth( pdi ) < pdi->u.maxreldepth ) )
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
      if ( duf_pdi_reldepth( pdi ) <= 1 )
      {
        DUF_TRACE( explain, 3, "cb2; option %s; option %s value %d; depth %d",
                   duf_option_cnames( DUF_OPTION_FLAG_RECURSIVE ), duf_option_cnames( DUF_OPTION_MAXDEPTH ), pdi->u.maxreldepth,
                   duf_pdi_reldepth( pdi ) );
      }
      else
      {
        DUF_TRACE( explain, 4, "cb2; option %s; option %s value %d; depth %d",
                   duf_option_cnames( DUF_OPTION_FLAG_RECURSIVE ), duf_option_cnames( DUF_OPTION_MAXDEPTH ), pdi->u.maxreldepth,
                   duf_pdi_reldepth( pdi ) );
      }

      DUF_OINV_OPENED( pdi-> );
      DUF_TRACE( scan, 0, "+" );
      r = duf_scan_dirs_by_parentid2( pstmt, duf_str_cb2_uni_scan_dir, pdi, sccb );
    }
    else
    {
      DUF_TRACE( explain, 1, "no %s option or depth condition by %s value %d", duf_option_cnames( DUF_OPTION_FLAG_RECURSIVE ),
                 duf_option_cnames( DUF_OPTION_MAXDEPTH ), pdi->u.maxreldepth );
    }

  }
  DUF_TEST_R( r );
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
static int
duf_uni_scan_from_path( const char *path, duf_ufilter_t * pu, duf_scan_callbacks_t * sccb, unsigned long long *pchanges )
{
  int r = 0;
  char *real_path = NULL;

  DEBUG_START(  );

  real_path = duf_realpath( path );
  DUF_TRACE( explain, 0, "start scan from path: ≪%s≫; real: ≪%s≫", path, real_path );

  DUF_TRACE( action, 1, "%" DUF_ACTION_TITLE_FMT ": sccb        %c", duf_uni_scan_action_title( sccb ), sccb ? '+' : '-' );
  DUF_TRACE( scan, 0, "uni" );
  if ( sccb )
  {
    duf_depthinfo_t di = {.depth = -1,
      .seq = 0,
      .levinfo = NULL,
      .u = *pu,
      .opendir = sccb ? sccb->opendir : 0,
      /* .name = real_path, */
    };

    DEBUG_STEP(  );

    r = duf_pdi_init( &di, real_path );
/* create level-control array, open 0 level */
    DUF_TRACE( action, 0, "%" DUF_ACTION_TITLE_FMT ": di.levinfo  %c", duf_uni_scan_action_title( sccb ), di.levinfo ? '+' : '-' );

    if ( sccb->leaf_selector_total2 )
    {
      char *sqlt;
      const char *csql;

      sqlt = mas_strdup( "SELECT " );
      sqlt = mas_strcat_x( sqlt, "count(*) AS nf" );
      sqlt = mas_strcat_x( sqlt, " " );
      sqlt = mas_strcat_x( sqlt, sccb->leaf_selector_total2 );
      csql = sqlt;
      DUF_SQL_START_STMT_NOPDI( csql, r, pstmt );
      DUF_SQL_STEP( r, pstmt );
      if ( r == DUF_SQL_ROW )
      {
        di.total_files = duf_sql_column_long_long( pstmt, 0 );
        r = 0;
      }
      DUF_SQL_END_STMT_NOPDI( r, pstmt );
      mas_free( sqlt );
    }
    /* DUF_ERROR( "TOTAL FILES:%llu; r:%d", di.total_files, r ); */
    DUF_TRACE( explain, 0, "%llu files registered in db", di.total_files );
    if ( r >= 0 )
    {
      r = duf_real_path2db( &di, real_path, 0 /* ifadd */  );
      if ( r == DUF_ERROR_NOT_IN_DB )
        DUF_ERROR( "not in db:'%s' (%s)", real_path ? real_path : "-", path ? path : "-" );
      DUF_TEST_R( r );
      if ( r >= 0 )
      {
        DUF_TRACE( path, 0, "top dirid:%llu for %s", duf_levinfo_dirid( &di ), real_path );
        /* assert( top dirid == duf_levinfo_dirid( &di ) ); */
        DUF_TRACE( scan, 0, "top dirid:%llu for %s", duf_levinfo_dirid( &di ), real_path );
        DUF_TEST_R( r );

        DUF_TRACE( action, 0, "%" DUF_ACTION_TITLE_FMT ": top dirid %llu for %s", duf_uni_scan_action_title( sccb ),
                   duf_levinfo_dirid( &di ), real_path );

        DUF_TRACE( explain, 0,
                   "≫≫≫≫≫≫≫≫≫≫ %" "s" /* DUF_ACTION_TITLE_FMT */ " ≪≪≪≪≪≪≪≪≪≪≪≪≪≪≪≪≪",
                   duf_uni_scan_action_title( sccb ) );


        /* DUF_ERROR( "L%d", di.depth ); */
        DUF_OINV_OPENED( di. );
        assert( di.depth >= 0 );
        DUF_OINV( di. );
        DUF_TRACE( scan, 5, "%llu:%s  duf_scan_dirs_by_parentid(2?) with str_cb=duf_str_cb_uni_scan_dir(%p)", duf_levinfo_dirid( &di ),
                   real_path, ( void * ) ( unsigned long long ) duf_str_cb_uni_scan_dir );


/* duf_str_cb_uni_scan_dir:
 * if recursive, call duf_scan_dirs_by_parentid + pdi (built from str_cb_udata)
 *       for each <dir> record by top dirid (i.e. children of top dirid) with corresponding args 
 * otherwise do nothing
 *
 * call duf_str_cb_uni_scan_dir with pdi for each dir at db by top dirid (i.e. children of top dirid) 
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
          DUF_TRACE( scan, 2, "before by_parentid" );
          DUF_TRACE( explain, 0, "to scan" );

          if ( sccb->scan_mode_2 )
            r = duf_scan_dirs_by_parentid2( ( duf_sqlite_stmt_t * ) NULL, duf_str_cb2_uni_scan_dir, &di, sccb );
          else
            r = duf_scan_dirs_by_parentid( duf_str_cb_uni_scan_dir, &di, sccb, ( duf_record_t * ) NULL /* precord */  );
        }
        DUF_OINV( di. );
        DUF_TEST_R( r );
      }
      else
      {
        DUF_TRACE( scan, 0, "? top dirid:%llu; real_path:'%s'", duf_levinfo_dirid( &di ), real_path );
      }
/* delete level-control array, close 0 level */
      if ( pchanges )
        *pchanges += di.changes;
      duf_pdi_close( &di );
    }
    if ( DUF_ACT_FLAG( summary ) )
    {
      DUF_PRINTF( 0, " summary; seq:     %llu", di.seq );
      DUF_PRINTF( 0, " summary; seq-leaf:%llu", di.seq_leaf );
      DUF_PRINTF( 0, " summary; seq-node:%llu", di.seq_node );
      if ( duf_config->u.maxseq )
        DUF_PRINTF( 0, " of %llu", duf_config->u.maxseq );
    }
    DUF_TRACE( action, 0, "%" DUF_ACTION_TITLE_FMT ": end scan ; summary:%d", duf_uni_scan_action_title( sccb ), DUF_ACT_FLAG( summary ) );
  }
  else
  {
    DUF_TRACE( scan, 0, "?" );
  }
  mas_free( real_path );
  DUF_TEST_R( r );

  DUF_TRACE( action, 0, "%" DUF_ACTION_TITLE_FMT ": end scan ; summary:%d", duf_uni_scan_action_title( sccb ), DUF_ACT_FLAG( summary ) );
  DEBUG_END(  );
  return r;
}

static int
duf_uni_scan_targ( duf_scan_callbacks_t * sccb )
{
  int r = 0;
  unsigned long long changes = 0;

  DEBUG_START(  );
/*
TODO scan mode
  1. direct, like now
  2. place id's to temporary table, then scan in certain order
*/
  if ( duf_config && sccb )
  {
    int do1;
    int do2;

    if ( r >= 0 && sccb->init_scan )
    {
      DUF_TRACE( explain, 0, "to init scan" );
      r = sccb->init_scan(  );
    }
    else
    {
      DUF_TRACE( explain, 0, "no init scan" );
    }
    DUF_TRACE( explain, 0, "scan targ; action title: %s", duf_uni_scan_action_title( sccb ) );
    DUF_TEST_R( r );
    DUF_TRACE( action, 0, "%" DUF_ACTION_TITLE_FMT ": inited scan", duf_uni_scan_action_title( sccb ) );

    do1 = ( !sccb->scan_mode_2 && ( sccb->node_scan_before /* */
                                    || sccb->node_scan_middle /* */
                                    || sccb->node_scan_after /* */
                                    || sccb->leaf_scan /* */
                                    || sccb->leaf_scan_fd /* */
                                    || sccb->dirent_file_scan_before /* */
                                    || sccb->dirent_dir_scan_before /* */
             ) );
    do2 = ( sccb->scan_mode_2 && ( sccb->node_scan_before2 /* */
                                   || sccb->node_scan_middle2 /* */
                                   || sccb->node_scan_after2 /* */
                                   || sccb->leaf_scan2 /* */
                                   || sccb->leaf_scan_fd2 /* */
                                   || sccb->dirent_file_scan_before2 /* */
                                   || sccb->dirent_dir_scan_before2 /* */
             ) );
    if ( r >= 0 && ( do2 || do1 ) )
    {
      DUF_TRACE( action, 0, "%" DUF_ACTION_TITLE_FMT ": targc:%u", duf_uni_scan_action_title( sccb ), duf_config->targc );
      for ( int ia = 0; r >= 0 && ia < duf_config->targc; ia++ )
        DUF_TRACE( action, 0, "%" DUF_ACTION_TITLE_FMT ": targv[%d]='%s'", duf_uni_scan_action_title( sccb ), ia, duf_config->targv[ia] );
      if ( duf_config->targc > 0 )
        for ( int ia = 0; r >= 0 && ia < duf_config->targc; ia++ )
          r = duf_uni_scan_from_path( duf_config->targv[ia], &duf_config->u, sccb, &changes );
      else
        r = duf_uni_scan_from_path( NULL, &duf_config->u, sccb, &changes );
      DUF_TEST_R( r );

      DUF_TRACE( action, 0, "after scan" );
    }
    else
    {
      DUF_TRACE( action, 0, "nothing defined for %s", duf_uni_scan_action_title( sccb ) );
    }

    if ( DUF_ACT_FLAG( summary ) )
      DUF_PRINTF( 0, " summary; changes:%llu", changes );

    /* if ( changes ) */
    {
      const char **psql = sccb->final_sql_argv;

      while ( r >= 0 && psql && *psql )
      {
        int changes = 0;

        DUF_TRACE( action, 0, "final psql : %s", *psql );
        /* r = duf_sql( *p, &changes ); */

        {
          DUF_SQL_START_STMT_NOPDI( *psql, r, pstmt );
          /* r = duf_sql_exec( *psql, &changes ); */
          DUF_SQL_STEP( r, pstmt );
          DUF_SQL_CHANGES_NOPDI( changes, r, pstmt );
          DUF_SQL_END_STMT_NOPDI( r, pstmt );
        }
        DUF_TEST_R( r );
        /* DUF_TRACE( action, 0, "(%d) final psql %s; changes:%d", r, *psql, changes ); */
        DUF_TRACE( action, 0, "%" DUF_ACTION_TITLE_FMT ": final SQL %lu; [%s] changes:%d; %s", duf_uni_scan_action_title( sccb ),
                   psql - sccb->final_sql_argv, *psql, changes, r < 0 ? "FAIL" : "OK" );
        psql++;
      }
    }
  }
  DEBUG_ENDR( r );
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
  DUF_TRACE( explain, 0, "scan all; setting actions" );
  ppscan_callbacks = mas_malloc( max_asteps * sizeof( duf_scan_callbacks_t * ) );

  assert( asteps < max_asteps );
  if ( DUF_ACT_FLAG( integrity ) )
  {
    extern duf_scan_callbacks_t duf_integrity_callbacks /* __attribute( ( weak ) ) */ ;

    DUF_TRACE( explain, 0, "     option %s", duf_option_cnames( DUF_OPTION_FLAG_INTEGRITY ) );
    DUF_TRACE( action, 0, "prep integrity ..." );
    DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪integrity≫ #%d", asteps );
    ppscan_callbacks[asteps++] = &duf_integrity_callbacks;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option", duf_option_cnames( DUF_OPTION_FLAG_INTEGRITY ) );
  }
  assert( asteps < max_asteps );
  if ( DUF_ACT_FLAG( collect ) )
  {
    DUF_TRACE( explain, 0, "     option %s", duf_option_cnames( DUF_OPTION_FLAG_COLLECT ) );
    if ( DUF_ACT_FLAG( dirent ) )
    {
      DUF_TRACE( explain, 0, "     option %s", duf_option_cnames( DUF_OPTION_FLAG_DIRENT ) );
      {
        extern duf_scan_callbacks_t duf_directories_callbacks __attribute( ( weak ) );

        if ( &duf_directories_callbacks && DUF_ACT_FLAG( dirs ) )
        {
          DUF_TRACE( explain, 0, "     option %s", duf_option_cnames( DUF_OPTION_FLAG_DIRS ) );
          DUF_TRACE( action, 0, "prep directories ..." );
          DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪directories≫ #%d", asteps );
          ppscan_callbacks[asteps++] = &duf_directories_callbacks;
        }
        else
        {
          DUF_TRACE( explain, 1, "no %s option or no callback", duf_option_cnames( DUF_OPTION_FLAG_DIRS ) );
        }
      }
      assert( asteps < max_asteps );
      {
        extern duf_scan_callbacks_t duf_filedata_callbacks __attribute( ( weak ) );


        if ( &duf_filedata_callbacks && DUF_ACT_FLAG( filedata ) )
        {
          DUF_TRACE( explain, 0, "     option %s", duf_option_cnames( DUF_OPTION_FLAG_FILEDATA ) );
          DUF_TRACE( action, 0, "prep filedata ..." );
          DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪filedata≫ #%d", asteps );
          ppscan_callbacks[asteps++] = &duf_filedata_callbacks;
        }
        else
        {
          DUF_TRACE( explain, 1, "no %s option or no callback", duf_option_cnames( DUF_OPTION_FLAG_FILEDATA ) );
        }
      }
      assert( asteps < max_asteps );
      {
        extern duf_scan_callbacks_t duf_filenames_callbacks __attribute( ( weak ) );


        if ( &duf_filenames_callbacks && DUF_ACT_FLAG( filenames ) )
        {
          DUF_TRACE( explain, 0, "     option %s", duf_option_cnames( DUF_OPTION_FLAG_FILENAMES ) );
          DUF_TRACE( action, 0, "prep filenames ..." );
          DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪filenames≫ #%d", asteps );
          ppscan_callbacks[asteps++] = &duf_filenames_callbacks;
        }
        else
        {
          DUF_TRACE( explain, 1, "no %s option or no callback", duf_option_cnames( DUF_OPTION_FLAG_FILENAMES ) );
        }
      }
    }
    assert( asteps < max_asteps );
    {
      extern duf_scan_callbacks_t duf_collect_openat_crc32_callbacks __attribute( ( weak ) );


      if ( &duf_collect_openat_crc32_callbacks && DUF_ACT_FLAG( crc32 ) )
      {
        DUF_TRACE( explain, 0, "     option %s", duf_option_cnames( DUF_OPTION_FLAG_CRC32 ) );
        DUF_TRACE( action, 0, "prep fill crc32" );
        DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪collect_openat_crc32≫ #%d", asteps );
        ppscan_callbacks[asteps++] = &duf_collect_openat_crc32_callbacks;
      }
      else
      {
        DUF_TRACE( explain, 1, "no %s option or no callback", duf_option_cnames( DUF_OPTION_FLAG_CRC32 ) );
      }
    }
    assert( asteps < max_asteps );
    {
      extern duf_scan_callbacks_t duf_collect_openat_sd5_callbacks __attribute( ( weak ) );


      if ( &duf_collect_openat_sd5_callbacks && DUF_ACT_FLAG( sd5 ) )
      {
        DUF_TRACE( explain, 0, "     option %s", duf_option_cnames( DUF_OPTION_FLAG_SD5 ) );
        DUF_TRACE( action, 0, "prep fill sd5" );
        DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪collect_openat_sd5≫ #%d", asteps );
        ppscan_callbacks[asteps++] = &duf_collect_openat_sd5_callbacks;
      }
      else
      {
        DUF_TRACE( explain, 1, "no %s option or no callback", duf_option_cnames( DUF_OPTION_FLAG_SD5 ) );
      }
    }
    assert( asteps < max_asteps );
    {
      extern duf_scan_callbacks_t duf_collect_openat_md5_callbacks __attribute( ( weak ) );


      if ( &duf_collect_openat_md5_callbacks && DUF_ACT_FLAG( md5 ) )
      {
        DUF_TRACE( explain, 0, "     option %s", duf_option_cnames( DUF_OPTION_FLAG_MD5 ) );
        DUF_TRACE( action, 0, "prep fill md5" );
        DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪collect_openat_md5≫ #%d", asteps );
        ppscan_callbacks[asteps++] = &duf_collect_openat_md5_callbacks;
      }
      else
      {
        DUF_TRACE( explain, 1, "no %s option or no callback", duf_option_cnames( DUF_OPTION_FLAG_MD5 ) );
      }
    }
    assert( asteps < max_asteps );
    {
      extern duf_scan_callbacks_t duf_collect_mime_callbacks __attribute( ( weak ) );

      if ( &duf_collect_mime_callbacks && DUF_ACT_FLAG( mime ) )
      {
        DUF_TRACE( explain, 0, "     option %s", duf_option_cnames( DUF_OPTION_FLAG_MIME ) );
        DUF_TRACE( action, 0, "prep fill mime" );
        DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪collect_mime≫ #%d", asteps );
        ppscan_callbacks[asteps++] = &duf_collect_mime_callbacks;
      }
      else
      {
        DUF_TRACE( explain, 1, "no %s option or no callback", duf_option_cnames( DUF_OPTION_FLAG_MIME ) );
      }
    }
    assert( asteps < max_asteps );
    {
      extern duf_scan_callbacks_t duf_collect_exif_callbacks __attribute( ( weak ) );

      if ( &duf_collect_exif_callbacks && DUF_ACT_FLAG( exif ) )
      {
        DUF_TRACE( explain, 0, "     option %s", duf_option_cnames( DUF_OPTION_FLAG_EXIF ) );
        DUF_TRACE( action, 0, "prep fill exif" );
        DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪collect_exif≫ #%d", asteps );
        ppscan_callbacks[asteps++] = &duf_collect_exif_callbacks;
      }
      else
      {
        DUF_TRACE( explain, 1, "no %s option or no callback", duf_option_cnames( DUF_OPTION_FLAG_EXIF ) );
      }
    }
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option", duf_option_cnames( DUF_OPTION_FLAG_COLLECT ) );
  }

  assert( asteps < max_asteps );
  if (  /* DUF_ACT_FLAG(update )&& */ DUF_ACT_FLAG( mdpath ) )
  {
    extern duf_scan_callbacks_t duf_collect_mdpath_callbacks /* __attribute( ( weak ) ) */ ;

    DUF_TRACE( explain, 0, "     option %s", duf_option_cnames( DUF_OPTION_FLAG_MDPATH ) );
    DUF_TRACE( action, 0, "prep mdpath" );
    DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪collect_mdpath≫ #%d", asteps );
    ppscan_callbacks[asteps++] = &duf_collect_mdpath_callbacks;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option", duf_option_cnames( DUF_OPTION_FLAG_MDPATH ) );
  }
  assert( asteps < max_asteps );
  if ( DUF_ACT_FLAG( print ) )
  {
    DUF_TRACE( explain, 0, "     option %s", duf_option_cnames( DUF_OPTION_FLAG_PRINT ) );
    if ( DUF_ACT_FLAG( md5 ) )
    {
      extern duf_scan_callbacks_t duf_print_md5_callbacks /* __attribute( ( weak ) ) */ ;

      DUF_TRACE( explain, 0, "     option %s", duf_option_cnames( DUF_OPTION_FLAG_MD5 ) );
      DUF_TRACE( action, 0, "prep print md5" );
      DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪print_md5≫ #%d", asteps );
      ppscan_callbacks[asteps++] = &duf_print_md5_callbacks;
    }
    else
    {
      assert( asteps < max_asteps );
      DUF_TRACE( explain, 1, "no %s option", duf_option_cnames( DUF_OPTION_FLAG_MD5 ) );
      if ( DUF_ACT_FLAG( tree ) )
      {
        extern duf_scan_callbacks_t duf_print_tree_callbacks /* __attribute( ( weak ) ) */ ;

        DUF_TRACE( explain, 0, "     option %s", duf_option_cnames( DUF_OPTION_FLAG_TREE ) );
        DUF_TRACE( action, 0, "prep print tree" );
        DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪print_tree≫ #%d", asteps );
        ppscan_callbacks[asteps++] = &duf_print_tree_callbacks;
      }
      else
      {
        assert( asteps < max_asteps );
        extern duf_scan_callbacks_t duf_print_dir_callbacks /* __attribute( ( weak ) ) */ ;

        DUF_TRACE( explain, 1, "no %s option", duf_option_cnames( DUF_OPTION_FLAG_TREE ) );
        DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪print_dir≫ #%d", asteps );
        ppscan_callbacks[asteps++] = &duf_print_dir_callbacks;
      }
    }
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option", duf_option_cnames( DUF_OPTION_FLAG_PRINT ) );
  }
  if ( duf_config->cli.act.sample )
  {
    extern duf_scan_callbacks_t duf_sample_callbacks /* __attribute( ( weak ) ) */ ;

    DUF_TRACE( explain, 0, "     option %s", duf_option_cnames( DUF_OPTION_SAMPLE ) );
    assert( asteps + duf_config->cli.act.sample < max_asteps );
    for ( int i = 0; i < duf_config->cli.act.sample && asteps < max_asteps; i++ )
    {
      DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪sample #%d", asteps );
      ppscan_callbacks[asteps++] = &duf_sample_callbacks;
    }
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option or no callback", duf_option_cnames( DUF_OPTION_SAMPLE ) );
  }

  assert( asteps < max_asteps );
  if ( duf_config->cli.act.sampupd )
  {
    extern duf_scan_callbacks_t duf_sampupd_callbacks /* __attribute( ( weak ) ) */ ;

    DUF_TRACE( explain, 0, "     option %s", duf_option_cnames( DUF_OPTION_SAMPUPD ) );
    DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪sampupd≫ #%d", asteps );
    ppscan_callbacks[asteps++] = &duf_sampupd_callbacks;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option or no callback", duf_option_cnames( DUF_OPTION_SAMPUPD ) );
  }
  if ( asteps )
    DUF_TRACE( action, 0, "%d actions set; %s", asteps, r < 0 ? "FAIL" : "" );
  for ( int astep = 0; r >= 0 && astep < asteps; astep++ )
  {
    if ( ppscan_callbacks[astep] )
    {
      DUF_TRACE( action, 0, "%" DUF_ACTION_TITLE_FMT ": astep %d", duf_uni_scan_action_title( ppscan_callbacks[astep] ), astep );
      r = duf_uni_scan_targ( ppscan_callbacks[astep] );
      duf_config->actions_done++;
    }
  }

  if ( !duf_config->actions_done )
  {
    char *optnames = NULL;
    char *dirent_optnames = NULL;

    r = DUF_ERROR_NO_ACTIONS;


    DUF_TRACE( action, 0, "no actions set; %s", r < 0 ? "FAIL" : "" );
    optnames = duf_option_names( DUF_OPTION_FLAG_COLLECT );
    dirent_optnames = duf_option_names( DUF_OPTION_FLAG_DIRENT );
    DUF_PRINTF( 0, "to collect something use %s", optnames );
    {
      char *ona = NULL;

      ona = duf_option_names( DUF_OPTION_FLAG_DIRS );
      DUF_PRINTF( 0, "to collect directories use %s WITH %s AND %s", optnames, ona, dirent_optnames );
      mas_free( ona );
    }
    {
      char *ona = NULL;

      ona = duf_option_names( DUF_OPTION_FLAG_FILEDATA );
      DUF_PRINTF( 0, "to collect file data use %s WITH %s AND %s", optnames, ona, dirent_optnames );
      mas_free( ona );
    }
    {
      char *ona = NULL;

      ona = duf_option_names( DUF_OPTION_FLAG_FILENAMES );
      DUF_PRINTF( 0, "to collect file names use %s WITH %s AND %s", optnames, ona, dirent_optnames );
      mas_free( ona );
    }
    {
      char *ona = NULL;

      ona = duf_option_names( DUF_OPTION_FLAG_MD5 );
      DUF_PRINTF( 0, "to collect md5 names use %s WITH %s AND %s", optnames, ona, dirent_optnames );
      mas_free( ona );
    }
    DUF_PUTS( 0, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" );
    DUF_PUTSL( 0 );
    DUF_PUTSL( 0 );
    mas_free( optnames );
    mas_free( dirent_optnames );
  }

  /* if ( r == DUF_ERROR_MAX_REACHED )                 */
  /*   DUF_TRACE( action, 0, "Maximum reached ...." ); */
  /* else if ( r < 0 )                                 */
  /*   DUF_ERROR( "code: %d", r );                     */
  mas_free( ppscan_callbacks );
  DEBUG_ENDR( r );
  return r;
}
