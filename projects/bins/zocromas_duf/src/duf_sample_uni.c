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

#include "duf_pdi.h"
#include "duf_levinfo.h"

#include "duf_path.h"
/* #include "duf_file.h" */

/* #include "duf_sql_def.h" */
/* #include "duf_sql.h" */
#include "duf_sql_field.h"

#include "duf_dbg.h"

/* ###################################################################### */
/* #include "duf_sample_uni.h" */
/* ###################################################################### */



/* callback of type duf_scan_hook_file_t */
static int
sample_scan_leaf( duf_depthinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;

  DUF_SFIELD( filename );

  DEBUG_START(  );
/* stat */

  /* SQL at duf_file_pathid.c : duf_scan_fil_by_pi */

  /* 
   * --uni-scan   -R   --sample   --files   -FF
   *                   ^^^^^^^^   ^^^^^^^
   * */




  DUF_TRACE( sample, 2, "sample" );

  if ( pdi->depth <= 0 )
  {
    DUF_ERROR( "depth should not be %d at this point", pdi->depth );
    assert( pdi->depth > 0 );
  }
  {
    const char *fpath;

    DUF_SFIELD( filename );

    fpath = duf_levinfo_path( pdi );
    DUF_TRACE( sample, 0, "@@@@@@@@@ (F%d) '%s'/'%s'", pdi->depth, fpath, filename );
    DUF_PRINTF( 1, "#%4llu: L%u%12s%s%s", pdi->seq, duf_pdi_depth( pdi ), "", fpath ? fpath : "?/", filename );
  }

  DUF_TRACE( sample, 2, "filename=%s", filename );
  DEBUG_ENDR( r );
  return r;
}

/* callback of type duf_scan_callback_dir_t */
/* will be static! */
int
sample_scan_node_before( unsigned long long pathid, /* const duf_dirhandle_t * pdh_unused, */ duf_depthinfo_t * pdi,
                         duf_record_t * precord )
{
  int r = 0;

  DEBUG_START(  );


  {
    const char *path = duf_levinfo_path( pdi );

    DUF_PRINTF( 2, "#%4llu: L%u+ id%-7llu %s", pdi->seq, duf_pdi_depth( pdi ), pathid, path );
  }

  if ( !pdi->levinfo[pdi->depth].context )
  {
    char *test = mas_strdup( "BEFORE" );

    pdi->levinfo[pdi->depth].context = ( void * ) test;
  }
  DEBUG_ENDR( r );
  return r;
}

static int
sample_scan_node_after( unsigned long long pathid, /* const duf_dirhandle_t * pdh_unused, */ duf_depthinfo_t * pdi,
                        duf_record_t * precord )
{
  int r = 0;

  DEBUG_START(  );

  {
    {
      const char *path = duf_levinfo_path( pdi );

      DUF_PRINTF( 4, "#%4llu: L%u- id%-7llu %s", pdi->seq, duf_pdi_depth( pdi ), pathid, path );
    }
  }
  if ( !pdi->levinfo[pdi->depth].context || 0 != strcmp( ( char * ) pdi->levinfo[pdi->depth].context, "MIDDLE" ) )
    DUF_ERROR( "sample context %s", ( char * ) pdi->levinfo[pdi->depth].context );
  DUF_TRACE( sample, 0, "(%p) context=%p", ( void * ) pdi, pdi->levinfo[pdi->depth].context );

  mas_free( pdi->levinfo[pdi->depth].context );


  pdi->levinfo[pdi->depth].context = NULL;
  DEBUG_ENDR( r );
  return r;
}

static int
sample_scan_node_middle( unsigned long long pathid, /* const duf_dirhandle_t * pdh_unused, */ duf_depthinfo_t * pdi,
                         duf_record_t * precord )
{
  int r = 0;

  DEBUG_START(  );

  DUF_TRACE_SAMPLE( 2, "T2 pathid=%llu", pathid );
  {
    duf_config->cli.trace.sql--;
    {
      const char *real_path = NULL;

      if ( !real_path )
        real_path = duf_levinfo_path( pdi );
      /* char *path = duf_pathid_to_path_s( pathid, pdi, &r ); */

      DUF_PRINTF( 5, "#%4llu: sample MIDDLE dPATH %s", pdi->seq, real_path );
      DUF_TRACE_SAMPLE( 1, "path=%s", real_path );
      /* mas_free( path ); */
    }
    duf_config->cli.trace.sql++;
  }
  DUF_TRACE_SAMPLE( 2, "T2 pathid=%llu", pathid );


  if ( !pdi->levinfo[pdi->depth].context || 0 != strcmp( ( char * ) pdi->levinfo[pdi->depth].context, "BEFORE" ) )
    DUF_ERROR( "sample context %s", ( char * ) pdi->levinfo[pdi->depth].context );

  mas_free( pdi->levinfo[pdi->depth].context );
  pdi->levinfo[pdi->depth].context = NULL;

  if ( !pdi->levinfo[pdi->depth].context )
  {
    char *test = mas_strdup( "MIDDLE" );

    pdi->levinfo[pdi->depth].context = ( void * ) test;
  }


  DEBUG_ENDR( r );
  return r;
}



duf_scan_callbacks_t duf_sample_callbacks = {
  .title = __FILE__,
  /* .opendir = 1, */
  .init_scan = NULL,
  .node_scan_before = sample_scan_node_before,
  .node_scan_after = sample_scan_node_after,
  .node_scan_middle = sample_scan_node_middle,
  .leaf_scan = sample_scan_leaf,
  .fieldset =
        " duf_filenames.pathid AS dirid " " ,duf_filenames.name AS filename, duf_filedatas.size AS filesize"
        " , uid, gid, nlink, inode, mtim AS mtime " " , dupcnt AS nsame "
        " , duf_filenames.id AS filenameid" " , duf_filedatas.mode AS filemode, md.md5sum1, md.md5sum2 ",
  .leaf_selector =
        "SELECT %s FROM duf_filenames "
        " JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) "
        " LEFT JOIN duf_md5 AS md on (md.id=duf_filedatas.md5id)" "    WHERE "
        /* "           duf_filedatas.size >= %llu AND duf_filedatas.size < %llu "             */
        /* "       AND (md.dupcnt IS NULL OR (md.dupcnt >= %llu AND md.dupcnt < %llu))  AND " */
        " duf_filenames.pathid='%llu' ",
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
  /* .final_sql_argv = final_sql, */
};
