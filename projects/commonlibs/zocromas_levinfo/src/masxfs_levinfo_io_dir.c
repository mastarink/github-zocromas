#include "masxfs_levinfo_defs.h"
#include <string.h>
#include <errno.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include <mastar/mysqlpfs/mysqlpfs_mstmt.h>

#include "masxfs_levinfo_structs.h"
#include "masxfs_levinfo_mode.h"

#include "masxfs_levinfo_ref.h"
#include "masxfs_levinfo_db.h"
#include "masxfs_levinfo_io.h"
#include "masxfs_levinfo_io_dir.h"

/*
exiternal functions used:

 fdopendir
 closedir
 readdir
 rewinddir
*/

static inline masxfs_scan_mode_t
masxfs_levinfo_flags_mode( masxfs_levinfo_flags_t flags )
{
  return ( flags & MASXFS_CB_MODE_DB ) ? MASXFS_SCAN__MODE_DB : ( ( flags & MASXFS_CB_MODE_FS ) ? MASXFS_SCAN__MODE_FS : MASXFS_SCAN__MODE_NONE );
}

static int masxfs_levinfo_fs_opendir( masxfs_levinfo_t * li );
static int
masxfs_levinfo_fs_rewinddir( masxfs_levinfo_t * li )
{
  rDECL( 0 );

  rC( masxfs_levinfo_fs_opendir( li ) );
  QRLI( li, rCODE );
  if ( rGOOD && li && li->fs.pdir )
  {
    errno = 0;
    rewinddir( li->fs.pdir );
    if ( errno )
      rSETBAD;
  }
  QRLI( li, rCODE );
  rRET;
}

int
masxfs_levinfo_rewinddir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECL( -1 );

  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  if ( ( flags & MASXFS_CB_MODE_DB ) )
    WARN( "POINT DB" );

  switch ( mode )
  {
  case MASXFS_SCAN__MODE_NONE:
    rSETBAD;
    QRLI( li, rCODE );
    break;
  case MASXFS_SCAN__MODE_FS:
    rC( masxfs_levinfo_fs_rewinddir( li ) );
    QRLI( li, rCODE );
    break;
  case MASXFS_SCAN__MODE_DB:
    if ( ( flags & MASXFS_CB_MODE_DB ) )
      WARN( "POINT DB" );
    rC( masxfs_levinfo_db_rewinddir( li ) );
    QRLI( li, rCODE );
    break;
  }
  QRLI( li, rCODE );
  rRET;
}

static int
masxfs_levinfo_fs_opendir( masxfs_levinfo_t * li )
{
  rDECL( -1 );

  if ( li )
  {
    rCODE = 0;
    if ( !li->fs.pdir )
    {
      int fd = masxfs_levinfo_open( li, MASXFS_CB_MODE_FS );

      if ( fd > 0 )
      {
        errno = 0;
        li->fs.pdir = fdopendir( fd );
        rCODE = ( !li->fs.pdir && errno ) ? -1 : 0;
        QRLI( li, rCODE );
        if ( rGOOD && li->fs.pdir )
        {
          li->detype = MASXFS_ENTRY_DIR_NUM;
          rC( masxfs_levinfo_fs_rewinddir( li ) );
          QRLI( li, rCODE );
        }
        else
          rSETBAD;
        QRLI( li, rCODE );
      }
      else
        rSETBAD;
      QRLI( li, rCODE );
    }
    QRLI( li, rCODE );
  }
  else
    rSETBAD;
  QRLI( li, rCODE );
  rRET;
}

int
masxfs_levinfo_opendir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECL( -1 );

  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  switch ( mode )
  {
  case MASXFS_SCAN__MODE_NONE:
    rSETBAD;
    break;
  case MASXFS_SCAN__MODE_FS:
    rC( masxfs_levinfo_fs_opendir( li ) );
    break;
  case MASXFS_SCAN__MODE_DB:
    rC( masxfs_levinfo_db_opendir( li ) );
    break;
  }
  rRET;
}

static int
masxfs_levinfo_fs_closedir( masxfs_levinfo_t * li )
{
  rDECL( -1 );

  if ( li && li->fs.pdir )
  {
    errno = 0;
    rC( closedir( li->fs.pdir ) );
    QRLI( li, rCODE );
    li->fs.pdir = NULL;
    li->fd = 0;                                                      /*  closedir closes fd!  */
  /* rC( masxfs_levinfo_fs_close( li )); */
  }
  else
    rC( masxfs_levinfo_close( li, MASXFS_CB_MODE_FS ) );
#if 0
/* No */
  else
  rSETBAD;
#endif
  QRLI( li, rCODE );
  rRET;
}

int
masxfs_levinfo_closedir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECL( -1 );

  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  switch ( mode )
  {
  case MASXFS_SCAN__MODE_NONE:
    rSETBAD;
    break;
  case MASXFS_SCAN__MODE_FS:
    rC( masxfs_levinfo_fs_closedir( li ) );
    break;
  case MASXFS_SCAN__MODE_DB:
    rC( masxfs_levinfo_db_closedir( li ) );
    break;
  }
  rRET;
}

int
masxfs_levinfo_closedir_all_up( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECL( -1 );

  do
  {
#if 0
  /* No */
    if ( li && !li->fs.pdir )
      rSETBAD;
    QRLI( li, rCODE );
#endif
    rC( masxfs_levinfo_closedir( li, flags ) );
    QRLI( li, rCODE );
    if ( !li->lidepth )
      break;
    li--;
  } while ( rGOOD );

  QRLI( li, rCODE );
  rRET;
}

static masxfs_dirent_t *
masxfs_levinfo_fs_readdir( masxfs_levinfo_t * li )
{
  rDECL( -1 );
  masxfs_dirent_t *de = NULL;

  if ( li )
  {
    li->fs.pde = NULL;
    if ( li->fs.pdir )
    {
      errno = 0;

    /* r=readdir_r(li->fs.pdir, &li->de,...  ); No! */
      li->fs.pde = de = readdir( li->fs.pdir );
    /* li->de = *de; */
      rCODE = ( !de && errno ? -1 : 0 );
      QRLI( li, rCODE );
    }
  }
  QRLI( li, rCODE );
  return de;
}

static masxfs_dirent_t *
masxfs_levinfo_db_readdir( masxfs_levinfo_t * li _uUu_ )
{
  masxfs_dirent_t *de = NULL;

  if ( li )
  {
    if ( li->db.mstmt )
    {
      NIMP( "li:%p", li );
    /* li->db.pde=.... */
    }
  }
  return de;
}

masxfs_dirent_t *
masxfs_levinfo_readdir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  masxfs_dirent_t *de = NULL;

  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  switch ( mode )
  {
  case MASXFS_SCAN__MODE_NONE:
    break;
  case MASXFS_SCAN__MODE_FS:
    de = masxfs_levinfo_fs_readdir( li );
    break;
  case MASXFS_SCAN__MODE_DB:
    de = masxfs_levinfo_db_readdir( li );
    break;
  }
  return de;
}
