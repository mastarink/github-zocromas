#define DUF_SQL_PDI_STMT

#include <assert.h>




#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"



#include "duf_hook_types.h"

#include "duf_utils.h"
#include "duf_dbg.h"
#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"
#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_filedata.h"

#include "duf_path.h"



/* static int                                                                                                                  */
/* directories_entry_dir( const char *fname, const struct stat *pstat, unsigned long long dirid, duf_depthinfo_t * pdi,        */
/*                        duf_record_t * precord )                                                                             */
/* {                                                                                                                           */
/*   int r = 0;                                                                                                                */
/*   int changes = 0;                                                                                                          */
/*                                                                                                                             */
/*   DUF_TRACE( scan, 0, "@@@@@@@@@@@@@ scan entry dir by %s", fname );                                                        */
/*   ( void ) duf_insert_path_uni2( pdi, fname, 1 (* ifadd *) , pstat->st_dev, pstat->st_ino, 0 (*need_id *) , &changes, &r ); */
/*   DUF_TEST_R( r );                                                                                                          */
/*   return r;                                                                                                                 */
/* }                                                                                                                           */

static int
directories_entry_dir2(  /* duf_sqlite_stmt_t * pstmt_unused, */ const char *fname, const struct stat *pstat, unsigned long long dirid,
                        duf_depthinfo_t * pdi )
{
  int r = 0;
  int changes = 0;

  /* DUF_TRACE( scan, 0, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ IN scan entry dir2 by %s", fname ); */
  ( void ) duf_insert_path_uni2( pdi, fname, 1 /* ifadd */ , pstat->st_dev, pstat->st_ino, 0 /*need_id */ , &changes, &r );
  DUF_TEST_R( r );
  return r;
}






static const char *final_sql[] = {
  "INSERT OR IGNORE INTO " DUF_DBPREF "pathtot_files (Pathid, numfiles, minsize, maxsize) " /* */
        "SELECT fn.Pathid AS Pathid, COUNT(*) AS numfiles, min(size) AS minsize, max(size) AS maxsize " /* */
        " FROM " DUF_DBPREF "filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
        " GROUP BY fn.Pathid",
  "DELETE FROM " DUF_DBPREF "sizes",
  "INSERT OR IGNORE INTO " DUF_DBPREF "sizes (size, dupzcnt) " /* */
        "SELECT size, COUNT(*) " /* */
        " FROM " DUF_DBPREF "filedatas AS fd GROUP BY fd.size",
  "UPDATE " DUF_DBPREF "pathtot_files SET " /* */
        " minsize=(SELECT min(size) AS minsize " /* */
        " FROM " DUF_DBPREF "filenames AS fn JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
        " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid) " /* */
        ", maxsize=(SELECT max(size) AS maxsize " /* */
        " FROM " DUF_DBPREF "filenames AS fn JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
        " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid) " /* */
        ", numfiles=(SELECT COUNT(*) AS numfiles " /* */
        " FROM " DUF_DBPREF "filenames AS fn JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
        " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid)",

  "INSERT OR IGNORE INTO " DUF_DBPREF "pathtot_dirs (Pathid, numdirs) " /* */
        "SELECT parents." DUF_SQL_IDNAME " AS Pathid, COUNT(*) AS numdirs " /* */
        " FROM " DUF_DBPREF "paths " /* */
        " JOIN " DUF_DBPREF "paths AS parents ON (parents." DUF_SQL_IDNAME "=paths.parentid) " /* */
        " GROUP BY parents." DUF_SQL_IDNAME "" /* */
        ,

  "UPDATE " DUF_DBPREF "pathtot_dirs SET " /* */
        " numdirs=(SELECT COUNT(*) AS numdirs " /* */
        " FROM " DUF_DBPREF "paths AS p " /* */
        " WHERE p.parentid=" DUF_DBPREF "pathtot_dirs.Pathid )",
  /* "DELETE FROM " DUF_DBPREF "keydata", */
  /* "INSERT OR REPLACE INTO " DUF_DBPREF "keydata (md5id, filenameid, dataid, Pathid) " (* *)  */
  /*       "SELECT md." DUF_SQL_IDNAME " AS md5id, fn." DUF_SQL_IDNAME " AS filenameid, fd." DUF_SQL_IDNAME " AS dataid, p." DUF_SQL_IDNAME " AS Pathid " (* *) */
  /*       " FROM " DUF_DBPREF "filenames AS fn " (* *)                                         */
  /*       " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ")" (* *)                */
  /*       " JOIN " DUF_DBPREF "paths AS p ON (fn.Pathid=p." DUF_SQL_IDNAME ")" (* *)                           */
  /*       " JOIN " DUF_DBPREF "md5 AS md ON (fd.md5id=md." DUF_SQL_IDNAME ")",                                 */


  NULL,
};



duf_scan_callbacks_t duf_directories_callbacks = {
  .title = "directories",
  .init_scan = NULL,
  .def_opendir = 1,
  .scan_mode_2 = 1,
  /* .dirent_dir_scan_before = directories_entry_dir, */
  .dirent_dir_scan_before2 = directories_entry_dir2,


  /* filename for debug only */
  .leaf_fieldset = "fn.Pathid AS dirid, fn.name AS filename, fd.size AS filesize " /* */
        ", uid, gid, nlink, inode, mtim AS mtime " /* */
        ", fd.mode AS filemode " /* */
        ", fn." DUF_SQL_IDNAME " AS filenameid " /* */
        ", md.dup5cnt AS nsame, md.md5sum1, md.md5sum2 " /* */
        ", fd.md5id AS md5id"   /* */
        ,
  /* .leaf_selector = "SELECT %s FROM " DUF_DBPREF "filenames AS fn " (* *)                       */
  /*       " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON ( fn.dataid = fd." DUF_SQL_IDNAME " ) " (* *)             */
  /*       " LEFT JOIN " DUF_DBPREF "md5 AS md ON ( md." DUF_SQL_IDNAME " = fd.md5id ) " (* *)                    */
  /*       " WHERE "               (* *)                                                          */
  /*       (* " fd.size >= %llu AND fd.size < %llu "                      *)                      */
  /*       (* " AND( md.dup5cnt IS NULL OR( md.dup5cnt >= %llu AND md.dup5cnt < %llu ) ) AND " *) */
  /*       " fn.Pathid = '%llu' ",                                                                */
  .leaf_selector2 =             /* */
        /* "SELECT %s " */
        " FROM " DUF_DBPREF "filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON ( fn.dataid = fd." DUF_SQL_IDNAME " ) " /* */
        " LEFT JOIN " DUF_DBPREF "md5 AS md ON ( md." DUF_SQL_IDNAME " = fd.md5id ) " /* */
        " WHERE "               /* */
        " fn.Pathid = :dirID "  /* */
        " AND (:minSize IS NULL OR fd.size>=:minSize) AND (:maxSize IS NULL OR fd.size<=:maxSize) " /* */
        " AND (:minSame IS NULL OR md.dup5cnt>=:minSame) AND (:maxSame IS NULL OR md.dup5cnt<=:maxSame) " /* */
        ,
  .leaf_selector_total2 =       /* */
        " FROM " DUF_DBPREF "filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
        " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id)" /* */
        "    WHERE "            /* */
        "     (:minSize IS NULL OR fd.size>=:minSize) AND (:maxSize IS NULL OR fd.size<=:maxSize) " /* */
        " AND (:minSame IS NULL OR md.dup5cnt>=:minSame) AND (:maxSame IS NULL OR md.dup5cnt<=:maxSame) " /* */
        ,
  .node_fieldset = "pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.parentid " /* */
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
        ,
  /* .node_selector =              (* *)                                                                                                         */
  /*       (* "SELECT     pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.parentid "                  *)                   */
  /*       (* ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " *)                                   */
  /*       (* ", ( SELECT count( * ) FROM " DUF_DBPREF "paths AS subpaths WHERE subpaths.parentid = pt." DUF_SQL_IDNAME " ) AS ndirs "        *) */
  /*       (* ", ( SELECT count( * ) FROM " DUF_DBPREF "filenames AS sfn "                                                           *)          */
  /*       (* " LEFT JOIN " DUF_DBPREF "filedatas AS sfd ON( sfn.dataid = sfd." DUF_SQL_IDNAME " ) "                                     *)      */
  /*       (* " JOIN " DUF_DBPREF "md5 AS smd ON( sfd.md5id = smd." DUF_SQL_IDNAME " ) "                                            *)           */
  /*       (* " WHERE sfn.Pathid = pt." DUF_SQL_IDNAME " "                                                         *)                            */
  /*       (* " AND sfd.size >= %llu AND sfd.size < %llu "                                                                *)                     */
  /*       (* " AND( smd.dup5cnt IS NULL OR( smd.dup5cnt >= %llu AND smd.dup5cnt < %llu ) ) "                                *)                  */
  /*       (* " ) AS nfiles "                                                                                             *)                     */
  /*       (* ", ( SELECT min( sfd.size ) FROM " DUF_DBPREF "filedatas AS sfd "                        *)                                        */
  /*       (*         "JOIN " DUF_DBPREF "filenames AS sfn ON( sfn.dataid = sfd." DUF_SQL_IDNAME " ) " *)                                        */
  /*       (* " WHERE sfn.Pathid = pt." DUF_SQL_IDNAME " ) AS minsize "                                                            *)            */
  /*       (* ", ( SELECT max( sfd.size ) FROM " DUF_DBPREF "filedatas AS sfd "                              *)                                  */
  /*       (*      "JOIN " DUF_DBPREF "filenames AS sfn ON( sfn.dataid = sfd." DUF_SQL_IDNAME " ) "          *)                                  */
  /*       (* " WHERE sfn.Pathid = pt." DUF_SQL_IDNAME " ) AS maxsize "                                                    *)                    */
  /*       " FROM " DUF_DBPREF "paths AS pt " (* *)                                                                                              */
  /*       " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " (* *)                                             */
  /*       " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " (* *)                                            */
  /*       " WHERE pt.parentid = '%llu' ",                                                                                                       */
  .node_selector2 =             /* */
        /* "SELECT     pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.parentid "                  */
        /* ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " */
        " FROM " DUF_DBPREF "paths AS pt " /* */
        " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
        " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
        " WHERE pt.parentid = :dirID ",
  .final_sql_argv = final_sql,
};
