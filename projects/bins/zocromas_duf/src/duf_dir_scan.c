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
#include "duf_file_scan.h"


#include "duf_dir_scan.h"


static int
duf_scan_dirs_by_parentid( unsigned long long pathid, duf_str_cb_t str_cb, int recursive )
{
  int r = 0;

  r = duf_scan_files_sql( str_cb, &recursive,
                          "SELECT duf_paths.id, duf_paths.dirname " " FROM duf_paths " " WHERE duf_paths.parentid='%llu'", pathid );
  return r;
}

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_str_cb_t: 
 *              int fun( unsigned long long pathid, const char *path, unsigned long long filenameid, const char *name, void *str_cb_udata )
 * */
int
duf_sql_scan_print_dir( unsigned long long pathid, const char *path, unsigned long long filenameid, const char *name, void *str_cb_udata )
{
  int recursive = 0;

  recursive = ( str_cb_udata ? *( ( int * ) str_cb_udata ) : 0 );
  /* printf( "%7llu: %s/%s\n", pathid, path, name ); */
  printf( "%7llu: %-20s -- %d\n", pathid, path, recursive );
  if ( recursive )
  {
    duf_print_dirs_by_parentid( pathid, recursive );
  }
  return 0;
}

int
duf_print_dirs_by_parentid( unsigned long long parentid, int recursive )
{
  int r = 0;

  r = duf_scan_dirs_by_parentid( parentid, duf_sql_scan_print_dir, recursive );
  /* if ( recursive )                                           */
  /* {                                                          */
  /*   (* duf_print_dirs( const char *path, int recursive ); *) */
  /*                                                            */
  /*   fprintf( stderr, ">REC ==================\n" );          */
  /* }                                                          */
  return r;
}

int
duf_print_dirs( const char *path, int recursive )
{
  return duf_print_dirs_by_parentid( duf_path_to_pathid( path ), recursive );
}
