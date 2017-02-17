#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "masxfs_defs.h"
#include "masxfs_error.h"

#include "masxfs_structs.h"

#include "masxfs_levinfo_base.h"
#include "masxfs_levinfo.h"
#include "masxfs_levinfo_path.h"

#include "masxfs_levinfo_io.h"

masxfs_dir_t *
masxfs_levinfo_openpath( masxfs_levinfo_t * li, const char *real_path )
{
  li->dir = opendir( real_path );
  return li->dir;
}

masxfs_dir_t *
masxfs_levinfo_openpath_free( masxfs_levinfo_t * li, char *real_path )
{
  masxfs_dir_t *dir = masxfs_levinfo_openpath( li, real_path );

  if ( !li->name )
    WARN( "No li->name; path:%s", real_path );

  mas_free( real_path );
  return dir;
}

masxfs_dir_t *
masxfs_levinfo_opendir_up( masxfs_levinfo_t * li )
{
  return masxfs_levinfo_openpath_free( li, masxfs_levinfo_li2path_up( li ) );
}

masxfs_dir_t *
masxfs_levinfo_opendir_abs( masxfs_levinfo_t * li )
{
  return masxfs_levinfo_openpath_free( li, masxfs_levinfo_li2path( li ) );
}

int
masxfs_levinfo_opendirfd( masxfs_levinfo_t * li )
{
  if ( !li->dirfd )
  {
    if ( li->lidepth > 0 )
    {
      li->dirfd = openat( masxfs_levinfo_opendirfd( li - 1 ), li->name, O_DIRECTORY | O_NOFOLLOW | O_RDONLY );
    }
    else if ( li->name && !*li->name )
    {
      li->dirfd = open( "/", O_DIRECTORY | O_NOFOLLOW | O_RDONLY );
    }
  }
  return li->dirfd;
}

int
masxfs_levinfo_closedirfd( masxfs_levinfo_t * li )
{
  int r = close( li->dirfd );

  li->dirfd = 0;
  return r;
}

int
masxfs_levinfo_closedirfd_all_up( masxfs_levinfo_t * li )
{
  int r = 0;

  do
  {
    r = masxfs_levinfo_closedirfd( li );
    if ( !li--->lidepth )
      break;
  }
  while ( !r );
  return 0;
}

masxfs_dir_t *
masxfs_levinfo_opendir_rel( masxfs_levinfo_t * li )
{
  if ( !li->dir )
  {
/*TODO fdopendir / openat / dirfd  */
    if ( li->lidepth > 0 )
    {
//    int fd = dirfd( li[-1].dir );
      li->dirfd = openat( li[-1].dirfd, li->name, O_DIRECTORY | O_NOFOLLOW | O_RDONLY );

      li->dir = fdopendir( li->dirfd );
      WARN( "li->dirfd:%d", li->dirfd );
    }
    else
    {
      li->dirfd = open( "/", O_DIRECTORY | O_NOFOLLOW | O_RDONLY );
      li->dir = fdopendir( li->dirfd );
    }
    if ( !li->dir )
    {
      DIE( "Can't open dir %s; fd:%d; lidepth:%ld", li->name, li->dirfd, li->lidepth );
    }
  }
  return li->dir;
}

void
masxfs_levinfo_closedir( masxfs_levinfo_t * li )
{
  closedir( li->dir );
}
