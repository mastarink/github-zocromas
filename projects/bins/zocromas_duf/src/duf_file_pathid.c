#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_types.h"

#include "duf_sql.h"

#include "duf_utils.h"

#include "duf_path.h"
#include "duf_file.h"

#include "duf_file_pathid.h"



static int
duf_scan_files_by_pathid( unsigned long long pathid, duf_str_callback_t fuscan )
{
  int r = 0;

/* 
 * sql must select pathid, filenameid, filename
 * */
  r = duf_scan_files_sql( fuscan, "SELECT duf_filenames.pathid, duf_filenames.id, duf_filenames.name, duf_md5.dupcnt " " FROM duf_filenames "
                      " LEFT JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) "
                      " LEFT JOIN duf_md5 on (duf_md5.id=duf_filedatas.md5id) " " WHERE duf_filenames.pathid='%llu'", pathid );
  return r;
}

int
duf_print_files_by_pathid( unsigned long long pathid )
{
  int r = 0;

  duf_scan_files_by_pathid( pathid, duf_sql_scan_print_file );
  return r;
}
