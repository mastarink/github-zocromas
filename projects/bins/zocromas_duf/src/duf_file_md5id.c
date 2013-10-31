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

#include "duf_file_md5id.h"




/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 *               int fun( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t fuscan )
 * */
static int
duf_sql_filenameid_md5id( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t fuscan )
{
  unsigned long long *pmd5id;

  pmd5id = ( unsigned long long * ) sel_cb_udata;
  if ( nrow == 0 )
    *pmd5id = strtoll( presult[0], NULL, 10 );
  return 0;
}

unsigned long long
duf_filenameid_md5id( unsigned long long filenameid )
{
  int r;
  unsigned long long md5id;

  r = duf_sql_select( duf_sql_filenameid_md5id, &md5id, NULL, 0, "SELECT md5id FROM duf_filenames "
                      " LEFT JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) " " WHERE duf_filenames.id='%llu'", filenameid );
  return r >= 0 ? md5id : r;
}

unsigned long long
duf_filepath_md5id( const char *path )
{
  unsigned long long prev_pathid = 0;
  char *notfound = NULL;
  unsigned long long pathid = 0;
  unsigned long long md5id = 0;
  unsigned long long filenameid = 0;

  pathid = duf_path_to_pathid( path, &prev_pathid, &notfound );
  if ( !pathid && prev_pathid && notfound )
  {
    filenameid = file_at_pathid_to_filenameid( prev_pathid, notfound, NULL );
    md5id = duf_filenameid_md5id( filenameid );
  }
  mas_free( notfound );
  return md5id;
}

/* 
 * sql must select pathid, filenameid, filename ... (, md5id)
 * */
static int
duf_scan_files_by_md5id( unsigned long long md5id, duf_str_cb_t fuscan )
{
  int r = 0;

/* 
 * sql must select pathid, filenameid, filename
 * */
  r = duf_scan_files_sql( fuscan, "SELECT duf_filenames.pathid, duf_filenames.id, name FROM duf_filenames "
                          " LEFT JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) "
                          " LEFT JOIN duf_md5 on (duf_md5.id=duf_filedatas.md5id) " " WHERE duf_md5.id='%llu'", md5id );
  return r;
}

int
duf_print_files_by_md5id( unsigned long long md5id )
{
  int r = 0;

  duf_scan_files_by_md5id( md5id, duf_sql_scan_print_file );
  return r;
}

int
duf_print_files_same( const char *path )
{
  unsigned long long md5id;

  md5id = duf_filepath_md5id( path );
  if ( md5id )
  {
    duf_print_files_by_md5id( md5id );
  }
  return 0;
}
