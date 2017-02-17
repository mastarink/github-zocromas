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

static masxfs_dir_t *
masxfs_levinfo_openpath( masxfs_levinfo_t * li, const char *real_path )
{
  masxfs_dir_t *dir = NULL;

  if ( li )
  {
    li->dir = dir = opendir( real_path );
    if ( li->dir )
    {
      rewinddir( li->dir );
    }
  }else
    DIE( "dir:%d", dir );
  return dir;
}

masxfs_dir_t *
masxfs_levinfo_openpath_free( masxfs_levinfo_t * li, char *real_path )
{
  masxfs_dir_t *dir = NULL;

  if ( li )
  {
    dir = masxfs_levinfo_openpath( li, real_path );
    mas_free( real_path );
  }
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

masxfs_dir_t *
masxfs_levinfo_opendir( masxfs_levinfo_t * li )
{
  masxfs_dir_t *dir = NULL;

  if ( li )
  {
    if ( li->dir )
      dir = li->dir;
    else
    {
      int fd = masxfs_levinfo_opendirfd( li );

      if ( fd > 0 )
      {
        li->dir = fdopendir( fd );
        if ( li->dir )
        {
          rewinddir( li->dir );
        }
      }
    }
  }
  return dir;
}

int
masxfs_levinfo_closedirfd( masxfs_levinfo_t * li )
{
  int r = close( li->dirfd );

  li->dirfd = 0;
  if ( r )
    DIE( "R:%d", r );
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
  } while ( !r );
  if ( r )
    DIE( "R:%d", r );
  return r;
}

int
masxfs_levinfo_closedir( masxfs_levinfo_t * li )
{
  int r = 0;

  if ( li )
  {
    closedir( li->dir );
    li->dir = NULL;
    r = masxfs_levinfo_closedirfd( li );
    if ( r )
      DIE( "R:%d", r );
  }
  else
    r = -1;
  return r;
}

masxfs_dirent_t *
masxfs_levinfo_readdir( masxfs_levinfo_t * li )
{
  masxfs_dirent_t *de = NULL;

  if ( li && li->dir )
    li->de = de = readdir( li->dir );
  else
    DIE( "DE:%p", de );
  return de;
}

int
masxfs_levinfo_rewinddir( masxfs_levinfo_t * li )
{
  int r = 0;

  if ( li && li->dir )
    rewinddir( li->dir );
  else
    r = -1;
  if ( r )
    DIE( "R:%d %d:%d", r, li ? 1 : 0, li && li->dir ? 1 : 0 );
  return r;
}
