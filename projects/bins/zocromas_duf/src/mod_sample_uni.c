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
    DUF_PRINTF( 1, "#%4llu: L%2u%12s%s%s", pdi->seq, duf_pdi_depth( pdi ), "", fpath ? fpath : "?/", filename );
  }

  DUF_TRACE( sample, 2, "filename=%s", filename );
  DEBUG_ENDR( r );
  return r;
}

/* 
 * this is callback of type: duf_scan_hook_dir_t
 * */
/* will be static! */
int
sample_scan_node_before( unsigned long long pathid_unused, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;
  unsigned long long dirid = duf_levinfo_dirid( pdi );

  DEBUG_START(  );


  {
    const char *path = duf_levinfo_path( pdi );

    DUF_PRINTF( 2, "#%4llu: " DUF_DEPTH_PFMT "+ id%-7llu %s", pdi->seq, duf_pdi_depth( pdi ), dirid, path );
  }
  {
    void *ctx = duf_levinfo_context( pdi );

    if ( !ctx )
    {
      char *test = mas_strdup( "BEFORE" );

      duf_levinfo_set_context( pdi, test );
    }
  }
  DEBUG_ENDR( r );
  return r;
}

static int
sample_scan_node_after( unsigned long long pathid_unused, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;

  DEBUG_START(  );

  {
    unsigned long long dirid = duf_levinfo_dirid( pdi );
    const char *path = duf_levinfo_path( pdi );

    DUF_PRINTF( 4, "#%4llu: " DUF_DEPTH_PFMT "- id%-7llu %s", pdi->seq, duf_pdi_depth( pdi ), dirid, path );
  }
  {
    void *ctx = duf_levinfo_context( pdi );

    if ( !ctx || 0 != strcmp( ( char * ) ctx, "MIDDLE" ) )
      DUF_ERROR( "sample context %s", ( char * ) ctx );
    DUF_TRACE( sample, 0, "(%p) context=%p", ( void * ) pdi, ctx );
    duf_levinfo_set_context( pdi, NULL );
  }
  DEBUG_ENDR( r );
  return r;
}

static int
sample_scan_node_middle( unsigned long long pathid_unused, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;
  unsigned long long dirid = duf_levinfo_dirid( pdi );

  DEBUG_START(  );

  DUF_TRACE( sample, 2, "T2 dirid=%llu", dirid );
  {
    duf_config->cli.trace.sql--;
    {
      const char *real_path = NULL;

      if ( !real_path )
        real_path = duf_levinfo_path( pdi );
      /* char *path = duf_pathid_to_path_s( dirid, pdi, &r ); */

      DUF_PRINTF( 5, "#%4llu: sample MIDDLE dPATH %s", pdi->seq, real_path );
      DUF_TRACE( sample, 1, "path=%s", real_path );
      /* mas_free( path ); */
    }
    duf_config->cli.trace.sql++;
  }
  DUF_TRACE( sample, 2, "T2 dirid=%llu", dirid );

  {
    void *ctx = duf_levinfo_context( pdi );

    if ( !ctx || 0 != strcmp( ( char * ) ctx, "BEFORE" ) )
      DUF_ERROR( "sample context %s", ( char * ) ctx );

    duf_levinfo_set_context( pdi, NULL );
    ctx = duf_levinfo_context( pdi );
    if ( !ctx )
    {
      char *test = mas_strdup( "MIDDLE" );

      duf_levinfo_set_context( pdi, test );
    }
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
  .fieldset = "fd.Pathid AS dirid " /* */
        " ,fd.name AS filename, fd.size AS filesize" /* */
        " , uid, gid, nlink, inode, mtim AS mtime " /* */
        " , dupcnt AS nsame "   /* */
        " , fd.id AS filenameid" /* */
        " , fd.mode AS filemode, md.md5sum1, md.md5sum2 ",
  .leaf_selector = "SELECT %s FROM duf_filenames AS fn " /* */
        " LEFT JOIN duf_filedatas AS fd ON (fd.dataid=fd.id) " /* */
        " LEFT JOIN duf_md5 AS md ON (md.id=fd.md5id)" /* */
        "    WHERE "            /* */
        /* "           fd.size >= %llu AND fd.size < %llu "             */
        /* "       AND (md.dupcnt IS NULL OR (md.dupcnt >= %llu AND md.dupcnt < %llu))  AND " */
        " fd.Pathid='%llu' ",
  .node_selector = "SELECT     pt.id AS dirid, pt.dirname, pt.dirname AS dfname,  pt.parentid " /* */
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
        /* " ,(SELECT count(*) FROM duf_paths AS subpaths WHERE subpaths.parentid=pt.id) AS ndirs "       */
        /* " ,(SELECT count(*) FROM duf_filenames AS sfn "                                                       */
        /* "          JOIN duf_filedatas AS sfd ON (sfn.dataid=sfd.id) "                                         */
        /* "          JOIN duf_md5 AS smd ON (sfd.md5id=smd.id) "                                                */
        /* "          WHERE sfn.Pathid=pt.id "                                                            */
        /* "              AND   sfd.size >= %llu AND sfd.size < %llu "                                           */
        /* "              AND (smd.dupcnt IS NULL OR (smd.dupcnt >= %llu AND smd.dupcnt < %llu)) "               */
        /* " ) AS nfiles "                                                                                       */
        /* " ,(SELECT min(sfd.size) FROM duf_filedatas AS sfd JOIN duf_filenames AS sfn ON (sfn.dataid=sfd.id) " */
        /* "           WHERE sfn.Pathid=pt.id) AS minsize "                                               */
        /* " ,(SELECT max(sfd.size) FROM duf_filedatas AS sfd JOIN duf_filenames AS sfn ON (sfn.dataid=sfd.id) " */
        /* "           WHERE sfn.Pathid=pt.id) AS maxsize "                                               */
        " FROM duf_paths AS pt " /* */
        " LEFT JOIN duf_pathtot_dirs AS td ON (td.Pathid=pt.id) " /* */
        " LEFT JOIN duf_pathtot_files AS tf ON (tf.Pathid=pt.id) " /* */
        " WHERE pt.parentid='%llu' ",
  /* .final_sql_argv = final_sql, */
};
