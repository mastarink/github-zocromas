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

static int
masxfs_levinfo_opendir_at( masxfs_levinfo_t * li, int fd )
{
  int r = -1;

  if ( li && li->name )
    r = li->fd = openat( fd, li->name, O_DIRECTORY | O_NOFOLLOW | O_RDONLY );
  return r;
}

int
masxfs_levinfo_opendirfd( masxfs_levinfo_t * li )
{
  if ( !li->fd )
  {
    if ( li->lidepth > 0 )
      masxfs_levinfo_opendir_at( li, masxfs_levinfo_opendirfd( li - 1 ) );
    else if ( li->name && !*li->name )
      li->fd = open( "/", O_DIRECTORY | O_NOFOLLOW | O_RDONLY );
  }
  return li->fd;
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
      errno = 0;
      li->pdir = fdopendir( fd );
      if ( !li->pdir && errno )
        r = -1;
      QRLI( li,  r );
      if ( !r && li->pdir )
        rewinddir( li->pdir );
      else
        r = -1;
      QRLI( li,  r );
    }
    else
      r = -1;
    QRLI( li,  r );
  }
  else
    r = -1;
  QRLI( li,  r );
  return r;
}

int
masxfs_levinfo_closedirfd( masxfs_levinfo_t * li )
{
  int r = 0;

  if ( li && li->fd )
  {
    r = close( li->fd );
    li->fd = 0;
    QRLI( li,  r );
  }
/* else      */
/*   r = -1; */
  QRLI( li,  r );
  return r;
}

int
masxfs_levinfo_closedirfd_all_up( masxfs_levinfo_t * li )
{
  int r = 0;

  do
  {
    r = masxfs_levinfo_closedirfd( li );
    QRLI( li,  r );
    if ( !( li-- )->lidepth )
      break;
  } while ( !r );
  QRLI( li,  r );
  return r;
}

int
masxfs_levinfo_closedir( masxfs_levinfo_t * li )
{
  int r = 0;

  if ( li && li->pdir )
  {
    r = closedir( li->pdir );
    QRLI( li,  r );
    li->pdir = NULL;
    li->fd = 0;                                                   /*  closedir closes fd!  */
  /* r = masxfs_levinfo_closedirfd( li ); */
  }
  else
    r = masxfs_levinfo_closedirfd( li );
#if 0
/* No */
  else
  r = -1;
#endif
  QRLI( li,  r );
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
    QRLI( li,  r );
#endif
    r = masxfs_levinfo_closedir( li );
    QRLI( li,  r );
    if ( !( li-- )->lidepth )
      break;
  } while ( !r );

  QRLI( li,  r );
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
    QRLI( li,  r );
  }
  else
    r = -1;
  QRLI( li,  r );
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
  QRLI( li,  r );
  return r;
}
