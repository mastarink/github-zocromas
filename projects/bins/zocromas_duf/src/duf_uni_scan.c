#define DUF_SQL_PDI_STMT

#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>



#include "duf_types.h"
#include "duf_errors_headers.h"


#include "duf_utils.h"
#include "duf_utils_path.h"

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
/* #include "duf_dir_scan1.h" */
#include "duf_dir_scan2.h"


/* #include "duf_uni_scan1.h" */
#include "duf_uni_scan2.h"
#include "duf_prepare_actions.h"

/* ###################################################################### */
#include "duf_uni_scan.h"
/* ###################################################################### */


/*
 * run   --uni-scan /home/mastar/a/down/ --max-depth=4  --max-items=70 -R --tree --print  --files
 * run   --uni-scan /home/mastar/a/down/ --max-depth=4  --max-items=70 -R --tree --print
*/

static unsigned long long
duf_count_total_items( duf_scan_callbacks_t * sccb, int *pr )
{
  unsigned long long cnt = 0;
  int r = -1;

  if ( sccb && sccb->leaf_selector_total2 )
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
      cnt = duf_sql_column_long_long( pstmt, 0 );
      r = 0;
    }
    DUF_SQL_END_STMT_NOPDI( r, pstmt );
    mas_free( sqlt );
  }
  if ( pr )
    *pr = r;
  return cnt;
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
duf_uni_scan_from_path( const char *path, duf_ufilter_t * pu, duf_scan_callbacks_t * sccb, unsigned long long *pchanges )
{
  int r = 0;
  char *real_path = NULL;

  DEBUG_START(  );

  real_path = duf_realpath( path, &r );

  DUF_TRACE( explain, 0, "start scan from path: ≪%s≫; real: ≪%s≫", path, real_path );

  DUF_SCCB( DUF_TRACE, action, 0, "..." );
  DUF_SCCB( DUF_TRACE, scan, 0, "uni" );
  if ( r >= 0 && sccb )
  {
    duf_depthinfo_t di = {.depth = -1,
      .seq = 0,
      .levinfo = NULL,
      .u = *pu,
      .opendir = sccb ? sccb->def_opendir : 0,
      /* .name = real_path, */
    };

    DEBUG_STEP(  );

    di.total_files = duf_count_total_items( sccb, &r );
    DUF_SCCB( DUF_TRACE, action, 0, "total_files: %llu", di.total_files );

    r = duf_pdi_init_msg( &di, real_path );

    /* create level-control array, open 0 level */
    DUF_SCCB( DUF_TRACE, action, 2, "di.levinfo  %c", di.levinfo ? '+' : '-' );
    DUF_SCCB( DUF_TRACE, action, 0, "top dirid: %llu; path: %s;", duf_levinfo_dirid( &di ), real_path );
    
    
    DUF_TEST_R( r );

    /* DUF_ERROR( "TOTAL FILES:%llu; r:%d", di.total_files, r ); */
    DUF_TRACE( explain, 0, "%llu files registered in db", di.total_files );

    if ( r >= 0 )
    {
      DUF_TRACE( path, 0, "top dirid:%llu for %s", duf_levinfo_dirid( &di ), real_path );
      /* assert( top dirid == duf_levinfo_dirid( &di ) ); */
      DUF_TRACE( scan, 10, "top dirid:%llu for %s", duf_levinfo_dirid( &di ), real_path );
      DUF_TEST_R( r );

      DUF_TRACE( explain, 0, "≫≫≫≫≫≫≫≫≫≫ %" "s" /* DUF_ACTION_TITLE_FMT */ " ≪≪≪≪≪≪≪≪≪≪≪≪≪≪≪≪≪",
                 duf_uni_scan_action_title( sccb ) );


      /* DUF_ERROR( "L%d", di.depth ); */
      DUF_OINV_OPENED( di. );
      assert( di.depth >= 0 );


/* duf_str_cb(1?)_uni_scan_dir:
 * if recursive, call duf_scan_dirs_by_parentid + pdi (built from str_cb_udata)
 *       for each <dir> record by top dirid (i.e. children of top dirid) with corresponding args 
 * otherwise do nothing
 *
 * call duf_str_cb(1?)_uni_scan_dir with pdi for each dir at db by top dirid (i.e. children of top dirid) 
 *
 *   i.e.
 *     1. for <current> dir call sccb->node_scan_before
 *     2. for each leaf in <current> dir call sccb->leaf_scan
 *     3. for <current> dir call sccb->node_scan_middle
 *   recursively from <current> dir (if recursive flag set):
 *     4. for each dir in <current> dir call duf_str_cb(1?)_uni_scan_dir + &di as str_cb_udata
 *     5. for <current> dir call sccb->node_scan_after
 * */
      DUF_OINV_OPENED( di. );

      if ( r >= 0 )
      {
        DUF_TRACE( scan, 12, "before by_parentid" );
        DUF_TRACE( explain, 0, "to scan" );

        DUF_SCCB( DUF_TRACE, scan, 0, "scanning: top dirid: %llu; path: %s;", duf_levinfo_dirid( &di ), real_path );
        if ( sccb->scan_mode_2 )
          r = duf_scan_dirs_by_parentid2( ( duf_sqlite_stmt_t * ) NULL, duf_str_cb2_uni_scan_dir, &di, sccb );
        else
        {
          /* r = duf_scan_dirs_by_parentid1( duf_str_cb1_uni_scan_dir, &di, sccb, ( duf_record_t * ) NULL (* precord *)  ); */
          DUF_ERROR( "OBSOLETE sccb->scan_mode_2 == 0" );
        }
      }
      DUF_TEST_R( r );
    }
    else
    {
      DUF_TRACE( scan, 10, "? top dirid:%llu; real_path:'%s'", duf_levinfo_dirid( &di ), real_path );
    }
/* delete level-control array, close 0 level */
    if ( pchanges )
      *pchanges += di.changes;
    duf_pdi_close( &di );

    if ( DUF_ACT_FLAG( summary ) )
    {
      DUF_PRINTF( 0, "%s", duf_uni_scan_action_title( sccb ) );

      DUF_PRINTF( 0, " summary; seq:     %llu", di.seq );
      DUF_PRINTF( 0, " summary; seq-leaf:%llu", di.seq_leaf );
      DUF_PRINTF( 0, " summary; seq-node:%llu", di.seq_node );
      if ( duf_config->u.max_seq )
        DUF_PRINTF( 0, " of %llu (max-seq)", duf_config->u.max_seq );
    }
    DUF_TRACE( action, 1, "%" DUF_ACTION_TITLE_FMT ": end scan ; summary:%d", duf_uni_scan_action_title( sccb ), DUF_ACT_FLAG( summary ) );
  }
  else
  {
    DUF_TRACE( scan, 10, "?" );
  }
  mas_free( real_path );
  r = duf_clear_error( r, DUF_ERROR_MAX_SEQ_REACHED, 0 );
  DUF_TEST_R( r );

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
    DUF_TRACE( action, 1, "%" DUF_ACTION_TITLE_FMT ": inited scan", duf_uni_scan_action_title( sccb ) );

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
      DUF_TRACE( action, 1, "%" DUF_ACTION_TITLE_FMT ": targc:%u", duf_uni_scan_action_title( sccb ), duf_config->targc );
      for ( int ia = 0; r >= 0 && ia < duf_config->targc; ia++ )
        DUF_TRACE( action, 1, "%" DUF_ACTION_TITLE_FMT ": targv[%d]='%s'", duf_uni_scan_action_title( sccb ), ia, duf_config->targv[ia] );
      if ( duf_config->targc > 0 )
        for ( int ia = 0; r >= 0 && ia < duf_config->targc; ia++ )
          r = duf_uni_scan_from_path( duf_config->targv[ia], &duf_config->u, sccb, &changes );
      else
        r = duf_uni_scan_from_path( NULL, &duf_config->u, sccb, &changes );
      DUF_TEST_R( r );

      DUF_TRACE( action, 1, "after scan" );
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

        DUF_TRACE( action, 2, "final psql : %s", *psql );
        /* r = duf_sql( *p, &changes ); */

        {
          DUF_SQL_START_STMT_NOPDI( *psql, r, pstmt );
          /* r = duf_sql_exec( *psql, &changes ); */
          DUF_SQL_STEP( r, pstmt );
          DUF_SQL_CHANGES_NOPDI( changes, r, pstmt );
          DUF_SQL_END_STMT_NOPDI( r, pstmt );
        }
        DUF_TEST_R( r );
        /* DUF_TRACE( action, 2, "(%d) final psql %s; changes:%d", r, *psql, changes ); */
        DUF_TRACE( action, 2, "%" DUF_ACTION_TITLE_FMT ": final SQL %lu; [%s] changes:%d; %s", duf_uni_scan_action_title( sccb ),
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

  DEBUG_START(  );

  DUF_TRACE( action, 1, "prep" );
  DUF_TRACE( explain, 0, "scan all; setting actions" );
  DUF_TRACE( explain, 0, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" );
  {
    int max_asteps = 100;

    ppscan_callbacks = mas_malloc( max_asteps * sizeof( duf_scan_callbacks_t * ) );

    {
      int asteps = 0;

      asteps += duf_set_actions( ppscan_callbacks + asteps, max_asteps - asteps );
      asteps += duf_set_actions_sample( ppscan_callbacks + asteps, max_asteps - asteps );
      if ( asteps )
        DUF_TRACE( action, 0, "%d actions set; %s", asteps, r < 0 ? "FAIL" : "" );
      for ( int astep = 0; r >= 0 && astep < asteps; astep++ )
      {
        if ( ppscan_callbacks[astep] )
        {
          DUF_TRACE( action, 2, "%" DUF_ACTION_TITLE_FMT ": astep %d", duf_uni_scan_action_title( ppscan_callbacks[astep] ), astep );
          r = duf_uni_scan_targ( ppscan_callbacks[astep] );
          duf_config->actions_done++;
        }
      }
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
    DUF_TRACE( explain, 0, "to collect something use %s", optnames );
    {
      char *ona = NULL;

      ona = duf_option_names( DUF_OPTION_FLAG_DIRS );
      DUF_TRACE( explain, 0, "to collect directories use %s WITH %s AND %s", optnames, ona, dirent_optnames );
      mas_free( ona );
    }
    {
      char *ona = NULL;

      ona = duf_option_names( DUF_OPTION_FLAG_FILEDATA );
      DUF_TRACE( explain, 0, "to collect file data use %s WITH %s AND %s", optnames, ona, dirent_optnames );
      mas_free( ona );
    }
    {
      char *ona = NULL;

      ona = duf_option_names( DUF_OPTION_FLAG_FILENAMES );
      DUF_TRACE( explain, 0, "to collect file names use %s WITH %s AND %s", optnames, ona, dirent_optnames );
      mas_free( ona );
    }
    {
      char *ona = NULL;

      ona = duf_option_names( DUF_OPTION_FLAG_MD5 );
      DUF_TRACE( explain, 0, "to collect md5 names use %s WITH %s AND %s", optnames, ona, dirent_optnames );
      mas_free( ona );
    }
    DUF_TRACE( explain, 0, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" );
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
