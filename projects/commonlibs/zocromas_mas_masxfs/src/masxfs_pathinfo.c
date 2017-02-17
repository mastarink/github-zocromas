#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "masxfs_defs.h"
#include "masxfs_error.h"

#include "masxfs_structs.h"

#include "masxfs_levinfo_io.h"
#include "masxfs_levinfo_path.h"
#include "masxfs_levinfo.h"

#include "masxfs_pathinfo_base.h"
#include "masxfs_pathinfo.h"

char *
masxfs_pathinfo_pi2path( masxfs_pathinfo_t * pi )
{
  return masxfs_levinfo_lia2path( pi->levinfo, pi->pidepth );
}

masxfs_dir_t *
masxfs_pathinfo_opendir( masxfs_pathinfo_t * pi )
{
  masxfs_dir_t *dir = NULL;

  dir = masxfs_levinfo_opendir( masxfs_pathinfo_last_li( pi ) );
/* dir = masxfs_levinfo_openpath_free( masxfs_pathinfo_last_li( pi ), masxfs_pathinfo_pi2path( pi ) ); */

  WARN( "HAHA %p / %p", dir, masxfs_pathinfo_last_li( pi )->dir );
  return dir;
}

void
masxfs_pathinfo_closedir( masxfs_pathinfo_t * pi )
{
  masxfs_levinfo_closedir( masxfs_pathinfo_last_li( pi ) );
}

char *
masxfs_pathinfo_normal_path( masxfs_pathinfo_t * pi, const char *name )
{
  char *path = masxfs_pathinfo_pi2path( pi );

  if ( name )
  {
    path = mas_strcat_x( path, "/" );
    path = mas_strcat_x( path, name );
  }
  return path;
}

int
masxfs_pathinfo_scan( masxfs_pathinfo_t * pi, masxfs_entry_callback_t * callbacks, int recursive )
{
  int r = 0;
  masxfs_levinfo_t *li = masxfs_pathinfo_last_li( pi );

  masxfs_pathinfo_opendir( pi );
  WARN( "@@@@ %p", pi );
  r = masxfs_levinfo_scandir( li, callbacks, recursive );
  if ( r )
    RDIE( "R:%d", r );
  masxfs_pathinfo_closedir( pi );

  return r;
}

masxfs_levinfo_t *
masxfs_pathinfo_tail( masxfs_pathinfo_t * pi, size_t offset )
{
  return pi->levinfo + pi->pidepth - 1 - offset;
}

masxfs_levinfo_t *
masxfs_pathinfo_last_li( masxfs_pathinfo_t * pi )
{
  return masxfs_pathinfo_tail( pi, 0 );
}
