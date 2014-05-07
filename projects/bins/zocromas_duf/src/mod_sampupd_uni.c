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

/* 
 * this is callback of type: duf_scan_hook_dir_t
 * */
static int
sampupd_scan_node_before( unsigned long long pathid_unused, /* const duf_dirhandle_t * pdh_unused, */ duf_depthinfo_t * pdi,
                          duf_record_t * precord )
{
  int r = 0;

  /* DUF_SFIELD( filename ); */

  DEBUG_START(  );

  DEBUG_ENDR( r );
  return r;
}

/* 
 * this is callback of type: duf_scan_hook_dir_t
 * */
static int
sampupd_scan_node_after( unsigned long long pathid_unused, /* const duf_dirhandle_t * pdh_unused, */ duf_depthinfo_t * pdi,
                         duf_record_t * precord )
{
  int r = 0;

  /* DUF_SFIELD( filename ); */

  DEBUG_START(  );


  DEBUG_ENDR( r );
  return r;
}

/* 
 * this is callback of type: duf_scan_hook_dir_t
 * */
static int
sampupd_scan_node_middle( unsigned long long pathid_unused, /* const duf_dirhandle_t * pdh_unused, */ duf_depthinfo_t * pdi,
                          duf_record_t * precord )
{
  int r = 0;

  DEBUG_START(  );


  DEBUG_ENDR( r );
  return r;
}





static char *final_sql[] = {
  "UPDATE duf_md5 SET dupcnt=(SELECT COUNT(*) " /*	*/
        " FROM duf_filedatas AS fd " /*	*/
        " JOIN duf_md5 AS md ON (fd.md5id=md.id) " /*	*/
	" WHERE duf_md5.md5sum1=md.md5sum1 AND duf_md5.md5sum2=md.md5sum2)",
  /* "DELETE FROM duf_pathtot_files", */
  "INSERT OR IGNORE INTO duf_pathtot_files (Pathid, numfiles, minsize, maxsize) " /*	*/
        "SELECT fn.Pathid AS Pathid, COUNT(*) AS numfiles, min(size) AS minsize, max(size) AS maxsize " /*	*/
        " FROM duf_filenames AS fn " /*	*/
	" LEFT JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) " /*	*/
	" GROUP BY fn.Pathid",
  "UPDATE duf_pathtot_files SET " /*	*/
        " minsize=(SELECT min(size) AS minsize " /*	*/
        " FROM duf_filenames AS fn JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) " /*	*/
        " WHERE duf_pathtot_files.Pathid=fn.Pathid) " /*	*/
        ", maxsize=(SELECT max(size) AS maxsize " /*	*/
        " FROM duf_filenames AS fn JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) " /*	*/
        " WHERE duf_pathtot_files.Pathid=fn.Pathid) " /*	*/
        ", numfiles=(SELECT COUNT(*) AS numfiles " /*	*/
        " FROM duf_filenames AS fn JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) " /*	*/
	" WHERE duf_pathtot_files.Pathid=fn.Pathid)",
  "INSERT OR IGNORE INTO duf_pathtot_dirs (Pathid, numdirs) " /*	*/
        "SELECT p.id AS Pathid, COUNT(*) AS numdirs " /*	*/
        " FROM duf_paths AS p " /*	*/
	" LEFT JOIN duf_paths AS sp ON (sp.parentid=p.id) " /*	*/
	" GROUP BY sp.parentid",
  "UPDATE duf_pathtot_dirs SET " /*	*/
        " numdirs=(SELECT COUNT(*) AS numdirs " /*	*/
	" FROM duf_paths AS p " /*	*/
	" WHERE p.parentid=duf_pathtot_dirs.Pathid )",
  /* "DELETE FROM duf_keydata", */
  "INSERT OR REPLACE INTO duf_keydata (md5id, filenameid, dataid, Pathid) " /*	*/
        "SELECT md.id AS md5id, fn.id AS filenameid, fd.id AS dataid, p.id AS Pathid " /*	*/
        " FROM duf_filenames AS fn " /*	*/
        " LEFT JOIN duf_filedatas AS fd ON (fn.dataid=fd.id)" /*	*/
        " JOIN duf_paths AS p ON (fn.Pathid=p.id)" /*	*/
	" JOIN duf_md5 AS md ON (fd.md5id=md.id)",

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
        "fn.Pathid AS dirid, fn.name AS filename, fd.size AS filesize" /*	*/
        " , uid, gid, nlink, inode, mtim AS mtime " /*	*/
	" , dupcnt AS nsame" /*	*/
        " , fn.id AS filenameid, fd.mode AS filemode, md.md5sum1, md.md5sum2 ",
  .leaf_selector =
        "SELECT %s FROM duf_filenames AS fn " /*	*/
        " LEFT JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) " /*	*/
        " LEFT JOIN duf_md5 AS md ON (md.id=fd.md5id)" /*	*/
	"    WHERE " /*	*/
        /* "           fd.size >= %llu AND fd.size < %llu "             */
        /* "       AND (md.dupcnt IS NULL OR (md.dupcnt >= %llu AND md.dupcnt < %llu))  AND " */
        " fn.Pathid='%llu' ",
  .node_selector =
        "SELECT     pt.id AS dirid, pt.dirname, pt.dirname AS dfname,  pt.parentid " /*	*/
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /*	*/
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
        " FROM     duf_paths AS pt " /*	*/
        " LEFT JOIN duf_pathtot_dirs AS td ON (td.Pathid=pt.id) " /*	*/
        " LEFT JOIN duf_pathtot_files AS tf ON (tf.Pathid=pt.id) " /*	*/
	" WHERE pt.parentid='%llu' ",
  .final_sql_argv = final_sql,
};
