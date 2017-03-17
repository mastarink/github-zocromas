#define R_GOOD(_r) (_r>=0)
#include "masxfs_levinfo_defs.h"
#include <string.h>
#include <errno.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

/* #include <mastar/qstd/qstd_mstmt.h> */

#include "masxfs_levinfo_structs.h"
#include "masxfs_levinfo_mode.h"

#include "masxfs_levinfo_tools.h"
#include "masxfs_levinfo_ref.h"
#include "masxfs_levinfo_db.h"
#include "masxfs_levinfo_io.h"

#include "masxfs_levinfo_io_dir.h"
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
masxfs_levinfo_fs_rewinddir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;

  rC( masxfs_levinfo_fs_opendir( li, flags ) );
  if ( rGOOD && li && li->fs.scan.pdir )
  {
    errno = 0;
    rewinddir( li->fs.scan.pdir );
    if ( !errno )
      rSETGOOD;
    QRLI( li, rCODE );
  }
  rRET;
}

int
masxfs_levinfo_fs_opendir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;

  if ( li )
  {
    rSETGOOD;
    
    /* TODO */
#if 0
    if ( li->lidepth && !li[-1].fs.scan.pdir )
      rC( masxfs_levinfo_fs_opendir( li - 1, flags ) );
#endif

    /* WARN( "A %lld '%s' ==> '%s'", li[0].db.node_id, li[0].name, li[1].name ); */
    if ( rGOOD && !li->fs.scan.pdir )
    {
      int fd = masxfs_levinfo_fs_open( li, flags );

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
          rC( masxfs_levinfo_fs_rewinddir( li, flags ) );
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
    /* WARN( "B %lld '%s' ==> '%s'", li[0].db.node_id, li[0].name, li[1].name ); */
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
masxfs_levinfo_fs_readdir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags _uUu_, int *phas_data )
{
  rDECLBAD;
  masxfs_dirent_t *de = NULL;

  if ( li )
  {
    li->fs.scan.pde = NULL;
    if ( li->fs.scan.pdir )
    {
      errno = 0;
    /* r=readdir_r(li->fs.scan.pdir, &li->de,...  ); No! */
      li->fs.scan.pde = de = readdir( li->fs.scan.pdir );
    /* li->de = *de; */
      /* TODO : directly to li[1]; i.e. init child here, no need of li->fs.scan.pde, just local pde */
      if ( de || !errno )
        rSETGOOD;
      QRLI( li, rCODE );
    }
    if ( phas_data )
    {
      *phas_data = ( de ? 1 : 0 );
    }
  }
  else
    QRLI( li, rCODE );
  rRET;
}
