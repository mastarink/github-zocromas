#define DUF_SQL_PDI_STMT

#include <fcntl.h>              /* Definition of AT_* constants */





#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"


#include "duf_config_ref.h"


#include "duf_levinfo_ref.h"


#include "duf_sql_defs.h"
/* #include "duf_sql2.h" */




#include "duf_filedata.h"

/* #include "duf_dbg.h" */

/* #include "sql_beginning_selected.h" */
#include "sql_beginning_tables.h"


/* ########################################################################################## */
static int register_pdifiledata( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi );

/* ########################################################################################## */


static duf_sql_sequence_t final_sql = {.done = 0,
  .sql = {
#if 0
          "UPDATE " DUF_DBPREF "md5 SET dup5cnt=(SELECT COUNT(*) " /* */
          " FROM " DUF_DBPREF "filedatas AS fd " /* */
          " JOIN " DUF_DBPREF "md5 AS md ON (fd.md5id=md." DUF_SQL_IDNAME ") " /* */
          " WHERE " DUF_DBPREF "md5." DUF_SQL_IDNAME "=md." DUF_SQL_IDNAME ")" /* */
          /* " WHERE " DUF_DBPREF "md5.md5sum1=md.md5sum1 AND " DUF_DBPREF "md5.md5sum2=md.md5sum2)" */
          ,
          "UPDATE " DUF_DBPREF "exif SET dupexifcnt=(SELECT COUNT(*) " /* */
          " FROM " DUF_DBPREF "filedatas AS fd " /* */
          " JOIN " DUF_DBPREF "exif AS x ON (fd.exifid=x." DUF_SQL_IDNAME ") " /* */
          " WHERE exif." DUF_SQL_IDNAME "=x." DUF_SQL_IDNAME " AND fixed IS NULL ) WHERE fixed IS NULL" /* */
          ,
#endif
          "DELETE FROM " DUF_DBPREF "sizes",
          "INSERT OR IGNORE INTO " DUF_DBPREF "sizes (size, dupzcnt) " /* */
          "SELECT size, COUNT(*) " /* */
          " FROM " DUF_DBPREF "filedatas AS fd GROUP BY fd.size" /* */
          ,


#if 0
          /* "DELETE FROM " DUF_DBPREF "pathtot_files", */
          "INSERT OR IGNORE INTO " DUF_DBPREF "pathtot_files (Pathid, numfiles, minsize, maxsize) " /* */
          "SELECT fn.Pathid AS Pathid, COUNT(*) AS numfiles, min(size) AS minsize, max(size) AS maxsize " /* */
          " FROM " DUF_DBPREF "filenames AS fn " /* */
          " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
          " GROUP BY fn.Pathid" /* */
          ,
          "UPDATE " DUF_DBPREF "pathtot_files SET " /* */
          " minsize=(SELECT min(size) AS minsize " /* */
          " FROM " DUF_DBPREF "filenames AS fn JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
          " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid) " /* */
          ", maxsize=(SELECT max(size) AS maxsize " /* */
          " FROM " DUF_DBPREF "filenames AS fn JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
          " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid) " /* */
          ", numfiles=(SELECT COUNT(*) AS numfiles " /* */
          " FROM " DUF_DBPREF "filenames AS fn JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
          " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid)" /* */
          ,

          "INSERT OR IGNORE INTO " DUF_DBPREF "pathtot_dirs (Pathid, numdirs) " /* */
          "SELECT parents." DUF_SQL_IDNAME " AS Pathid, COUNT(*) AS numdirs " /* */
          " FROM " DUF_DBPREF "paths " /* */
          " JOIN " DUF_DBPREF "paths AS parents ON (parents." DUF_SQL_IDNAME "=paths.ParentId) " /* */
          " GROUP BY parents." DUF_SQL_IDNAME "" /* */
          ,
          /* "UPDATE " DUF_DBPREF "pathtot_dirs SET " (* *)                      */
          /*       " numdirs=(SELECT COUNT(*) AS numdirs " (* *)                 */
          /*       " FROM " DUF_DBPREF "paths AS p " (* *)                       */
          /*       " WHERE p.parentid=" DUF_DBPREF "pathtot_dirs.Pathid )" (* *) */
          /*       ,                                                             */
#endif

          /* "DELETE FROM " DUF_DBPREF "keydata", */
          /* "INSERT OR REPLACE INTO " DUF_DBPREF "keydata (md5id, filenameid, dataid, Pathid) " (* *)  */
          /*       "SELECT md." DUF_SQL_IDNAME " AS md5id, fn." DUF_SQL_IDNAME " AS filenameid, fd." DUF_SQL_IDNAME " AS dataid, p." DUF_SQL_IDNAME " AS Pathid " (* *) */
          /*       " FROM " DUF_DBPREF "filenames AS fn " (* *)                                         */
          /*       " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ")" (* *)                */
          /*       " JOIN " DUF_DBPREF "paths AS p ON (fn.Pathid=p." DUF_SQL_IDNAME ")" (* *)                           */
          /*       " JOIN " DUF_DBPREF "md5 AS md ON (fd.md5id=md." DUF_SQL_IDNAME ")",                                 */

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


  .use_std_leaf = 0,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node = 0,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .count_nodes = 1,
  .leaf = {.fieldset = "fn.Pathid AS dirid, fn.name AS filename, fn.name AS dfname, fd.size AS filesize" /* */
           ", 0 as ndirs, 0 as nfiles" /* */
           ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks, strftime('%s', fd.mtim) AS mtime " /* */
           ", dup5cnt AS nsame" /* */
           ", fn." DUF_SQL_IDNAME " AS filenameid, fd.mode AS filemode, md.md5sum1, md.md5sum2 " /* */
           ", fd.md5id AS md5id" /* */
           ,
           .selector2 =         /* */
           /* "SELECT %s " */
           " FROM " DUF_DBPREF "filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON ( fn.dataid = fd." DUF_SQL_IDNAME " ) " /* */
           " LEFT JOIN " DUF_DBPREF "md5 AS md ON ( md." DUF_SQL_IDNAME " = fd.md5id ) " /* */
           " WHERE "            /* */
           " fn.Pathid = :parentdirID " /* */
           ,
           .selector_total2 =   /* */
#if 0
           " FROM " DUF_DBPREF "paths AS p " /* */
#else
           " FROM " DUF_SQL_TABLES_PATHS_FULL " AS p " /* */
#endif
           }
  ,
  .node = {.fieldset = "pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.parentid " /* */
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
           ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks, STRFTIME( '%s', pt.mtim ) AS mtime " /* */
           ,
           .selector2 =         /* */
           /* "SELECT     pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.parentid "                  */
           /* ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " */
           " FROM " DUF_DBPREF "paths AS pt " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PATHTOT_DIRS_FULL "  AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
#if 0
           " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
#endif
           " WHERE pt.ParentId = :parentdirID  AND ( :dirName IS NULL OR dirname=:dirName ) " /* */
           ,
           .selector_total2 =   /* */
           " /* fd */ FROM " DUF_SQL_TABLES_PATHS_FULL " AS p " /* */
           }
  ,
  .final_sql_seq = &final_sql   /* */
};

/* ########################################################################################## */
static int
register_pdifiledata( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi )
{
  return duf_pdistat2file( pdi );
}
