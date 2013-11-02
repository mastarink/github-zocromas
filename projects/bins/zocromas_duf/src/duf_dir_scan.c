#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

/* #include "duf_sql.h" */
/* #include "duf_utils.h" */
#include "duf_path.h"
/* #include "duf_file.h" */
/* #include "duf_file_pathid.h" */
#include "duf_file_scan.h"


/* ###################################################################### */
#include "duf_dir_scan.h"
/* ###################################################################### */


static int
duf_scan_dirs_by_parentid( unsigned long long pathid, duf_str_cb_t str_cb, duf_dirinfo_t * pdi )
{
  int r = 0;

  r = duf_scan_files_sql( str_cb, pdi,
                          "SELECT duf_paths.id, duf_paths.dirname, items " " FROM duf_paths " " WHERE duf_paths.parentid='%llu'", pathid );
  return r;
}

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_str_cb_t: 
 *              int fun( unsigned long long pathid, const char *path, unsigned long long filenameid, const char *name, void *str_cb_udata,
 *                      const char *const *presult )
 * */
int
duf_sql_scan_print_dir( unsigned long long pathid, const char *path, unsigned long long filenameid, const char *name, void *str_cb_udata,
                        const char *const *presult )
{
  duf_dirinfo_t *pdi;
  duf_dirinfo_t di;

  pdi = ( duf_dirinfo_t * ) str_cb_udata;
  /* printf( "%7llu: %s/%s\n", pathid, path, name ); */
  if ( pdi )
  {
    char format[256];

    if ( 1 )
    {
      snprintf( format, sizeof( format ), "%%4llu: (%%2s) %%-%ds %%s\n", pdi->level * 2 + 1 );
      printf( format, pathid, presult[2], pdi->level?"":"-", presult[1] );
    }
    else
    {
      snprintf( format, sizeof( format ), "%%4llu: %%%ds %%-20s ##%%d##   %%s\n", pdi->level * 3 + 5 );
      printf( format, pathid, "", path, pdi->level, presult[1] );
    }
    if ( pdi->recursive )
    {
      di = *pdi;
      di.level++;
      duf_print_dirs_by_parentid( pathid, &di );
    }
  }
  return 0;
}

int
duf_print_dirs_by_parentid( unsigned long long parentid, duf_dirinfo_t * pdi )
{
  int r = 0;

  r = duf_scan_dirs_by_parentid( parentid, duf_sql_scan_print_dir, pdi );
  return r;
}

int
duf_print_dirs( const char *path, int recursive )
{
  duf_dirinfo_t di = {.level = 0,.recursive = recursive };

  return duf_print_dirs_by_parentid( duf_path_to_pathid( path ), &di );
}
