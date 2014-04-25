#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

/* #include <dirent.h> */
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"

#include "duf_path.h"
#include "duf_file.h"

/* #include "duf_sql_def.h" */
/* #include "duf_sql.h" */
#include "duf_sql_field.h"

#include "duf_dbg.h"

/* ###################################################################### */
/* #include "duf_sampupd_uni.h" */
/* ###################################################################### */



/* callback of type duf_scan_callback_file_t */
static int
scan_leaf(  duf_depthinfo_t * pdi,
                           duf_record_t * precord /*, const duf_dirhandle_t * pdh_notused */  )
{
  int r = 0;

  DUF_SFIELD( filename );
  /* const char *filename = duf_sql_str_by_name( "filename", precord, 0 ); */

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
/* stat */

  /* SQL at duf_file_pathid.c : duf_scan_fil_by_pi */

  /* 
   * --uni-scan   -R   --sampupd   --files   -FF
   *                   ^^^^^^^^   ^^^^^^^
   * */


  if ( pdi->depth <= 0 )
  {
    DUF_ERROR( "depth should not be %d at this point", pdi->depth );
    assert( pdi->depth > 0 );
  }
  if ( pdi->depth > 0 )
  {
    DUF_UFIELD( filenameid );
    char *fpath = filenameid_to_filepath( filenameid, pdi, &r );

    printf( "#%4llu: sampupd fpath %s\n", pdi->seq, fpath );

    DUF_TRACE( sampupd, 1, "fpath=%s", fpath );
    DUF_TRACE_SAMPLE( 1, "fpath=%s", fpath );

    mas_free( fpath );
    DUF_TRACE( sampupd, 0, "(%p) context=%p", ( void * ) pdi, pdi->levinfo[pdi->depth - 1].context );
  }

  DUF_TRACE( sampupd, 1, "filename=%s", filename );
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}

/* callback of type duf_scan_callback_dir_t */
static int
scan_node_before( unsigned long long pathid, /* const duf_dirhandle_t * pdh_notused, */ duf_depthinfo_t * pdi,
                                duf_record_t * precord )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  DUF_TRACE_SAMPLE( 1, "T1 pathid=%llu", pathid );
  /* DUF_TRACE_SAMPLE( 0, "T0 pathid=%llu", pathid ); */
  {
    char *path = duf_pathid_to_path_s( pathid, pdi, &r );


    printf( "#%4llu: sampupd BEFORE dPATH %s\n", pdi->seq, path );
    DUF_TRACE_SAMPLE( 1, "path=%s", path );
    mas_free( path );
  }

  {
    char *test = mas_strdup( "HELLO" );

    pdi->levinfo[pdi->depth].context = ( void * ) test;
  }
  DUF_TRACE( sampupd, 0, "(%p) context=%p", ( void * ) pdi, pdi->levinfo[pdi->depth].context );
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}

static int
scan_node_after( unsigned long long pathid, /* const duf_dirhandle_t * pdh_notused, */ duf_depthinfo_t * pdi,
                                      duf_record_t * precord )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  DUF_TRACE( sampupd, 0, "@@@@@@@@@@@@@@ %llu -- %llu", pathid, pdi->levinfo[pdi->depth].dirid );
  DUF_TRACE_SAMPLE( 2, "T2 pathid=%llu", pathid );
  {
    char *path = duf_pathid_to_path_s( pathid, pdi, &r );


    printf( "#%4llu: sampupd AFTER  dPATH %s\n", pdi->seq, path );
    DUF_TRACE_SAMPLE( 1, "path=%s", path );
    mas_free( path );
  }

  DUF_TRACE( sampupd, 0, "(%p) context=%p", ( void * ) pdi, pdi->levinfo[pdi->depth].context );
  mas_free( pdi->levinfo[pdi->depth].context );
  pdi->levinfo[pdi->depth].context = NULL;
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}

static int
scan_node_middle( unsigned long long pathid, /* const duf_dirhandle_t * pdh_notused, */ duf_depthinfo_t * pdi,
                                       duf_record_t * precord )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  DUF_TRACE_SAMPLE( 2, "T2 pathid=%llu", pathid );
  {
    char *path = duf_pathid_to_path_s( pathid, pdi, &r );


    printf( "#%4llu: sampupd MIDDLE dPATH %s\n", pdi->seq, path );
    DUF_TRACE_SAMPLE( 1, "path=%s", path );
    mas_free( path );
  }

  DUF_TRACE( sampupd, 0, "(%p) context=%p", ( void * ) pdi, pdi->levinfo[pdi->depth].context );
  mas_free( pdi->levinfo[pdi->depth].context );
  pdi->levinfo[pdi->depth].context = NULL;
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}



duf_scan_callbacks_t duf_sampupd_callbacks = {
  .title = __FILE__,
  .init_scan = NULL,
  .node_scan_before = scan_node_before,
  .node_scan_after = scan_node_after,
  .node_scan_middle = scan_node_middle,
  .leaf_scan = scan_leaf,
  .fieldset =
        " duf_filenames.pathid as dirid " " ,duf_filenames.name as filename, duf_filedatas.size as filesize"
        " , uid, gid, nlink, inode, mtim as mtime " " , dupcnt as nsame"
        " , duf_filenames.id as filenameid" " , duf_filedatas.mode as filemode, md.md5sum1, md.md5sum2 ",
  .leaf_selector =
        "SELECT %s FROM duf_filenames "
        " JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) "
        " LEFT JOIN duf_md5 as md on (md.id=duf_filedatas.md5id)" "    WHERE "
        /* "           duf_filedatas.size >= %llu AND duf_filedatas.size < %llu "             */
        /* "       AND (md.dupcnt IS NULL OR (md.dupcnt >= %llu AND md.dupcnt < %llu))  AND " */
        " duf_filenames.pathid='%llu' ",
  .node_selector =
        "SELECT duf_paths.id as dirid, duf_paths.dirname, duf_paths.dirname as dfname,  duf_paths.parentid "
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize "
        /* " ,(SELECT count(*) FROM duf_paths as subpaths WHERE subpaths.parentid=duf_paths.id) as ndirs "       */
        /* " ,(SELECT count(*) FROM duf_filenames as sfn "                                                       */
        /* "          JOIN duf_filedatas as sfd ON (sfn.dataid=sfd.id) "                                         */
        /* "          JOIN duf_md5 as smd ON (sfd.md5id=smd.id) "                                                */
        /* "          WHERE sfn.pathid=duf_paths.id "                                                            */
        /* "              AND   sfd.size >= %llu AND sfd.size < %llu "                                           */
        /* "              AND (smd.dupcnt IS NULL OR (smd.dupcnt >= %llu AND smd.dupcnt < %llu)) "               */
        /* " ) as nfiles "                                                                                       */
        /* " ,(SELECT min(sfd.size) FROM duf_filedatas as sfd JOIN duf_filenames as sfn ON (sfn.dataid=sfd.id) " */
        /* "           WHERE sfn.pathid=duf_paths.id) as minsize "                                               */
        /* " ,(SELECT max(sfd.size) FROM duf_filedatas as sfd JOIN duf_filenames as sfn ON (sfn.dataid=sfd.id) " */
        /* "           WHERE sfn.pathid=duf_paths.id) as maxsize "                                               */
        " FROM duf_paths "
        " LEFT JOIN duf_pathtot_dirs AS td ON (td.pathid=duf_paths.id) "
        " LEFT JOIN duf_pathtot_files AS tf ON (tf.pathid=duf_paths.id) " " WHERE duf_paths.parentid='%llu' ",
  /* .final_sql_argv = final_sql, */
};
