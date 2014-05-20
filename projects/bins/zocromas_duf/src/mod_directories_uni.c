#define DUF_SQL_PDI_STMT

#include <assert.h>




#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_trace_defs.h"
#include "duf_debug_defs.h"



#include "duf_hook_types.h"
/* #include "duf_types.h" */

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



static int
directories_entry_dir( const char *fname, const struct stat *pstat, unsigned long long dirid, duf_depthinfo_t * pdi,
                       duf_record_t * precord )
{
  int r = 0;

  ( void ) duf_insert_path_uni( pdi, fname, pstat->st_dev, pstat->st_ino, dirid, 0 /*need_id */ , &r );
  DUF_TEST_R( r );
  return r;
}

static int
directories_entry_dir2( duf_sqlite_stmt_t * pstmt, const char *fname, const struct stat *pstat, unsigned long long dirid,
                        duf_depthinfo_t * pdi )
{
  int r = 0;

  DUF_TRACE( scan, 0, "scan entry dir2 by %s", fname );
  ( void ) duf_insert_path_uni( pdi, fname, pstat->st_dev, pstat->st_ino, dirid, 0 /*need_id */ , &r );
  DUF_TEST_R( r );
  return r;
}






static const char *final_sql[] = {
  "INSERT OR IGNORE INTO " DUF_DBPREF "pathtot_files (Pathid, numfiles, minsize, maxsize) " /* */
        "SELECT fn.Pathid AS Pathid, COUNT(*) AS numfiles, min(size) AS minsize, max(size) AS maxsize " /* */
        " FROM " DUF_DBPREF "filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd.id) " /* */
        " GROUP BY fn.Pathid",
  "DELETE FROM " DUF_DBPREF "sizes",
  "INSERT OR IGNORE INTO " DUF_DBPREF "sizes (size, dupzcnt) " /* */
        "SELECT size, COUNT(*) " /* */
        " FROM " DUF_DBPREF "filedatas AS fd GROUP BY fd.size",
  "UPDATE " DUF_DBPREF "pathtot_files SET " /* */
        " minsize=(SELECT min(size) AS minsize " /* */
        " FROM " DUF_DBPREF "filenames AS fn JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd.id) " /* */
        " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid) " /* */
        ", maxsize=(SELECT max(size) AS maxsize " /* */
        " FROM " DUF_DBPREF "filenames AS fn JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd.id) " /* */
        " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid) " /* */
        ", numfiles=(SELECT COUNT(*) AS numfiles " /* */
        " FROM " DUF_DBPREF "filenames AS fn JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd.id) " /* */
        " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid)",
 
   "INSERT OR IGNORE INTO " DUF_DBPREF "pathtot_dirs (Pathid, numdirs) " /* */
        "SELECT parents.id AS Pathid, COUNT(*) AS numdirs " /* */
        " FROM " DUF_DBPREF "paths " /* */
        " JOIN " DUF_DBPREF "paths AS parents ON (parents.id=paths.parentid) " /* */
        " GROUP BY parents.id"  /* */
        ,

  "UPDATE " DUF_DBPREF "pathtot_dirs SET " /* */
        " numdirs=(SELECT COUNT(*) AS numdirs " /* */
        " FROM " DUF_DBPREF "paths AS p " /* */
        " WHERE p.parentid=" DUF_DBPREF "pathtot_dirs.Pathid )",
  /* "DELETE FROM " DUF_DBPREF "keydata", */
  "INSERT OR REPLACE INTO " DUF_DBPREF "keydata (md5id, filenameid, dataid, Pathid) " /* */
        "SELECT md.id AS md5id, fn.id AS filenameid, fd.id AS dataid, p.id AS Pathid " /* */
        " FROM " DUF_DBPREF "filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd.id)" /* */
        " JOIN " DUF_DBPREF "paths AS p ON (fn.Pathid=p.id)" /* */
        " JOIN " DUF_DBPREF "md5 AS md ON (fd.md5id=md.id)",


  NULL,
};



duf_scan_callbacks_t duf_directories_callbacks = {
  .title = "directories",
  .init_scan = NULL,
  .opendir = 1,
  .scan_mode_2 = 1,
  .dirent_dir_scan_before = directories_entry_dir,
  .dirent_dir_scan_before2 = directories_entry_dir2,


  /* filename for debug only */
  .leaf_fieldset = "fn.Pathid AS dirid, fn.name AS filename, fd.size AS filesize " /* */
        ", uid, gid, nlink, inode, mtim AS mtime " /* */
        ", fd.mode AS filemode " /* */
        ", fn.id AS filenameid " /* */
        ", md.dup5cnt AS nsame, md.md5sum1, md.md5sum2 ",
  .leaf_selector = "SELECT %s FROM " DUF_DBPREF "filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON ( fn.dataid = fd.id ) " /* */
        " LEFT JOIN " DUF_DBPREF "md5 AS md ON ( md.id = fd.md5id ) " /* */
        " WHERE "               /* */
        /* " fd.size >= %llu AND fd.size < %llu "                      */
        /* " AND( md.dup5cnt IS NULL OR( md.dup5cnt >= %llu AND md.dup5cnt < %llu ) ) AND " */
        " fn.Pathid = '%llu' ",
  .leaf_selector2 =             /* */
        /* "SELECT %s " */
        " FROM " DUF_DBPREF "filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON ( fn.dataid = fd.id ) " /* */
        " LEFT JOIN " DUF_DBPREF "md5 AS md ON ( md.id = fd.md5id ) " /* */
        " WHERE "               /* */
        " fn.Pathid = :dirid "  /* */
        " AND (:minsize IS NULL OR fd.size>=:minsize) AND (:maxsize IS NULL OR fd.size<=:maxsize) " /* */
        " AND (:minsame IS NULL OR md.dup5cnt>=:minsame) AND (:maxsame IS NULL OR md.dup5cnt<=:maxsame) " /* */
        ,
  .leaf_selector_total2 =       /* */
        " FROM " DUF_DBPREF "filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd.id) " /* */
        " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md.id=fd.md5id)" /* */
        "    WHERE "            /* */
        "     (:minsize IS NULL OR fd.size>=:minsize) AND (:maxsize IS NULL OR fd.size<=:maxsize) " /* */
        " AND (:minsame IS NULL OR md.dup5cnt>=:minsame) AND (:maxsame IS NULL OR md.dup5cnt<=:maxsame) " /* */
        ,
  .node_fieldset = "pt.id AS dirid, pt.dirname, pt.dirname AS dfname,  pt.parentid " /* */
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
        ,
  .node_selector =              /* */
        /* "SELECT     pt.id AS dirid, pt.dirname, pt.dirname AS dfname,  pt.parentid "                  */
        /* ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " */
        /* ", ( SELECT count( * ) FROM " DUF_DBPREF "paths AS subpaths WHERE subpaths.parentid = pt.id ) AS ndirs "        */
        /* ", ( SELECT count( * ) FROM " DUF_DBPREF "filenames AS sfn "                                                           */
        /* " LEFT JOIN " DUF_DBPREF "filedatas AS sfd ON( sfn.dataid = sfd.id ) "                                                     */
        /* " JOIN " DUF_DBPREF "md5 AS smd ON( sfd.md5id = smd.id ) "                                                            */
        /* " WHERE sfn.Pathid = pt.id "                                                                         */
        /* " AND sfd.size >= %llu AND sfd.size < %llu "                                                                */
        /* " AND( smd.dup5cnt IS NULL OR( smd.dup5cnt >= %llu AND smd.dup5cnt < %llu ) ) "                                */
        /* " ) AS nfiles "                                                                                             */
        /* ", ( SELECT min( sfd.size ) FROM " DUF_DBPREF "filedatas AS sfd JOIN " DUF_DBPREF "filenames AS sfn ON( sfn.dataid = sfd.id ) " */
        /* " WHERE sfn.Pathid = pt.id ) AS minsize "                                                            */
        /* ", ( SELECT max( sfd.size ) FROM " DUF_DBPREF "filedatas AS sfd JOIN " DUF_DBPREF "filenames AS sfn ON( sfn.dataid = sfd.id ) " */
        /* " WHERE sfn.Pathid = pt.id ) AS maxsize "                                                            */
        " FROM " DUF_DBPREF "paths AS pt " /* */
        " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.Pathid=pt.id) " /* */
        " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt.id) " /* */
        " WHERE pt.parentid = '%llu' ",
  .node_selector2 =             /* */
        /* "SELECT     pt.id AS dirid, pt.dirname, pt.dirname AS dfname,  pt.parentid "                  */
        /* ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " */
        " FROM " DUF_DBPREF "paths AS pt " /* */
        " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.Pathid=pt.id) " /* */
        " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt.id) " /* */
        " WHERE pt.parentid = :dirid ",
  .final_sql_argv = final_sql,
};
