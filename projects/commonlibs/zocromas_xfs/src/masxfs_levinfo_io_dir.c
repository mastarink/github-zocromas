#include "masxfs_defs.h"
#include <string.h>
#include <errno.h>
/* #include <sys/types.h> */
/* #include <dirent.h> */

/* #include <sys/stat.h> */
/* #include <unistd.h> */
/* #include <fcntl.h> */

#include <mastar/wrap/mas_memory.h>
/* #include <mastar/tools/mas_arg_tools.h> */
#include <mastar/minierr/minierr.h>

#include "masxfs_levinfo_structs.h"
/* #include "masxfs_structs.h" */

/* #include "masxfs_levinfo.h" */
/* #include "masxfs_levinfo_ref.h" */

#include "masxfs_levinfo_io.h"
/* #include "masxfs_levinfo_tools.h" */

#include "masxfs_levinfo_io_dir.h"

/*
exiternal functions used:

 fdopendir
 closedir
 readdir
 rewinddir
*/

int
masxfs_levinfo_opendir( masxfs_levinfo_t * li )
{
  int r = 0;

  if ( li )
  {
    if ( !li->pdir )
    {
      int fd = masxfs_levinfo_open( li );

      if ( fd > 0 )
      {
        errno = 0;
        li->pdir = fdopendir( fd );
        if ( !li->pdir && errno )
          r = -1;
        QRLI( li, r );
        if ( r >= 0 && li->pdir )
        {
          li->detype = MASXFS_ENTRY_DIR_NUM;
          r = masxfs_levinfo_rewinddir( li );
          QRLI( li, r );
        }
        else
          r = -1;
        QRLI( li, r );
      }
      else
        r = -1;
      QRLI( li, r );
    }
  }
  else
    r = -1;

  QRLI( li, r );
  return r;
}

int
masxfs_levinfo_closedir( masxfs_levinfo_t * li )
{
  int r = 0;

  if ( li && li->pdir )
  {
    errno = 0;
    r = closedir( li->pdir );
    QRLI( li, r );
    li->pdir = NULL;
    li->fd = 0;                                                      /*  closedir closes fd!  */
  /* r = masxfs_levinfo_close( li ); */
  }
  else
    r = masxfs_levinfo_close( li );
#if 0
/* No */
  else
  r = -1;
#endif
  QRLI( li, r );
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
    QRLI( li, r );
#endif
    r = masxfs_levinfo_closedir( li );
    QRLI( li, r );
    if ( !li->lidepth )
      break;
    li--;
  } while ( r >= 0 );

  QRLI( li, r );
  return r;
}

masxfs_dirent_t *
masxfs_levinfo_readdir( masxfs_levinfo_t * li )
{
  int r = 0;
  masxfs_dirent_t *de = NULL;

  if ( li )
  {
    li->pde = NULL;
    if ( li->pdir )
    {
      errno = 0;

    /* r=readdir_r(li->pdir, &li->de,...  ); No! */
      li->pde = de = readdir( li->pdir );
    /* li->de = *de; */
      if ( !de && errno )
        r = -1;
      QRLI( li, r );
    }
  }
  QRLI( li, r );
  return de;
}

int
masxfs_levinfo_rewinddir( masxfs_levinfo_t * li )
{
  int r = 0;

  r = masxfs_levinfo_opendir( li );
  QRLI( li, r );
  if ( r >= 0 && li && li->pdir )
  {
    errno = 0;
    rewinddir( li->pdir );
    if ( errno )
      r = -1;
  }
  QRLI( li, r );
  return r;
}
