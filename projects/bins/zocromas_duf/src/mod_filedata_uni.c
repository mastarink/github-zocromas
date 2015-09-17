#include <fcntl.h>              /* Definition of AT_* constants */

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"

#include "duf_levinfo_ref.h"
#include "duf_sql_defs.h"
#include "duf_filedata.h"

/* #include "duf_dbg.h" */

#include "sql_beginning_tables.h"

/* ########################################################################################## */
static int register_pdifiledata( duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi );

/* ########################################################################################## */


static duf_sql_sequence_t final_sql = /* */
{
  .name = "final @ ...",
  .done = 0,
  .sql = {
#if 0
          "UPDATE " DUF_SQL_TABLES_MD5_FULL " SET dup5cnt=(SELECT COUNT(*) " /* */
          " FROM " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd " /* */
          " JOIN " DUF_SQL_TABLES_MD5_FULL " AS md ON (fd.md5id=md." DUF_SQL_IDNAME ") " /* */
          " WHERE " DUF_SQL_TABLES_MD5_FULL "." DUF_SQL_IDNAME "=md." DUF_SQL_IDNAME ")" /* */
          /* " WHERE " DUF_SQL_TABLES_MD5_FULL ".md5sum1=md.md5sum1 AND " DUF_SQL_TABLES_MD5_FULL ".md5sum2=md.md5sum2)" */
          ,
          "UPDATE " DUF_SQL_TABLES_EXIF_FULL " SET dupexifcnt=(SELECT COUNT(*) " /* */
          " FROM " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd " /* */
          " JOIN " DUF_SQL_TABLES_EXIF_FULL " AS x ON (fd.exifid=x." DUF_SQL_IDNAME ") " /* */
          " WHERE " DUF_SQL_TABLES_EXIF_FULL "." DUF_SQL_IDNAME "=x." DUF_SQL_IDNAME " AND fixed IS NULL ) WHERE fixed IS NULL" /* */
          ,
#endif
          "DELETE FROM " DUF_SQL_TABLES_SIZES_FULL "",
          "INSERT OR IGNORE INTO " DUF_SQL_TABLES_SIZES_FULL " (size, dupzcnt) " /* */
          "SELECT size, COUNT(*) " /* */
          " FROM " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd GROUP BY fd.size" /* */
          ,


#if 0
          /* "DELETE FROM " DUF_DBPREF "pathtot_files", */
          "INSERT OR IGNORE INTO " DUF_DBPREF "pathtot_files (Pathid, numfiles, minsize, maxsize) " /* */
          "SELECT fn.Pathid AS Pathid, COUNT(*) AS numfiles, min(size) AS minsize, max(size) AS maxsize " /* */
          " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn " /* */
          " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
          " GROUP BY fn.Pathid" /* */
          ,
          "UPDATE " DUF_DBPREF "pathtot_files SET " /* */
          " minsize=(SELECT min(size) AS minsize " /* */
          " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
          " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid) " /* */
          ", maxsize=(SELECT max(size) AS maxsize " /* */
          " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
          " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid) " /* */
          ", numfiles=(SELECT COUNT(*) AS numfiles " /* */
          " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
          " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid)" /* */
          ,

          "INSERT OR IGNORE INTO " DUF_DBPREF "pathtot_dirs (Pathid, numdirs) " /* */
          "SELECT parents." DUF_SQL_IDNAME " AS Pathid, COUNT(*) AS numdirs " /* */
          " FROM " DUF_SQL_TABLES_PATHS_FULL " " /* */
          " JOIN " DUF_SQL_TABLES_PATHS_FULL " AS parents ON (parents." DUF_SQL_IDNAME "=" DUF_SQL_TABLES_PATHS_FULL ".ParentId) " /* */
          " GROUP BY parents." DUF_SQL_IDNAME "" /* */
          ,
          /* "UPDATE " DUF_DBPREF "pathtot_dirs SET " (* *)                      */
          /*       " numdirs=(SELECT COUNT(*) AS numdirs " (* *)                 */
          /*       " FROM " DUF_SQL_TABLES_PATHS_FULL " AS p " (* *)                       */
          /*       " WHERE p.parentid=" DUF_DBPREF "pathtot_dirs.Pathid )" (* *) */
          /*       ,                                                             */
#endif

          /* "DELETE FROM " DUF_DBPREF "keydata", */
          /* "INSERT OR REPLACE INTO " DUF_DBPREF "keydata (md5id, filenameid, dataid, Pathid) " (* *)  */
          /*       "SELECT md." DUF_SQL_IDNAME " AS md5id, fn." DUF_SQL_IDNAME " AS filenameid, fd." DUF_SQL_IDNAME " AS dataid, p." DUF_SQL_IDNAME " AS Pathid " (* *) */
          /*       " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn " (* *)                                         */
          /*       " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ")" (* *)                */
          /*       " JOIN " DUF_SQL_TABLES_PATHS_FULL " AS p ON (fn.Pathid=p." DUF_SQL_IDNAME ")" (* *)                           */
          /*       " JOIN " DUF_SQL_TABLES_MD5_FULL " AS md ON (fd.md5id=md." DUF_SQL_IDNAME ")",                                 */

          /* *INDENT-ON*  */

          NULL,
          }
};

/* ########################################################################################## */

duf_scan_callbacks_t duf_filedata_callbacks = {
  .title = "file data",
  .name = "filedata",
  .init_scan = NULL,
  .def_opendir = 1,


  .dirent_file_scan_before2 = register_pdifiledata,




/* TODO : exp;ain values of use_std_leaf and use_std_node TODO */
  .use_std_leaf = 1,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node = 0,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .count_nodes = 1,
  .leaf = {                     /* */
           .name = "fd leaf",
           .type = DUF_NODE_LEAF,
           .fieldset =          /* Never used!? */
#if 0
           "'filedata-leaf' AS fieldset_id, " /* */
           "  fn.Pathid AS dirid " /* */
           ", 0 AS ndirs, 0 AS nfiles" /* */
           ", fn.name AS filename, fn.name AS dfname, fd.size AS filesize " /* */
           ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
           ", STRFTIME( '%s', fd.mtim ) AS mtime " /* */
           ", fd.mode AS filemode " /* */
           ", fn." DUF_SQL_IDNAME " AS filenameid " /* */
           ", fn." DUF_SQL_IDNAME " AS nameid " /* */
           ", fd." DUF_SQL_IDNAME " AS filedataid " /* */
           ", fd." DUF_SQL_IDNAME " AS dataid " /* */
           ", md.dup5cnt AS nsame " /* */
           ", fd.md5id AS md5id" /* */
           /* ", md." DUF_SQL_IDNAME " AS md5id " (* *) */
           ", md.md5sum1, md.md5sum2 " /* */
           ", fd.exifid AS exifid, fd.mimeid AS mimeid " /* */
           ", fd.size AS filesize " /* */
           ,
           .selector2 =         /* */
           /* "SELECT %s " */
           " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn " /* */
           " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON ( fn.dataid = fd." DUF_SQL_IDNAME " ) " /* */
           " LEFT JOIN " DUF_SQL_TABLES_MD5_FULL " AS md ON ( md." DUF_SQL_IDNAME " = fd.md5id ) " /* */
           ,
           .matcher = " fn.Pathid = :parentdirID " /* */
           ,
           .filter = NULL       /* */
#else
           NULL
#endif
           },
  .node = {                     /* */
           .name = "fd node",
           .type = DUF_NODE_NODE,
           .expand_sql = 1,     /* */
           .fieldset =          /* */
           "'filedata-node' AS fieldset_id, " /* */
           " pt." DUF_SQL_IDNAME " AS dirid" /* */
           ", pt." DUF_SQL_IDNAME " AS nameid " /* */
           ", pt.dirname, pt.dirname AS dfname,  pt.parentid " /* */
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
           ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks, STRFTIME( '%s', pt.mtim ) AS mtime " /* */
           ,
           .selector2 =         /* */
           " FROM " DUF_SQL_TABLES_PATHS_FULL " AS pt " /* */
           " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL "  AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
#if 0
           " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
#endif
           ,
           .matcher = "pt.parentid = :parentdirID  AND ( :dirName IS NULL OR dirname=:dirName ) " /* */
           ,
           .filter = NULL       /* */
           },
  .final_sql_seq = &final_sql
};

/* ########################################################################################## */

static int
register_pdifiledata( duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi )
{
  return duf_pdistat2file( pdi );
}
