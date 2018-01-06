#define R_GOOD(_r) ((_r)>=0)
#include "masxfs_levinfo_defs.h"
#include <string.h>
/* #include <unistd.h> */
#include <errno.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_levinfo_structs.h"

#include "masxfs_levinfo_tools.h"
#include "masxfs_levinfo_base.h"
#include "masxfs_levinfo_io.h"

#include "masxfs_levinfo_fs.h"

#include "masxfs_levinfo_fs_dir.h"

/*
exiternal functions used:

 fdopendir
 closedir
 readdir
 rewinddir
*/

int
masxfs_levinfo_fs_rewinddir( masxfs_levinfo_t * li )
{
  rDECLBAD;

  rC( masxfs_levinfo_fs_opendir( li ) );
  if ( rGOOD && li && li->fs.scan.pdir )
  {
    errno = 0;
    rewinddir( li->fs.scan.pdir );
    if ( !errno )
      rSETGOOD;
    else
      QRLI( li, rCODE );
  }
  else
    QRLI( li, rCODE );
  rRET;
}

int
masxfs_levinfo_fs_opendir( masxfs_levinfo_t * li )
{
  rDECLBAD;

  if ( li )
  {
    rSETGOOD;

    if ( !li->fs.scan.pdir )
    {
      int fd = masxfs_levinfo_fs_open( li );

      if ( fd > 0 )
      {
        errno = 0;
        li->fs.scan.pdir = fdopendir( fd );
        if ( !li->fs.scan.pdir && errno )
          rSETBAD;
        QRLI( li, rCODE );
        if ( rGOOD && li->fs.scan.pdir )
        {
          li->detype = MASXFS_ENTRY_DIR_NUM;
          rC( masxfs_levinfo_fs_rewinddir( li ) );
        }
        else
        {
          rSETBAD;
          QRLI( li, rCODE );
        }
      }
      else
      {
        rSETBAD;
        QRLI( li, rCODE );
      }
    }
  }
  else
    QRLI( li, rCODE );
  rRET;
}

int
masxfs_levinfo_fs_closedir( masxfs_levinfo_t * li )
{
  rDECLBAD;

  if ( li && li->fs.scan.pdir )
  {
    errno = 0;
    rC( closedir( li->fs.scan.pdir ) );
    QRLI( li, rCODE );
    li->fs.scan.pdir = NULL;
    li->fd = 0;                                                      /*  closedir closes fd!  */
  /* rC( masxfs_levinfo_fs_close( li )); */
  }
  else
    rC( masxfs_levinfo_close( li, MASXFS_CB_MODE_FS ) );
  rRET;
}

int
masxfs_levinfo_fs_readdir( masxfs_levinfo_t * li, masxfs_entry_filter_t * entry_pfilter, int *phas_data )
{
  rDECLBAD;
  masxfs_dirent_t *de = NULL;
  int has_data = 0;

  if ( li )
  {
    if ( li->fs.scan.pdir )
    {
      masxfs_entry_type_t detype = MASXFS_ENTRY_NONE_NUM;
      const char *dename = NULL;

      do
      {
        rSETBAD;
        errno = 0;
        de = readdir( li->fs.scan.pdir );

        if ( de || !errno )
          rSETGOOD;
        detype = de && rGOOD ? masxfs_levinfo_de2entry( de->d_type ) : MASXFS_ENTRY_UNKNOWN_NUM;
        dename = de ? de->d_name : NULL;
      } while ( dename && !( masxfs_levinfo_name_valid( dename, detype, entry_pfilter ) ) );
      if ( rGOOD && de )
      {
        const char *dename = de->d_name;

      /* have de->d_name, de->d_type and de->d_ino */
        masxfs_levinfo_init( li + 1, li->lidepth + 1, dename, detype, NULL, NULL, 0 );
        assert( li[0].lidepth + 1 == li[1].lidepth );
        has_data = de ? 1 : 0;
      }
      QRLI( li, rCODE );
    }
    else
      QRLI( li, rCODE );
  }
  else
    QRLI( li, rCODE );
  if ( phas_data )
    *phas_data = has_data;
  rRET;
}
