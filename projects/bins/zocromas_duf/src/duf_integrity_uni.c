#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <dirent.h>
#include <errno.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"



#include "duf_sql_field.h"
#include "duf_path.h"

#include "duf_add.h"


#include "duf_filedata.h"
#include "duf_dirent.h"

#include "duf_sql_def.h"
#include "duf_sql.h"
#include "duf_dbg.h"

 /* *INDENT-OFF*  */

static char *final_sql[] = {
  "UPDATE duf_md5 SET dupcnt=(SELECT COUNT(*) "
        " FROM duf_filedatas AS fd "
	  " JOIN duf_md5 AS md ON (fd.md5id=md.id) "
        " WHERE duf_md5.md5sum1=md.md5sum1 and duf_md5.md5sum2=md.md5sum2)",
  "DELETE FROM duf_pathtot_files",
  "INSERT OR IGNORE INTO duf_pathtot_files (pathid, numfiles, minsize, maxsize, last_updated) "
        " SELECT p.id AS pathid, COUNT(*) AS numfiles, min(size) AS minsize, max(size) AS maxsize, datetime() "
	  " FROM duf_paths AS p "
            " LEFT JOIN duf_filenames AS fn ON (fn.pathid=p.id) "
	      " JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) "
	" GROUP BY fn.pathid",
  "INSERT OR IGNORE INTO duf_pathtot_dirs (pathid, numdirs, last_updated) "
        " SELECT p.id AS pathid, COUNT(*) AS numdirs, datetime() "
	  " FROM duf_paths AS p "
            " LEFT JOIN duf_paths AS sp ON (sp.parentid=p.id) "
	" GROUP BY sp.parentid",
  "INSERT INTO duf_keydata (md5id, filenameid, dataid, pathid) "
      "SELECT md.id AS md5id, fn.id AS filenameid, fd.id AS dataid, p.id AS pathid "
	  " FROM duf_filenames AS fn "
	    " JOIN duf_filedatas AS fd ON (fn.dataid=fd.id)"
	      " JOIN duf_paths AS p ON (fn.pathid=p.id)"
	        " JOIN duf_md5 AS md ON (fd.md5id=md.id)",

 /* *INDENT-ON*  */

NULL,};


duf_scan_callbacks_t duf_integrity_callbacks = {
  .title = __FILE__,.init_scan = NULL,
  .node_scan_before = NULL,
  .leaf_scan = NULL,
  /* filename for debug only */
  .fieldset = NULL,
  .leaf_selector = NULL,
  .node_selector = NULL,
  .final_sql_argv = final_sql,
};
