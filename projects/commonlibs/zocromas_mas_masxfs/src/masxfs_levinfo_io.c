#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "masxfs_defs.h"
#include "masxfs_error.h"

#include "masxfs_structs.h"

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

  if ( li && !li->pdir )
  {
    int fd = masxfs_levinfo_opendirfd( li );

    if ( fd > 0 )
    {
      li->pdir = fdopendir( fd );
      if ( li->pdir )
        rewinddir( li->pdir );
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
    QR( r );
  }
/* else      */
/*   r = -1; */
  QR( r );
  return r;
}

int
masxfs_levinfo_closedirfd_all_up( masxfs_levinfo_t * li )
{
  int r = 0;

  do
  {
    r = masxfs_levinfo_closedirfd( li );
    QR( r );
    if ( !( li-- )->lidepth )
      break;
  } while ( !r );
  QR( r );
  return r;
}

int
masxfs_levinfo_closedir( masxfs_levinfo_t * li )
{
  int r = 0;

  if ( li && li->pdir )
  {
    r = closedir( li->pdir );
    QR( r );
    li->pdir = NULL;
    li->dirfd = 0;                                                   /*  closedir closes fd!  */
  /* r = masxfs_levinfo_closedirfd( li ); */
  }
  else
    r = masxfs_levinfo_closedirfd( li );
#if 0
/* No */
  else
  r = -1;
#endif
  QR( r );
  return r;
}

int
masxfs_levinfo_closedir_all_up( masxfs_levinfo_t * li )
{
  int r = 0;

  do
  {
#if 0
  /* No */
    if ( li && !li->pdir )
      r = -1;
    QR( r );
#endif
    r = masxfs_levinfo_closedir( li );
    QR( r );
    if ( !( li-- )->lidepth )
      break;
  } while ( !r );

  QR( r );
  return r;
}

masxfs_dirent_t *
masxfs_levinfo_readdir( masxfs_levinfo_t * li )
{
  int r = 0;
  masxfs_dirent_t *de = NULL;

  if ( li && li->pdir )
  {
    errno = 0;

  /* r=readdir_r(li->pdir, &li->de,...  ); No! */
    li->pde = de = readdir( li->pdir );
  /* li->de = *de; */
    if ( !de && errno )
      r = -1;
    QR( r );
  }
  else
    r = -1;
  QR( r );
  return de;
}

int
masxfs_levinfo_rewinddir( masxfs_levinfo_t * li )
{
  int r = 0;

  if ( li && li->pdir )
    rewinddir( li->pdir );
  else
    r = -1;
  QR( r );
  return r;
}
