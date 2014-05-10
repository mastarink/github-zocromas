#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

/* #include <dirent.h> */
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>


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
#include "duf_sql_field1.h"

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

duf_scan_callbacks_t duf_statistics_callbacks = {
  .title = __FILE__,
  .init_scan = NULL,
  .node_scan_before = scan_node_before,
  .leaf_scan = scan_leaf,
  .leaf_fieldset =
        "fn.Pathid AS dirid " /*	*/
        " , fn.name AS filename, fd.size AS filesize" /*	*/
        " , uid, gid, nlink, inode, mtim AS mtime " /*	*/
	" , dupcnt AS nsame" /*	*/
        " , fn.id AS filenameid" /*	*/
	" , fd.mode AS filemode",
  .leaf_selector =
        "SELECT %s FROM " DUF_DBPREF "filenames AS fn " /*	*/
        " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd.id) " /*	*/
        " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md.id=fd.md5id)" /*	*/
	"    WHERE " /*	*/
        /* "           fd.size >= %llu AND fd.size < %llu "            */
        /* "       AND (md.dupcnt IS NULL OR (md.dupcnt >= %llu AND md.dupcnt < %llu)) AND " */
        " fn.Pathid='%llu' ",
  .node_fieldset = "pt.id AS dirid, pt.dirname, pt.dirname AS dfname,  pt.ParentId " /* */
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
	,
  .node_selector =
        "SELECT     pt.id AS dirid, pt.dirname, pt.dirname AS dfname, pt.ParentId " /*	*/
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /*	*/
        /* " ,(SELECT count(*) FROM " DUF_DBPREF "paths AS subpaths WHERE subpaths.ParentId=pt.id) AS ndirs "       */
        /* " ,(SELECT count(*) FROM " DUF_DBPREF "filenames AS sfn "                                                       */
        /* "          JOIN " DUF_DBPREF "filedatas AS sfd ON (sfn.dataid=sfd.id) "                                         */
        /* "          JOIN " DUF_DBPREF "md5 AS smd ON (sfd.md5id=smd.id) "                                                */
        /* "          WHERE sfn.Pathid=pt.id "                                                            */
        /* "              AND   sfd.size >= %llu AND sfd.size < %llu "                                           */
        /* "              AND (smd.dupcnt IS NULL OR (smd.dupcnt >= %llu AND smd.dupcnt < %llu)) "               */
        /* " ) AS nfiles "                                                                                       */
        /* " ,(SELECT min(sfd.size) FROM " DUF_DBPREF "filedatas AS sfd JOIN " DUF_DBPREF "filenames AS sfn ON (sfn.dataid=sfd.id) " */
        /* "           WHERE sfn.Pathid=pt.id) AS minsize "                                               */
        /* " ,(SELECT max(sfd.size) FROM " DUF_DBPREF "filedatas AS sfd JOIN " DUF_DBPREF "filenames AS sfn ON (sfn.dataid=sfd.id) " */
        /* "           WHERE sfn.Pathid=pt.id) AS maxsize "                                               */
        " FROM " DUF_DBPREF "paths AS pt " /*	*/
        " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.Pathid=pt.id) " /*	*/
        " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt.id) " /*	*/
	" WHERE pt.ParentId='%llu' ",
  /* .final_sql_argv = final_sql, */
};
