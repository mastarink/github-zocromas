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

#include "duf_levinfo.h"

#include "duf_path.h"
/* #include "duf_file.h" */

/* #include "duf_sql_def.h" */
/* #include "duf_sql.h" */
#include "duf_sql_field.h"

#include "duf_dbg.h"

/* ###################################################################### */
/* #include "duf_statistics_uni.h" */
/* ###################################################################### */



/* callback of type duf_scan_hook_file_t */
static int
scan_leaf( duf_depthinfo_t * pdi, duf_record_t * precord /*, const duf_dirhandle_t * pdh_unused */  )
{
  int r = 0;

  DUF_SFIELD( filename );
  /* const char *filename = duf_sql_str_by_name( "filename", precord, 0 ); */

  DEBUG_START(  );
/* stat */

  /* SQL at duf_file_pathid.c : duf_scan_fil_by_pi */

  /* 
   * --uni-scan   -R   --statistics   --files   -FF
   *                   ^^^^^^^^   ^^^^^^^
   * */

  /* {                                                              */
  /*   DUF_UFIELD( filenameid );                                    */
  /*   char *fpath = filenameid_to_filepath( filenameid, pdi, &r ); */
  /*                                                                */
  /*   printf( "#%4llu: statistics fpath %s\n", pdi->seq, fpath );  */
  /*                                                                */
  /*   DUF_TRACE( statistics, 1, "fpath=%s", fpath );               */
  /*                                                                */
  /*   mas_free( fpath );                                           */
  /* }                                                              */


  DUF_TRACE( statistics, 1, "filename=%s", filename );
  DEBUG_ENDR( r );
  return r;
}

/* 
 * this is callback of type: duf_scan_hook_dir_t
 * */
static int
scan_node_before( unsigned long long pathid_unused, /* const duf_dirhandle_t * pdh_unused, */ duf_depthinfo_t * pdi,
                  duf_record_t * precord )
{
  int r = 0;

  DEBUG_START(  );
  {
    const char *real_path = NULL;

    if ( !real_path )
      real_path = duf_levinfo_path( pdi );

    printf( "#%4llu: statistics dPATH %s\n", pdi->seq, real_path );
    DUF_TRACE( statistics, 1, "real_path=%s", real_path );
  }
  DEBUG_ENDR( r );
  return r;
}
/* *INDENT-OFF*  */
duf_scan_callbacks_t duf_statistics_callbacks = {
  .title = __FILE__,
  .init_scan = NULL,
  .node_scan_before = scan_node_before,
  .leaf_scan = scan_leaf,
  .fieldset =
        " duf_filenames.Pathid AS dirid "
        " , duf_filenames.name AS filename, duf_filedatas.size AS filesize"
        " , uid, gid, nlink, inode, mtim AS mtime " " , dupcnt AS nsame"
        " , duf_filenames.id AS filenameid" " , duf_filedatas.mode AS filemode",
  .leaf_selector =
        "SELECT %s FROM duf_filenames "
        " JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) "
        " LEFT JOIN duf_md5 AS md on (md.id=duf_filedatas.md5id)" "    WHERE "
        /* "           duf_filedatas.size >= %llu AND duf_filedatas.size < %llu "            */
        /* "       AND (md.dupcnt IS NULL OR (md.dupcnt >= %llu AND md.dupcnt < %llu)) AND " */
        " duf_filenames.Pathid='%llu' ",
  .node_selector =
        "SELECT duf_paths.id AS dirid, duf_paths.dirname, duf_paths.dirname AS dfname, duf_paths.parentid "
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize "
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
        " FROM duf_paths "
        " LEFT JOIN duf_pathtot_dirs AS td ON (td.Pathid=duf_paths.id) "
        " LEFT JOIN duf_pathtot_files AS tf ON (tf.Pathid=duf_paths.id) " " WHERE duf_paths.parentid='%llu' ",
  /* .final_sql_argv = final_sql, */
};
/* *INDENT-ON*  */
