#include <assert.h>
#include <dirent.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"
#include "duf_hook_types.h"

#include "duf_sql_defs.h"


static duf_sql_sequence_t final_sql = {.done = 0,
  .sql = {
          "UPDATE " DUF_DBPREF "md5 SET dup5cnt=(SELECT COUNT(*) " /* */
          " FROM " DUF_DBPREF "filedatas AS fd " /* */
          " JOIN " DUF_DBPREF "md5 AS md ON (fd.md5id=md." DUF_SQL_IDNAME ") " /* */
          " WHERE " DUF_DBPREF "md5." DUF_SQL_IDNAME "=md." DUF_SQL_IDNAME ")" /* */
          /* " WHERE " DUF_DBPREF "md5.md5sum1=md.md5sum1 AND " DUF_DBPREF "md5.md5sum2=md.md5sum2)", */
          ,

          "UPDATE " DUF_DBPREF "exif SET dupexifcnt=(SELECT COUNT(*) " /* */
          " FROM " DUF_DBPREF "filedatas AS fd " /* */
          " JOIN " DUF_DBPREF "exif AS x ON (fd.exifid=x." DUF_SQL_IDNAME ") " /* */
          " WHERE exif." DUF_SQL_IDNAME "=x." DUF_SQL_IDNAME " AND fixed IS NULL ) WHERE fixed IS NULL" /* */
          ,

          /* "UPDATE " DUF_DBPREF "exif SET dupexifcnt=(SELECT COUNT(*) " (*       *)                     */
          /*       " FROM " DUF_DBPREF "filedatas AS fd " (*       *)                                     */
          /*         " JOIN " DUF_DBPREF "exif AS x ON (fd.exifid=x." DUF_SQL_IDNAME ") " (*       *)     */
          /*       " WHERE " DUF_DBPREF "x.md5sum1=md.md5sum1 AND " DUF_DBPREF "md5.md5sum2=md.md5sum2)", */

          "DELETE FROM " DUF_DBPREF "sizes",
          "INSERT OR IGNORE INTO " DUF_DBPREF "sizes (size, dupzcnt) " /* */
          "SELECT size, COUNT(*) " /* */
          " FROM " DUF_DBPREF "filedatas AS fd GROUP BY fd.size" /* */
          ,
          "DELETE FROM path_pairs" /* */
          ,
          "INSERT OR IGNORE INTO path_pairs (samefiles, Pathid1, Pathid2) SELECT count(*), fna.Pathid AS Pathid1, fnb.Pathid  AS Pathid2" /* */
          " FROM filenames AS fna" /* */
          "   JOIN filedatas AS fda ON (fna.dataid=fda.rowid)" /* */
          "   JOIN md5 AS mda ON (fda.md5id=mda.rowid)" /* */
          "   JOIN filedatas AS fdb ON (fdb.md5id=mda.rowid)" /* */
          "   JOIN filenames AS fnb ON (fdb.rowid=fnb.dataid)" /* */
          " WHERE Pathid1 < Pathid2 AND fna.name=fnb.name" /* */
          " GROUP BY Pathid1, Pathid2" /* */
          ,
#if 0
          "DELETE FROM pathtot_files",
          "INSERT OR IGNORE INTO " DUF_DBPREF "pathtot_files (Pathid, numfiles, minsize, maxsize) " /* */
          "SELECT fn.Pathid AS Pathid, COUNT(*) AS numfiles, min(size) AS minsize, max(size) AS maxsize " /* */
          " FROM " DUF_DBPREF "filenames AS fn " /* */
          " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
          " GROUP BY fn.Pathid" /* */
          ,

          "DELETE FROM pathtot_dirs",
          "INSERT OR IGNORE INTO " DUF_DBPREF "pathtot_dirs (Pathid, numdirs) " /* */
          "SELECT parents." DUF_SQL_IDNAME " AS Pathid, COUNT(*) AS numdirs " /* */
          " FROM " DUF_DBPREF "paths " /* */
          " JOIN " DUF_DBPREF "paths AS parents ON (parents." DUF_SQL_IDNAME "=paths.ParentId) " /* */
          " GROUP BY parents." DUF_SQL_IDNAME "" /* */
          ,
#endif
          NULL,
          }
};


duf_scan_callbacks_t duf_integrity_callbacks = {
  .title = __FILE__,
  .name = "integrity",
  .init_scan = NULL,

  .use_std_leaf = 0, /* 1 : preliminary selection; 2 : direct (beginning_sql_argv=NULL recommended in many cases) */
  .use_std_node = 0, /* 1 : preliminary selection; 2 : direct (beginning_sql_argv=NULL recommended in many cases) */
  /* filename for debug only */
  .leaf = {.fieldset = NULL,
           .selector = NULL}
  ,
  .node = {.selector = NULL}
  ,
  .final_sql_argv = &final_sql,
};
