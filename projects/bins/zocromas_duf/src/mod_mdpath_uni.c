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
/* #include "duf_file.h" */

#include "duf_sql_def.h"
#include "duf_sql_field.h"

#include "duf_sql.h"
#include "duf_sql1.h"

#include "duf_dbg.h"

/* ###################################################################### */
/* #include "duf_mdpath_uni.h" */
/* ###################################################################### */

/* run  --db-name=test`datem`.db  --uni-scan --collect --mdpath -dfR  /mnt/new_media/media/down/  --trace-mdpath=1 --trace-new=1 --trace-stdout */


/* callback of type duf_scan_hook_file_t */
static int
mdpath_scan_leaf( duf_depthinfo_t * pdi, duf_record_t * precord /*, const duf_dirhandle_t * pdh_unused */  )
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
    MD5_CTX *pctx;

    pctx = duf_levinfo_context_up( pdi );
    assert( pctx );
    if ( precord->presult[5] && precord->presult[6] )
    {
      DUF_UFIELD( md5sum1 );
      DUF_UFIELD( md5sum2 );

      if ( r >= 0 && MD5_Update( pctx, &md5sum1, sizeof( md5sum1 ) ) != 1 )
        r = DUF_ERROR_MD5;
      DUF_TEST_R( r );
      if ( r >= 0 && MD5_Update( pctx, &md5sum2, sizeof( md5sum2 ) ) != 1 )
        r = DUF_ERROR_MD5;
      DUF_TEST_R( r );
    }
  }
  DEBUG_ENDR( r );
  return r;
}

/* 
 * this is callback of type: duf_scan_hook_dir_t
 * */
static int
mdpath_scan_node_before( unsigned long long pathid_unused, /* const duf_dirhandle_t * pdh_unused, */ duf_depthinfo_t * pdi,
                         duf_record_t * precord )
{
  int r = 0;

  DEBUG_START(  );

  {
    const char *real_path = NULL;

    if ( !real_path )
      real_path = duf_levinfo_path( pdi );
    /* char *path = duf_pathid_to_path_s( pathid, pdi, &r ); */


    DUF_TRACE( mdpath, 1, "#%4llu: BEFORE dPATH=%s", pdi->seq, real_path );
    /* mas_free( path ); */
  }
  {
    MD5_CTX *pctx;

    pctx = mas_malloc( sizeof( MD5_CTX ) );
    memset( pctx, 0, sizeof( *pctx ) );
    if ( MD5_Init( pctx ) != 1 )
      r = DUF_ERROR_MD5;
    DUF_TEST_R( r );
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

  r = duf_sql( "INSERT OR IGNORE INTO duf_mdpath (mdpathsum1,mdpathsum2) VALUES ('%lld','%lld')", &changes, md64[1], md64[0] );
  if ( ( r == DUF_SQL_CONSTRAINT || !r ) && !changes )
  {
    duf_scan_callbacks_t sccb = {.fieldset = "mppathid" };
    r = duf_sql_select( duf_sel_cb_field_by_sccb, &mdpathid, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                        &sccb /*, ( const duf_dirhandle_t * ) NULL off */ ,
                        "SELECT id AS mppathid FROM duf_mdpath WHERE mdpathsum1='%lld' AND mdpathsum2='%lld'", md64[1], md64[0] );
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

/* 
 * this is callback of type: duf_scan_hook_dir_t
 * */
static int
mdpath_scan_node_after( unsigned long long pathid_unused, /* const duf_dirhandle_t * pdh_unused, */ duf_depthinfo_t * pdi,
                        duf_record_t * precord )
{
  int r = 0;

  DEBUG_START(  );

  {
    unsigned char md[MD5_DIGEST_LENGTH];

    memset( &md, 0, sizeof( md ) );
    if ( r >= 0 )
    {
      MD5_CTX *pctx;

      pctx = duf_levinfo_context( pdi );
      assert( pctx );

      if ( pctx )
        if ( MD5_Final( md, pctx ) != 1 )
          r = DUF_ERROR_MD5;
      DUF_TEST_R( r );
    }
    DUF_TEST_R( r );
    if ( r >= 0 )
    {
      unsigned long long mdpathid;
      unsigned long long *md64 = NULL;

      md64 = ( unsigned long long * ) md;
      mdpathid = duf_insert_mdpath_uni( md64, &r );

      if ( r >= 0 )
      {
        int changes = 0;
        unsigned long long dirid = duf_levinfo_dirid( pdi );

        r = duf_sql( "UPDATE duf_pathtot_dirs SET mdpathid='%lld' WHERE Pathid=='%lld'", &changes, mdpathid, dirid );
        if ( r >= 0 && !changes )
        {

          r = duf_sql( "INSERT INTO duf_pathtot_dirs (mdpathid,Pathid) VALUES ('%lld','%lld')", &changes, mdpathid, dirid );
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

duf_scan_callbacks_t duf_collect_mdpath_callbacks = {
  .title = __FILE__,
  .init_scan = NULL,
  .node_scan_before = mdpath_scan_node_before,
  .node_scan_after = mdpath_scan_node_after,
  .leaf_scan = mdpath_scan_leaf,
  .fieldset =
        " fn.Pathid AS dirid " /*	*/
	" ,fn.name AS filename, fd.size AS filesize" /*	*/
        " , uid, gid, nlink, inode, mtim AS mtime " /*	*/
	" , dupcnt AS nsame" /*	*/
        " , fn.id AS filenameid" /*	*/
	" , fd.mode AS filemode, md.md5sum1, md.md5sum2",
  .leaf_selector =
        "SELECT %s FROM duf_filenames AS fn " /*	*/
        " LEFT JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) " /*	*/
        " LEFT JOIN duf_md5 AS md ON (md.id=fd.md5id)" /*	*/
	"    WHERE " /*	*/
        /* "           fd.size >= %llu AND fd.size < %llu "            */
        /* "       AND (md.dupcnt IS NULL OR (md.dupcnt >= %llu AND md.dupcnt < %llu)) AND " */
        " fn.Pathid='%llu' " /*	*/
	" ORDER BY fn.name ",
  .node_selector =
        "SELECT duf_paths.id AS dirid, duf_paths.dirname, duf_paths.dirname AS dfname,  duf_paths.parentid " /*	*/
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /*	*/
        /* " ,(SELECT count(*) FROM duf_paths AS subpaths WHERE subpaths.parentid=duf_paths.id) AS ndirs "       */
        /* " ,(SELECT count(*) FROM duf_filenames AS sfn "                                                       */
        /* "          JOIN duf_filedatas AS sfd ON (sfn.dataid=sfd.id) "                                         */
        /* "          JOIN duf_md5 AS smd ON (sfd.md5id=smd.id) "                                                */
        /* "          WHERE sfn.Pathid=duf_paths.id "                                                            */
        /* "              AND   sfd.size >= %llu AND sfd.size < %llu "                                           */
        /* "              AND (smd.dupcnt IS NULL OR (smd.dupcnt >= %llu AND smd.dupcnt < %llu)) "               */
        /* " ) AS nfiles "                                                                                       */
        /* " ,(SELECT min(sfd.size) FROM duf_filedatas AS sfd JOIN duf_filenames AS sfn ON (sfn.dataid=sfd.id) " */
        /* "           WHERE sfn.Pathid=duf_paths.id) AS minsize "                                               */
        /* " ,(SELECT max(sfd.size) FROM duf_filedatas AS sfd JOIN duf_filenames AS sfn ON (sfn.dataid=sfd.id) " */
        /* "           WHERE sfn.Pathid=duf_paths.id) AS maxsize "                                               */
        " FROM duf_paths " /*	*/
        " LEFT JOIN duf_pathtot_dirs AS td ON (td.Pathid=duf_paths.id) " /*	*/
        " LEFT JOIN duf_pathtot_files AS tf ON (tf.Pathid=duf_paths.id) " /*	*/
	" WHERE duf_paths.parentid='%llu' ",
  .final_sql_argv = final_sql,
};
