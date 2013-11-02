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

#include "duf_file_scan.h"

#include "duf_file_pathid.h"



/* 
 * sql must select pathid, filenameid, filename, md5id, size
 * */
static int
duf_scan_files_by_pathid( unsigned long long pathid, duf_str_cb_t str_cb )
{
  int r = 0;

  r = duf_scan_files_sql( str_cb, STR_CB_UDATA_DEF,
                          "SELECT duf_filenames.pathid, duf_filenames.id, duf_filenames.name, duf_md5.id, duf_md5.size, duf_md5.dupcnt "
                          " FROM duf_filenames " " LEFT JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) "
                          " LEFT JOIN duf_md5 on (duf_md5.id=duf_filedatas.md5id) " " WHERE duf_filenames.pathid='%llu'", pathid );
  return r;
}

int
duf_print_files_by_pathid( unsigned long long pathid, int recursive )
{
  int r = 0;

  r = duf_scan_files_by_pathid( pathid, duf_sql_scan_print_file );
  if ( recursive )
  {
    /* duf_print_files( const char *path, int recursive ); */

    /* fprintf( stderr, ">REC ==================\n" ); */
  }
  return r;
}
