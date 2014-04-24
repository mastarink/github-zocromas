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


static char *final_sql[] = {
  "UPDATE duf_md5 SET dupcnt=(SELECT COUNT(*) " " FROM duf_filedatas AS fd " " JOIN duf_md5 AS md ON (fd.md5id=md.id) "
        "              WHERE duf_md5.md5sum1=md.md5sum1 and duf_md5.md5sum2=md.md5sum2)",
  "DELETE FROM duf_pathtot_files",
  "INSERT OR IGNORE INTO duf_pathtot_files (pathid, numfiles, minsize, maxsize, last_updated) "
        " SELECT p.id AS pathid, COUNT(*) AS numfiles, min(size) AS minsize, max(size) AS maxsize, datetime() " " FROM duf_paths AS p "
        " LEFT JOIN duf_filenames AS fn ON (fn.pathid=p.id) JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) " " GROUP BY fn.pathid",
  "INSERT OR IGNORE INTO duf_pathtot_dirs (pathid, numdirs, last_updated) "
        " SELECT p.id AS pathid, COUNT(*) AS numdirs, datetime() " " FROM duf_paths AS p "
        " LEFT JOIN duf_paths AS sp ON (sp.parentid=p.id) " " GROUP BY sp.parentid",

  /* "UPDATE duf_paths SET "                                                                                                   */
  /*       "    numdirs=(SELECT COUNT(*) FROM duf_paths AS sp WHERE sp.parentid=duf_paths.id)                            "     */
  /*       "  , numfiles = ( SELECT COUNT( * ) FROM duf_filenames AS sfn JOIN duf_filedatas AS sfd ON( sfn.dataid = sfd.id ) " */
  /*       "                 JOIN duf_md5 AS smd ON( sfd.md5id = smd.id ) "                                                    */
  /*       "                 WHERE sfn.pathid = duf_paths.id ) "                                                               */
  /*       "  , minsize = ( SELECT min( sfd.size ) FROM duf_filedatas AS sfd "                                                 */
  /*       "                 JOIN duf_filenames AS sfn ON( sfn.dataid = sfd.id ) WHERE sfn.pathid = duf_paths.id ) "           */
  /*       "  , maxsize = ( SELECT max( sfd.size ) FROM duf_filedatas AS sfd "                                                 */
  /*       "                 JOIN duf_filenames AS sfn ON( sfn.dataid = sfd.id ) WHERE sfn.pathid = duf_paths.id ) ",          */

  /* "INSERT OR IGNORE INTO duf_pathtot (pathid) SELECT id AS pathid FROM duf_paths ", */
  /* "UPDATE duf_pathtot (numdirs) SELECT COUNT(*) AS numdirs FROM duf_paths AS sp WHERE sp.parentid=duf_pathtotpathid", */

  NULL,
};


duf_scan_callbacks_t duf_integrity_callbacks = {
  .title = __FILE__,.init_scan = NULL,
  .directory_scan_before = NULL,
  .file_scan = NULL,
  /* filename for debug only */
  .fieldset = NULL,
  .file_selector = NULL,
  .dir_selector = NULL,
  .final_sql_argv = final_sql,
};
