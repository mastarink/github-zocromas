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
/* #include "duf_dir_scan1.h" */
#include "duf_dir_scan2.h"

#include "duf_dbg.h"

/* #include "duf_uni_scan1.h" */
#include "duf_uni_scan2.h"

/* ###################################################################### */
#include "duf_uni_scan.h"
/* ###################################################################### */


/*
 * run   --uni-scan /home/mastar/a/down/ --max-depth=4  --max-items=70 -R --tree --print  --files
 * run   --uni-scan /home/mastar/a/down/ --max-depth=4  --max-items=70 -R --tree --print
*/




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

  real_path = duf_realpath( path );
  DUF_TRACE( explain, 0, "start scan from path: ≪%s≫; real: ≪%s≫", path, real_path );

  DUF_SCCB( DUF_TRACE, action, 0, "..." );
  DUF_SCCB( DUF_TRACE, scan, 0, "uni" );
  if ( sccb )
  {
    duf_depthinfo_t di = {.depth = -1,
      .seq = 0,
      .levinfo = NULL,
      .u = *pu,
      .opendir = sccb ? sccb->def_opendir : 0,
      /* .name = real_path, */
    };

    DEBUG_STEP(  );

    r = duf_pdi_init( &di, real_path );
/* create level-control array, open 0 level */
    DUF_SCCB( DUF_TRACE, action, 2, "di.levinfo  %c", di.levinfo ? '+' : '-' );

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
        DUF_SCCB( DUF_TRACE, action, 0, "total_files: %llu", di.total_files );
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
      DUF_SCCB( DUF_TRACE, action, 0, "top dirid: %llu; path: %s;", duf_levinfo_dirid( &di ), real_path );
      if ( r == DUF_ERROR_NOT_IN_DB )
      {
        if ( !path )
          DUF_ERROR( "not in db: no path" );
        else if ( !real_path )
          DUF_ERROR( "not in db: no real_path" );
        else
          DUF_ERROR( "not in db:'%s' (%s)", real_path, path );
      }
      DUF_TEST_R( r );
      if ( r >= 0 )
      {
        DUF_TRACE( path, 0, "top dirid:%llu for %s", duf_levinfo_dirid( &di ), real_path );
        /* assert( top dirid == duf_levinfo_dirid( &di ) ); */
        DUF_TRACE( scan, 10, "top dirid:%llu for %s", duf_levinfo_dirid( &di ), real_path );
        DUF_TEST_R( r );

        DUF_TRACE( explain, 0,
                   "≫≫≫≫≫≫≫≫≫≫ %" "s" /* DUF_ACTION_TITLE_FMT */ " ≪≪≪≪≪≪≪≪≪≪≪≪≪≪≪≪≪",
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
	    DUF_ERROR("OBSOLETE sccb->scan_mode_2 == 0");
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
    }
    if ( DUF_ACT_FLAG( summary ) )
    {
      DUF_PRINTF( 0, "%s", duf_uni_scan_action_title( sccb ) );

      DUF_PRINTF( 0, " summary; seq:     %llu", di.seq );
      DUF_PRINTF( 0, " summary; seq-leaf:%llu", di.seq_leaf );
      DUF_PRINTF( 0, " summary; seq-node:%llu", di.seq_node );
      if ( duf_config->u.maxseq )
        DUF_PRINTF( 0, " of %llu (max-seq)", duf_config->u.maxseq );
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

typedef struct
{
  duf_config_act_flags_combo_t on;
  duf_config_act_flags_combo_t off;
  duf_scan_callbacks_t *sccb;
} duf_action_table_t;

extern duf_scan_callbacks_t duf_integrity_callbacks __attribute( ( weak ) ),
      duf_directories_callbacks __attribute( ( weak ) ),
      duf_filedata_callbacks __attribute( ( weak ) ),
      duf_filenames_callbacks __attribute( ( weak ) ),
      duf_collect_openat_crc32_callbacks __attribute( ( weak ) ),
      duf_collect_openat_sd5_callbacks __attribute( ( weak ) ),
      duf_collect_openat_md5_callbacks __attribute( ( weak ) ),
      duf_collect_mime_callbacks __attribute( ( weak ) ),
      duf_collect_exif_callbacks __attribute( ( weak ) ),
      duf_collect_mdpath_callbacks __attribute( ( weak ) ),
      duf_print_md5_callbacks __attribute( ( weak ) ),
      duf_print_tree_callbacks __attribute( ( weak ) ),
      duf_print_dir_callbacks __attribute( ( weak ) ),
      duf_bubububububububububububububububububububububububububububububububububububu __attribute( ( weak ) );

duf_action_table_t act_table[] = {
  {.sccb = &duf_integrity_callbacks,
   .on.flag = {.integrity = 1}
   },
  {.sccb = &duf_directories_callbacks,
   .on.flag = {.collect = 1,.dirent = 1,.dirs = 1}
   },
  {.sccb = &duf_filedata_callbacks,
   .on.flag = {.collect = 1,.dirent = 1,.filedata = 1}
   },
  {.sccb = &duf_filenames_callbacks,
   .on.flag = {.collect = 1,.dirent = 1,.filenames = 1}
   },
  {.sccb = &duf_collect_openat_crc32_callbacks,
   .on.flag = {.collect = 1,.crc32 = 1}
   },
  {.sccb = &duf_collect_openat_sd5_callbacks,
   .on.flag = {.collect = 1,.sd5 = 1}
   },
  {.sccb = &duf_collect_openat_md5_callbacks,
   .on.flag = {.collect = 1,.md5 = 1}
   },
  {.sccb = &duf_collect_mime_callbacks,
   .on.flag = {.collect = 1,.mime = 1}
   },
  {.sccb = &duf_collect_exif_callbacks,
   .on.flag = {.collect = 1,.exif = 1}
   },
  {.sccb = &duf_collect_mdpath_callbacks,
   .on.flag = {.mdpath = 1}},

  {.sccb = &duf_print_tree_callbacks,
   .on.flag = {.print = 1,.tree = 1},
   .off.flag = {.md5 = 1}
   },
  {.sccb = &duf_print_dir_callbacks,
   .on.flag = {.print = 1},
   .off.flag = {.md5 = 1,.tree = 1}
   },
  {.sccb = &duf_print_md5_callbacks,
   .on.flag = {.print = 1,.md5 = 1}
   },
};

#if 1
static int
duf_set_actions( duf_scan_callbacks_t ** ppscan_callbacks, int max_asteps )
{
  int asteps = 0;

  for ( int iac = 0; iac < sizeof( act_table ) / sizeof( act_table[0] ); iac++ )
  {
    if ( ( duf_config->cli.act.v.bit & act_table[iac].on.bit ) == act_table[iac].on.bit
         && ( duf_config->cli.act.v.bit & act_table[iac].off.bit ) == 0 )
    {
      duf_scan_callbacks_t *sccb = act_table[iac].sccb;

      if ( sccb )
      {
        DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
        ppscan_callbacks[asteps++] = sccb;
      }
    }
  }
  return asteps;
}
#else
static int
duf_set_actions( duf_scan_callbacks_t ** ppscan_callbacks, int max_asteps )
{
  int asteps = 0;

  /*
   * 1. DUF_ACT_FLAG( integrity ) => duf_integrity_callbacks
   * 
   * 2. DUF_ACT_FLAG( collect )
   *   2.1 DUF_ACT_FLAG( dirent )
   *     2.1.1.    DUF_ACT_FLAG( dirs )  => duf_directories_callbacks
   *     2.1.2    DUF_ACT_FLAG( filedata ) => duf_filedata_callbacks
   *     2.1.3    DUF_ACT_FLAG( filenames ) => duf_filenames_callbacks
   *   2.2.    DUF_ACT_FLAG( crc32 )     => duf_collect_openat_crc32_callbacks
   *   2.3.    DUF_ACT_FLAG( sd5 )       => duf_collect_openat_sd5_callbacks
   *   2.4.    DUF_ACT_FLAG( md5 )       => duf_collect_openat_md5_callbacks
   *   2.5.    DUF_ACT_FLAG( mime )      => duf_collect_mime_callbacks
   *   2.6.    DUF_ACT_FLAG( exif )      => duf_collect_exif_callbacks
   *
   * */
  if ( DUF_ACT_FLAG( integrity ) )
  {
    extern duf_scan_callbacks_t duf_integrity_callbacks /* __attribute( ( weak ) ) */ ;
    duf_scan_callbacks_t *sccb = &duf_integrity_callbacks;

    DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_INTEGRITY ) );
    DUF_TRACE( action, 2, "prep integrity ..." );
    DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪integrity≫ #%d", asteps );
    DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
    ppscan_callbacks[asteps++] = sccb;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option", DUF_OPT_NAME( FLAG_INTEGRITY ) );
  }
  assert( asteps < max_asteps );
  if ( DUF_ACT_FLAG( collect ) )
  {
    DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_COLLECT ) );
    if ( DUF_ACT_FLAG( dirent ) )
    {
      DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_DIRENT ) );
      {
        extern duf_scan_callbacks_t duf_directories_callbacks __attribute( ( weak ) );

        if ( &duf_directories_callbacks && DUF_ACT_FLAG( dirs ) )
        {
          duf_scan_callbacks_t *sccb = &duf_directories_callbacks;

          DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_DIRS ) );
          DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪directories≫ #%d", asteps );
          DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
          ppscan_callbacks[asteps++] = sccb;
        }
        else
        {
          DUF_TRACE( explain, 1, "no %s option or no callback", DUF_OPT_NAME( FLAG_DIRS ) );
        }
      }
      assert( asteps < max_asteps );
      {
        extern duf_scan_callbacks_t duf_filedata_callbacks __attribute( ( weak ) );


        if ( &duf_filedata_callbacks && DUF_ACT_FLAG( filedata ) )
        {
          duf_scan_callbacks_t *sccb = &duf_filedata_callbacks;

          DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_FILEDATA ) );
          DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪filedata≫ #%d", asteps );
          DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
          ppscan_callbacks[asteps++] = sccb;
        }
        else
        {
          DUF_TRACE( explain, 1, "no %s option or no callback", DUF_OPT_NAME( FLAG_FILEDATA ) );
        }
      }
      assert( asteps < max_asteps );
      {
        extern duf_scan_callbacks_t duf_filenames_callbacks __attribute( ( weak ) );


        if ( &duf_filenames_callbacks && DUF_ACT_FLAG( filenames ) )
        {
          duf_scan_callbacks_t *sccb = &duf_filenames_callbacks;

          DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_FILENAMES ) );
          DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪filenames≫ #%d", asteps );
          DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
          ppscan_callbacks[asteps++] = sccb;
        }
        else
        {
          DUF_TRACE( explain, 1, "no %s option or no callback", DUF_OPT_NAME( FLAG_FILENAMES ) );
        }
      }
    }
    assert( asteps < max_asteps );
    {
      extern duf_scan_callbacks_t duf_collect_openat_crc32_callbacks __attribute( ( weak ) );


      if ( &duf_collect_openat_crc32_callbacks && DUF_ACT_FLAG( crc32 ) )
      {
        duf_scan_callbacks_t *sccb = &duf_collect_openat_crc32_callbacks;

        DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_CRC32 ) );
        DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪collect_openat_crc32≫ #%d", asteps );
        DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
        ppscan_callbacks[asteps++] = sccb;
      }
      else
      {
        DUF_TRACE( explain, 1, "no %s option or no callback", DUF_OPT_NAME( FLAG_CRC32 ) );
      }
    }
    assert( asteps < max_asteps );
    {
      extern duf_scan_callbacks_t duf_collect_openat_sd5_callbacks __attribute( ( weak ) );


      if ( &duf_collect_openat_sd5_callbacks && DUF_ACT_FLAG( sd5 ) )
      {
        duf_scan_callbacks_t *sccb = &duf_collect_openat_sd5_callbacks;

        DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_SD5 ) );
        DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪collect_openat_sd5≫ #%d", asteps );
        DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
        ppscan_callbacks[asteps++] = sccb;
      }
      else
      {
        DUF_TRACE( explain, 1, "no %s option or no callback", DUF_OPT_NAME( FLAG_SD5 ) );
      }
    }
    assert( asteps < max_asteps );
    {
      extern duf_scan_callbacks_t duf_collect_openat_md5_callbacks __attribute( ( weak ) );


      if ( &duf_collect_openat_md5_callbacks && DUF_ACT_FLAG( md5 ) )
      {
        duf_scan_callbacks_t *sccb = &duf_collect_openat_md5_callbacks;

        DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_MD5 ) );
        DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪collect_openat_md5≫ #%d", asteps );
        DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
        ppscan_callbacks[asteps++] = sccb;
      }
      else
      {
        DUF_TRACE( explain, 1, "no %s option or no callback", DUF_OPT_NAME( FLAG_MD5 ) );
      }
    }
    assert( asteps < max_asteps );
    {
      extern duf_scan_callbacks_t duf_collect_mime_callbacks __attribute( ( weak ) );

      if ( &duf_collect_mime_callbacks && DUF_ACT_FLAG( mime ) )
      {
        duf_scan_callbacks_t *sccb = &duf_collect_mime_callbacks;

        DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_MIME ) );
        DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪collect_mime≫ #%d", asteps );
        DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
        ppscan_callbacks[asteps++] = sccb;
      }
      else
      {
        DUF_TRACE( explain, 1, "no %s option or no callback", DUF_OPT_NAME( FLAG_MIME ) );
      }
    }
    assert( asteps < max_asteps );
    {
      extern duf_scan_callbacks_t duf_collect_exif_callbacks __attribute( ( weak ) );

      if ( &duf_collect_exif_callbacks && DUF_ACT_FLAG( exif ) )
      {
        duf_scan_callbacks_t *sccb = &duf_collect_exif_callbacks;

        DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_EXIF ) );
        DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪collect_exif≫ #%d", asteps );
        DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
        ppscan_callbacks[asteps++] = sccb;
      }
      else
      {
        DUF_TRACE( explain, 1, "no %s option or no callback", DUF_OPT_NAME( FLAG_EXIF ) );
      }
    }
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option", DUF_OPT_NAME( FLAG_COLLECT ) );
  }

  assert( asteps < max_asteps );
  if (  /* DUF_ACT_FLAG(update )&& */ DUF_ACT_FLAG( mdpath ) )
  {
    extern duf_scan_callbacks_t duf_collect_mdpath_callbacks /* __attribute( ( weak ) ) */ ;
    duf_scan_callbacks_t *sccb = &duf_collect_mdpath_callbacks;

    DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_MDPATH ) );
    DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪collect_mdpath≫ #%d", asteps );
    DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
    ppscan_callbacks[asteps++] = sccb;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option", DUF_OPT_NAME( FLAG_MDPATH ) );
  }
  assert( asteps < max_asteps );
  if ( DUF_ACT_FLAG( print ) )
  {
    DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_PRINT ) );
    if ( DUF_ACT_FLAG( md5 ) )
    {
      extern duf_scan_callbacks_t duf_print_md5_callbacks /* __attribute( ( weak ) ) */ ;
      duf_scan_callbacks_t *sccb = &duf_print_md5_callbacks;

      DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_MD5 ) );
      DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪print_md5≫ #%d", asteps );
      DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
      ppscan_callbacks[asteps++] = sccb;
    }
    else
    {
      assert( asteps < max_asteps );
      DUF_TRACE( explain, 1, "no %s option", DUF_OPT_NAME( FLAG_MD5 ) );
      if ( DUF_ACT_FLAG( tree ) )
      {
        extern duf_scan_callbacks_t duf_print_tree_callbacks /* __attribute( ( weak ) ) */ ;
        duf_scan_callbacks_t *sccb = &duf_print_tree_callbacks;

        DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_TREE ) );
        DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪print_tree≫ #%d", asteps );
        DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
        ppscan_callbacks[asteps++] = sccb;
      }
      else
      {
        assert( asteps < max_asteps );
        extern duf_scan_callbacks_t duf_print_dir_callbacks /* __attribute( ( weak ) ) */ ;
        duf_scan_callbacks_t *sccb = &duf_print_dir_callbacks;

        DUF_TRACE( explain, 1, "no %s option", DUF_OPT_NAME( FLAG_TREE ) );
        DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪print_dir≫ #%d", asteps );
        DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
        ppscan_callbacks[asteps++] = sccb;
      }
    }
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option", DUF_OPT_NAME( FLAG_PRINT ) );
  }
  return asteps;
}
#endif
int
duf_set_actions_sample( duf_scan_callbacks_t ** ppscan_callbacks, int max_asteps )
{
  int asteps = 0;

  if ( duf_config->cli.act.sample )
  {
    extern duf_scan_callbacks_t duf_sample_callbacks /* __attribute( ( weak ) ) */ ;

    DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( SAMPLE ) );
    assert( asteps + duf_config->cli.act.sample < max_asteps );
    for ( int i = 0; i < duf_config->cli.act.sample && asteps < max_asteps; i++ )
    {
      duf_scan_callbacks_t *sccb = &duf_sample_callbacks;

      DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪sample #%d", asteps );
      DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
      ppscan_callbacks[asteps++] = sccb;
    }
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option or no callback", DUF_OPT_NAME( SAMPLE ) );
  }

  assert( asteps < max_asteps );
  if ( duf_config->cli.act.sampupd )
  {
    extern duf_scan_callbacks_t duf_sampupd_callbacks /* __attribute( ( weak ) ) */ ;
    duf_scan_callbacks_t *sccb = &duf_sampupd_callbacks;

    DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( SAMPUPD ) );
    DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪sampupd≫ #%d", asteps );
    DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
    ppscan_callbacks[asteps++] = sccb;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option or no callback", DUF_OPT_NAME( SAMPUPD ) );
  }
  return asteps;
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
