#include <stdio.h>
#include <string.h>
#include <errno.h>

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

int
masxfs_levinfo_opendirfd( masxfs_levinfo_t * li )
{
  if ( !li->dirfd )
  {
    if ( li->lidepth > 0 )
      li->dirfd = openat( masxfs_levinfo_opendirfd( li - 1 ), li->name, O_DIRECTORY | O_NOFOLLOW | O_RDONLY );
    else if ( li->name && !*li->name )
      li->dirfd = open( "/", O_DIRECTORY | O_NOFOLLOW | O_RDONLY );
  }
  return li->dirfd;
}

int
masxfs_levinfo_opendir( masxfs_levinfo_t * li )
{
  int r = 0;

  if ( li && !li->dir )
  {
    int fd = masxfs_levinfo_opendirfd( li );

    if ( fd > 0 )
    {
      li->dir = fdopendir( fd );
      if ( li->dir )
        rewinddir( li->dir );
      else
        r = -1;
    }
    else
      r = -1;
  }
  return r;
}

int
masxfs_levinfo_closedirfd( masxfs_levinfo_t * li )
{
  int r = 0;

  if ( li && li->dirfd )
  {
    r = close( li->dirfd );
    li->dirfd = 0;
    QRDIE( r );
  }
  else
    r = -1;
  QRDIE( r );
  return r;
}

int
masxfs_levinfo_closedirfd_all_up( masxfs_levinfo_t * li )
{
  int r = 0;

  do
  {
    r = masxfs_levinfo_closedirfd( li );
    QRDIE( r );
    if ( !( li-- )->lidepth )
      break;
  } while ( !r );
  QRDIE( r );
  return r;
}

int
masxfs_levinfo_closedir( masxfs_levinfo_t * li )
{
  int r = 0;

  if ( li && li->dir )
  {
    r = closedir( li->dir );
    QRDIE( r );
    li->dir = NULL;
    li->dirfd = 0;                                                   /*  closedir closes fd!  */
  /* r = masxfs_levinfo_closedirfd( li ); */
  }
  else
    r = -1;
  QRDIE( r );
  return r;
}

masxfs_dirent_t *
masxfs_levinfo_readdir( masxfs_levinfo_t * li )
{
  int r = 0;
  masxfs_dirent_t *de = NULL;

  if ( li && li->dir )
  {

    errno = 0;
    li->de = de = readdir( li->dir );
    if ( !de && errno )
      r = -1;
    QRDIE( r );
  }
  else
    r = -1;
  QRDIE( r );
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
  QRDIE( r );
  return r;
}
