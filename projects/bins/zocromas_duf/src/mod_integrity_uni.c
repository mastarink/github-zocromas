#include <assert.h>
#include <dirent.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"
#include "duf_hook_types.h"


 /* *INDENT-OFF*  */

static const char *final_sql[] = {
  "UPDATE " DUF_DBPREF "md5 SET dup5cnt=(SELECT COUNT(*) " /*	*/
        " FROM " DUF_DBPREF "filedatas AS fd " /*	*/
	  " JOIN " DUF_DBPREF "md5 AS md ON (fd.md5id=md.id) " /*	*/
        " WHERE " DUF_DBPREF "md5.md5sum1=md.md5sum1 AND " DUF_DBPREF "md5.md5sum2=md.md5sum2)",
  "DELETE FROM " DUF_DBPREF "sizes",
  "INSERT OR IGNORE INTO " DUF_DBPREF "sizes (size, dupzcnt) " /*	*/
        "SELECT size, COUNT(*) " /*	*/
          " FROM " DUF_DBPREF "filedatas AS fd GROUP BY fd.size",

  "DELETE FROM pathtot_files",
  "INSERT OR IGNORE INTO " DUF_DBPREF "pathtot_files (Pathid, numfiles, minsize, maxsize) " /*	*/
        "SELECT fn.Pathid AS Pathid, COUNT(*) AS numfiles, min(size) AS minsize, max(size) AS maxsize " /*	*/
	  " FROM " DUF_DBPREF "filenames AS fn " /*	*/
	      " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd.id) " /*	*/
	" GROUP BY fn.Pathid",
  /* "UPDATE " DUF_DBPREF "pathtot_files SET " (*  *)                                                                 */
  /*     " minsize=(SELECT min(size) AS minsize " (*       *)                                                         */
  /*         " FROM " DUF_DBPREF "filenames AS fn JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd.id) " (*     *) */
  /*            " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid) " (* *)                                         */
  /*    ", maxsize=(SELECT max(size) AS maxsize " (*       *)                                                         */
  /*         " FROM " DUF_DBPREF "filenames AS fn JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd.id) " (*     *) */
  /*            " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid) " (* *)                                         */
  /*    ", numfiles=(SELECT COUNT(*) AS numfiles " (*      *)                                                         */
  /*         " FROM " DUF_DBPREF "filenames AS fn JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd.id) " (*     *) */
  /*            " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid)",                                               */
 
  "DELETE FROM pathtot_dirs",
  "INSERT OR IGNORE INTO " DUF_DBPREF "pathtot_dirs (Pathid, numdirs) " /* */
        "SELECT parents.id AS Pathid, COUNT(*) AS numdirs " /* */
        " FROM " DUF_DBPREF "paths " /* */
        " JOIN " DUF_DBPREF "paths AS parents ON (parents.id=paths.parentid) " /* */
        " GROUP BY parents.id"  /* */
        ,

  /* "UPDATE " DUF_DBPREF "pathtot_dirs SET " (*   *)                             */
  /*     " numdirs=(SELECT COUNT(*) AS numdirs " (*        *)                     */
  /*                 " FROM " DUF_DBPREF "paths AS p " (*  *)                     */
  /*                     " WHERE p.parentid=" DUF_DBPREF "pathtot_dirs.Pathid )", */

  /* "DELETE FROM " DUF_DBPREF "keydata", */
  /* "INSERT OR REPLACE INTO " DUF_DBPREF "keydata (md5id, filenameid, dataid, Pathid) " (*        *) */
  /*     "SELECT md.id AS md5id, fn.id AS filenameid, fd.id AS dataid, p.id AS Pathid " (* *)         */
  /*         " FROM " DUF_DBPREF "filenames AS fn " (*     *)                                         */
  /*           " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd.id)" (*  *)                 */
  /*             " JOIN " DUF_DBPREF "paths AS p ON (fn.Pathid=p.id)" (*   *)                         */
  /*               " JOIN " DUF_DBPREF "md5 AS md ON (fd.md5id=md.id)",                               */

 /* *INDENT-ON*  */

NULL,};


duf_scan_callbacks_t duf_integrity_callbacks = {
  .title = __FILE__,
  .init_scan = NULL,
  .scan_mode_2 = 1,
  .node_scan_before = NULL,
  .leaf_scan = NULL,
  /* filename for debug only */
  .leaf_fieldset = NULL,
  .leaf_selector = NULL,
  .node_selector = NULL,
  .final_sql_argv = final_sql,
};
