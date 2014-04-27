#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>
#include <openssl/md5.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"

#include "duf_levinfo.h"

#include "duf_path.h"
#include "duf_file.h"

#include "duf_sql_def.h"
#include "duf_sql.h"
#include "duf_sql_field.h"

#include "duf_dbg.h"

/* ###################################################################### */
/* #include "duf_mdpath_uni.h" */
/* ###################################################################### */

/* run  --db-name=test`datem`.db  --uni-scan --fill --mdpath -dfR  /mnt/new_media/media/down/  --trace-mdpath=1 --trace-new=1 --trace-stdout */


/* callback of type duf_scan_callback_file_t */
static int
scan_leaf( duf_depthinfo_t * pdi, duf_record_t * precord /*, const duf_dirhandle_t * pdh_notused */  )
{
  int r = 0;

  DEBUG_START(  );
  if ( pdi->depth <= 0 )
  {
    DUF_ERROR( "depth should not be %d at this point", pdi->depth );
    assert( pdi->depth > 0 );
  }
  if ( pdi->depth > 0 )
  {
    {
      DUF_UFIELD( filenameid );
      char *fpath = filenameid_to_filepath( filenameid, pdi, &r );

      DUF_TRACE( mdpath, 2, "#%4llu: mdpath fpath=%s", pdi->seq, fpath );

      mas_free( fpath );
    }
    {
      MD5_CTX *pctx;

      /* unsigned long long pathid; */

      /* pathid = va_arg( args, unsigned long long );                                                */
      /* fprintf( stderr, "(duf_sel_cb_update_mdpaths_path) parthid:%lld / %s\n", pathid, precord->presult[0] ); */
      /* pctx = ( MD5_CTX * ) pdi->levinfo[pdi->depth - 1].context;                                                   */
      /* DUF_TRACE( mdpath, 1, "(%p) L%u context=%p", ( void * ) pdi, pdi->depth, pdi->levinfo[pdi->depth].context ); */
      pctx = duf_levinfo_context_up( pdi );
      assert( pctx );
      if ( precord->presult[5] && precord->presult[6] )
      {
        DUF_UFIELD( md5sum1 );
        DUF_UFIELD( md5sum2 );

        /* fprintf( stderr, "%lld:%lld\n", md5s1, md5s2 ); */
        MD5_Update( pctx, &md5sum1, sizeof( md5sum1 ) );
        MD5_Update( pctx, &md5sum2, sizeof( md5sum2 ) );
      }

    }
  }
  DEBUG_ENDR( r );
  return r;
}

/* callback of type duf_scan_callback_dir_t */
static int
scan_node_before( unsigned long long pathid, /* const duf_dirhandle_t * pdh_notused, */ duf_depthinfo_t * pdi,
                  duf_record_t * precord )
{
  int r = 0;

  DEBUG_START(  );

  {
    char *path = duf_pathid_to_path_s( pathid, pdi, &r );


    DUF_TRACE( mdpath, 1, "#%4llu: BEFORE dPATH=%s", pdi->seq, path );
    mas_free( path );
  }

  {
    MD5_CTX *pctx;

    pctx = mas_malloc( sizeof( MD5_CTX ) );
    memset( pctx, 0, sizeof( MD5_CTX ) );
    MD5_Init( pctx );

    assert( !duf_levinfo_context( pdi ) );
    duf_levinfo_set_context( pdi, pctx );
    assert( duf_levinfo_context( pdi ) );
  }
  DEBUG_ENDR( r );
  return r;
}

/*
 * this is callback of type: duf_scan_hook_dir_t (second range; ; sel_cb): 
 * */

static unsigned long long
duf_insert_mdpath_uni( unsigned long long *md64, int *pr )
{
  unsigned long long mdpathid = 0;
  int r = 0;
  int changes = 0;

  r = duf_sql( "INSERT OR IGNORE INTO duf_mdpath (mdpathsum1,mdpathsum2) VALUES ('%lld','%lld')",
               &changes, md64[1], md64[0] );
  if ( ( r == DUF_SQL_CONSTRAINT || !r ) && !changes )
  {
    duf_scan_callbacks_t sccb = {.fieldset = "mppathid" };
    r = duf_sql_select( duf_sel_cb_field_by_sccb, &mdpathid, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                        &sccb, ( const duf_dirhandle_t * ) NULL,
                        "SELECT id AS mppathid " " FROM duf_mdpath " " WHERE mdpathsum1='%lld' AND mdpathsum2='%lld'", md64[1], md64[0] );
  }
  else if ( r >= 0 )
  {
    mdpathid = duf_sql_last_insert_rowid(  );
  }
  else
  {
    r = DUF_ERROR_INSERT_MDPATH;
    DUF_ERROR( "insert mdpath %d", r );
  }
  if ( pr )
    *pr = r;
  return mdpathid;
}

static int
scan_node_after( unsigned long long pathid, /* const duf_dirhandle_t * pdh_notused, */ duf_depthinfo_t * pdi,
                 duf_record_t * precord )
{
  int r = 0;

  DEBUG_START(  );

  /* {                                                                          */
  /*   char *path = duf_pathid_to_path_s( pathid, pdi, &r );                    */
  /*                                                                            */
  /*                                                                            */
  /*   DUF_TRACE( mdpath, 1, "#%4llu: mdpath AFTER dPATH=%s", pdi->seq, path ); */
  /*   mas_free( path );                                                        */
  /* }                                                                          */
  /* if ( r >= 0 ) */
  {
    unsigned long long *md64 = NULL;

    if ( r >= 0 )
    {
      MD5_CTX *pctx;

      pctx = duf_levinfo_context( pdi );
      assert( pctx );

      if ( pctx )
      {
        unsigned char md[MD5_DIGEST_LENGTH];

        memset( &md, 0, sizeof( md ) );
        MD5_Final( md, pctx );
        md64 = ( unsigned long long * ) md;
      }
    }
    DUF_TEST_R( r );
    if ( r >= 0 )
    {
      unsigned long long mdpathid = duf_insert_mdpath_uni( md64, &r );

      /* DUF_TRACE( mdpath, 0, "[%016llx%016llx] UPDATE duf_paths " " SET md5dir1='%lld', md5dir2='%lld', mdpathid='%lld' " " WHERE id='%llu'", */
      /*            md64[1], md64[0], md64[1], md64[0], mdpathid, pathid );                                                                     */

      if ( r >= 0 )
      {
        int changes = 0;

        /* r = duf_sql( "UPDATE duf_paths " " SET md5dir1='%lld', md5dir2='%lld', mdpathid='%lld' " " WHERE id='%llu'", ( int * ) NULL, */
        /*              md64[1], md64[0], mdpathid, pathid );                                                                           */
        r = duf_sql( "UPDATE duf_pathtot_dirs SET mdpathid='%lld' WHERE pathid=='%lld'", &changes, mdpathid, pathid );
        if ( !changes )
        {
          r = duf_sql( "INSERT INTO duf_pathtot_dirs (mdpathid,pathid) VALUES ('%lld','%lld')", &changes, mdpathid, pathid );
          if ( !changes )
            DUF_ERROR( "CHI.%d", changes );
        }
      }
    }
    DUF_TEST_R( r );
  }
  DEBUG_ENDR( r );
  return r;
}




static char *final_sql[] = {
 /* *INDENT-OFF*  */

  /* *INDENT-ON*  */

  NULL,
};

duf_scan_callbacks_t duf_fill_mdpath_callbacks = {
  .title = __FILE__,
  .init_scan = NULL,
  .node_scan_before = scan_node_before,
  .node_scan_after = scan_node_after,
  .leaf_scan = scan_leaf,
  .fieldset =
        " duf_filenames.pathid AS dirid " " ,duf_filenames.name AS filename, duf_filedatas.size AS filesize"
        " , uid, gid, nlink, inode, mtim AS mtime " " , dupcnt AS nsame"
        " , duf_filenames.id AS filenameid" " , duf_filedatas.mode AS filemode, md.md5sum1, md.md5sum2",
  .leaf_selector =
        "SELECT %s FROM duf_filenames "
        " JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) "
        " LEFT JOIN duf_md5 AS md on (md.id=duf_filedatas.md5id)" "    WHERE "
        /* "           duf_filedatas.size >= %llu AND duf_filedatas.size < %llu "            */
        /* "       AND (md.dupcnt IS NULL OR (md.dupcnt >= %llu AND md.dupcnt < %llu)) AND " */
        " duf_filenames.pathid='%llu' " " ORDER BY duf_filenames.name ",
  .node_selector =
        "SELECT duf_paths.id AS dirid, duf_paths.dirname, duf_paths.dirname AS dfname,  duf_paths.parentid "
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize "
        /* " ,(SELECT count(*) FROM duf_paths AS subpaths WHERE subpaths.parentid=duf_paths.id) AS ndirs "       */
        /* " ,(SELECT count(*) FROM duf_filenames AS sfn "                                                       */
        /* "          JOIN duf_filedatas AS sfd ON (sfn.dataid=sfd.id) "                                         */
        /* "          JOIN duf_md5 AS smd ON (sfd.md5id=smd.id) "                                                */
        /* "          WHERE sfn.pathid=duf_paths.id "                                                            */
        /* "              AND   sfd.size >= %llu AND sfd.size < %llu "                                           */
        /* "              AND (smd.dupcnt IS NULL OR (smd.dupcnt >= %llu AND smd.dupcnt < %llu)) "               */
        /* " ) AS nfiles "                                                                                       */
        /* " ,(SELECT min(sfd.size) FROM duf_filedatas AS sfd JOIN duf_filenames AS sfn ON (sfn.dataid=sfd.id) " */
        /* "           WHERE sfn.pathid=duf_paths.id) AS minsize "                                               */
        /* " ,(SELECT max(sfd.size) FROM duf_filedatas AS sfd JOIN duf_filenames AS sfn ON (sfn.dataid=sfd.id) " */
        /* "           WHERE sfn.pathid=duf_paths.id) AS maxsize "                                               */
        " FROM duf_paths "
        " LEFT JOIN duf_pathtot_dirs AS td ON (td.pathid=duf_paths.id) "
        " LEFT JOIN duf_pathtot_files AS tf ON (tf.pathid=duf_paths.id) " " WHERE duf_paths.parentid='%llu' ",
  .final_sql_argv = final_sql,
};
