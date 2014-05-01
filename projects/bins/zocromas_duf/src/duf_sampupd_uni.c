#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"

#include "duf_path.h"
/* #include "duf_file.h" */

#include "duf_sql_field.h"

#include "duf_dbg.h"

/* ###################################################################### */
/* #include "duf_sampupd_uni.h" */
/* ###################################################################### */



/* callback of type duf_scan_hook_file_t */
static int
sampupd_scan_leaf( duf_depthinfo_t * pdi, duf_record_t * precord /*, const duf_dirhandle_t * pdh_unused */  )
{
  int r = 0;

  /* DUF_SFIELD( filename ); */

  DEBUG_START(  );

  DEBUG_ENDR( r );
  return r;
}

/* callback of type duf_scan_callback_dir_t */
static int
sampupd_scan_node_before( unsigned long long pathid, /* const duf_dirhandle_t * pdh_unused, */ duf_depthinfo_t * pdi,
                          duf_record_t * precord )
{
  int r = 0;

  /* DUF_SFIELD( filename ); */

  DEBUG_START(  );

  DEBUG_ENDR( r );
  return r;
}

static int
sampupd_scan_node_after( unsigned long long pathid, /* const duf_dirhandle_t * pdh_unused, */ duf_depthinfo_t * pdi,
                         duf_record_t * precord )
{
  int r = 0;

  /* DUF_SFIELD( filename ); */

  DEBUG_START(  );


  DEBUG_ENDR( r );
  return r;
}

static int
sampupd_scan_node_middle( unsigned long long pathid, /* const duf_dirhandle_t * pdh_unused, */ duf_depthinfo_t * pdi,
                          duf_record_t * precord )
{
  int r = 0;

  DEBUG_START(  );


  DEBUG_ENDR( r );
  return r;
}





static char *final_sql[] = {
  "UPDATE duf_md5 SET dupcnt=(SELECT COUNT(*) "
        " FROM duf_filedatas AS fd "
        " JOIN duf_md5 AS md ON (fd.md5id=md.id) " " WHERE duf_md5.md5sum1=md.md5sum1 AND duf_md5.md5sum2=md.md5sum2)",
  /* "DELETE FROM duf_pathtot_files", */
  "INSERT OR IGNORE INTO duf_pathtot_files (pathid, numfiles, minsize, maxsize) "
        " SELECT fn.id AS pathid, COUNT(*) AS numfiles, min(size) AS minsize, max(size) AS maxsize "
        " FROM duf_filenames AS fn " " JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) " " GROUP BY fn.pathid",
  "UPDATE duf_pathtot_files SET "
        " minsize=(SELECT min(size) AS minsize "
        " FROM duf_filenames AS fn JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) "
        " WHERE duf_pathtot_files.pathid=fn.pathid) "
        ", maxsize=(SELECT max(size) AS maxsize "
        " FROM duf_filenames AS fn JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) "
        " WHERE duf_pathtot_files.pathid=fn.pathid) "
        ", numfiles=(SELECT COUNT(*) AS numfiles "
        " FROM duf_filenames AS fn JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) " " WHERE duf_pathtot_files.pathid=fn.pathid)",
  "INSERT OR IGNORE INTO duf_pathtot_dirs (pathid, numdirs) "
        " SELECT p.id AS pathid, COUNT(*) AS numdirs "
        " FROM duf_paths AS p " " LEFT JOIN duf_paths AS sp ON (sp.parentid=p.id) " " GROUP BY sp.parentid",
  "UPDATE duf_pathtot_dirs SET "
        " numdirs=(SELECT COUNT(*) AS numdirs " " FROM duf_paths AS p " " WHERE p.parentid=duf_pathtot_dirs.pathid )",
  /* "DELETE FROM duf_keydata", */
  "INSERT OR REPLACE INTO duf_keydata (md5id, filenameid, dataid, pathid) "
        "SELECT md.id AS md5id, fn.id AS filenameid, fd.id AS dataid, p.id AS pathid "
        " FROM duf_filenames AS fn "
        " JOIN duf_filedatas AS fd ON (fn.dataid=fd.id)"
        " JOIN duf_paths AS p ON (fn.pathid=p.id)" " JOIN duf_md5 AS md ON (fd.md5id=md.id)",

  /* *INDENT-ON*  */

  NULL,
};




duf_scan_callbacks_t duf_sampupd_callbacks = {
  .title = __FILE__,
  .init_scan = NULL,
  .node_scan_before = sampupd_scan_node_before,
  .node_scan_after = sampupd_scan_node_after,
  .node_scan_middle = sampupd_scan_node_middle,
  .leaf_scan = sampupd_scan_leaf,
  .fieldset =
        " duf_filenames.pathid AS dirid " " ,duf_filenames.name AS filename, duf_filedatas.size AS filesize"
        " , uid, gid, nlink, inode, mtim AS mtime " " , dupcnt AS nsame"
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
  .final_sql_argv = final_sql,
};
