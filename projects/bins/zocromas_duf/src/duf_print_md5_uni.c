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

/* #include "duf_sql_def.h" */
/* #include "duf_sql.h" */
#include "duf_sql_field.h"

#include "duf_dbg.h"

/* ###################################################################### */
/* #include "duf_print_md5_uni.h" */
/* ###################################################################### */



/* callback of type duf_scan_hook_file_t */
static int
scan_leaf( duf_depthinfo_t * pdi, duf_record_t * precord /*, const duf_dirhandle_t * pdh_unused */  )
{
  int r = 0;

  DUF_SFIELD( filename );
  /* const char *filename = duf_sql_str_by_name( "filename", precord, 0 ); */
  DUF_UFIELD( filesize );
  /* unsigned long long filesize = duf_sql_ull_by_name( "filesize", precord, 0 ); */

  DEBUG_START(  );
/* stat */

  /* SQL at duf_file_pathid.c : duf_scan_fil_by_pi */

  /* 
   * --uni-scan   -R   --print --md5  --files   -FF
   *                   ^^^^^^^        ^^^^^^^
   * */


  DUF_PRINTF( 0, "%6llu:%6llu:%6llu [%10llu] file: %s", pdi->seq, pdi->items.files, pdi->items.total, filesize, filename );

  DEBUG_ENDR( r );
  return r;
}

/* callback of type duf_scan_callback_dir_t */
/* __attribute__ ( ( unused ) ) */
static int
scan_node_before( unsigned long long pathid_unused, /* const duf_dirhandle_t * pdh_unused, */ duf_depthinfo_t * pdi,
                  duf_record_t * precord )
{
  int r = 0;

  DUF_SFIELD( dirname );
  /* const char *filename = duf_sql_str_by_name( "filename", precord, 0 ); */

  DEBUG_START(  );

  {
    /* char *path = duf_pathid_to_path_s( pathid_unused ); */


    DUF_PRINTF( 0, "------------------------------------- md5: %s", dirname );


    /* mas_free( path ); */
  }
  DEBUG_ENDR( r );
  return r;
}

/* #### duf_sql( "UPDATE duf_md5 SET dupcnt='%llu' WHERE id='%llu'", cnt, md5id ); */

  /* .node_selector =                                                                                             */
  /*       "SELECT duf_md5.id AS dirid, printf('%%016x%%016x',md5sum1,md5sum2) AS dirname "                      */
  /*       " ,0 AS ndirs" " ,(SELECT count(*) FROM duf_filenames AS subfilenames "                               */
  /*       (* " LEFT "  toooooooo slow with LEFT *)                                                              */
  /*       "            JOIN duf_filedatas ON (duf_filedatas.id=subfilenames.dataid) "                           */
  /*       "                            WHERE duf_filedatas.md5id=duf_md5.id) AS nfiles "                        */
  /*       (* ", (SELECT count(*) FROM duf_filedatas "                                                        *) */
  /*       (* "           LEFT JOIN duf_filenames AS subfilenames ON (duf_filedatas.id=subfilenames.dataid) " *) */
  /*       (* "                             WHERE duf_filedatas.md5id=duf_md5.id) AS nfiles"                  *) */
  /*       " FROM duf_md5 " " WHERE %llu<1 "                                                                     */
  /*       (* " AND nfiles>1 " *)                                                                                */
  /*       " ORDER BY md5sum1,md5sum2 ",                                                                         */


duf_scan_callbacks_t duf_print_md5_callbacks = {
  .title = __FILE__,
  /* .opendir = 1, */
  .init_scan = NULL,
  .node_scan_before = scan_node_before,
  .leaf_scan = scan_leaf,
  .fieldset = " md.id AS dirid "
        " , duf_filenames.name AS filename, duf_filedatas.size AS filesize "
        " , uid, gid, nlink, inode, mtim AS mtime "
        " , dupcnt AS nsame "
        " , printf('%016x%016x',md5sum1,md5sum2) AS dirname, duf_filedatas.size AS filesize " " , duf_filenames.pathid AS hid "
        " , duf_filenames.id AS filenameid" " , duf_filedatas.mode AS filemode",
  .leaf_selector = "SELECT %s FROM duf_filenames "
        "              JOIN duf_filedatas ON (duf_filedatas.id=duf_filenames.dataid) "
        "              LEFT JOIN duf_md5 AS md on (md.id=duf_filedatas.md5id)"
        "                           WHERE duf_filedatas.md5id='%llu' ORDER BY duf_filedatas.size ",
  .node_selector =
        "SELECT md5.id AS dirid " ", printf('%%016x%%016x',md5.md5sum1,md5.md5sum2) AS dirname"
        ", printf('%%016x%%016x',md5.md5sum1,md5.md5sum2) AS dfname " " ,0 AS ndirs"
        ", (SELECT COUNT(*) FROM duf_md5 AS md WHERE md5.md5sum1=md.md5sum1 AND md5.md5sum2=md.md5sum2 ) AS nfiles"
        ", (SELECT size FROM duf_filedatas AS fd WHERE md5.id=fd.md5id ) AS filesize"
        ", 0 AS maxsize, 0 AS minsize" " FROM duf_md5 AS md5" " WHERE %llu<1 " " ORDER BY filesize,md5sum1,md5sum2 ",
  /* .final_sql_argv = final_sql, */
};
