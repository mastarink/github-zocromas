#include "masxfs_levinfo_defs.h"
#include <string.h>
#include <errno.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include <mastar/mysqlpfs/mysqlpfs_mstmt.h>

#include "masxfs_levinfo_structs.h"
#include "masxfs_levinfo_mode.h"

#include "masxfs_levinfo_ref.h"
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
  int r = 0;

  r = masxfs_levinfo_fs_opendir( li );
  QRLI( li, r );
  if ( r >= 0 && li && li->fs.pdir )
  {
    errno = 0;
    rewinddir( li->fs.pdir );
    if ( errno )
      r = -1;
  }
  QRLI( li, r );
  return r;
}

static int
masxfs_levinfo_db_rewinddir( masxfs_levinfo_t * li _uUu_ )
{
  int r = -1;

  mas_mysqlpfs_mstmt_data_seek( li->db.mstmt, 0 );
  return r;
}

int
masxfs_levinfo_rewinddir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  int r = -1;

  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  switch ( mode )
  {
  case MASXFS_SCAN__MODE_NONE:
    r = -1;
    QRLI( li, r );
    break;
  case MASXFS_SCAN__MODE_FS:
    r = masxfs_levinfo_fs_rewinddir( li );
    QRLI( li, r );
    break;
  case MASXFS_SCAN__MODE_DB:
    r = masxfs_levinfo_db_rewinddir( li );
    QRLI( li, r );
    break;
  }
  QRLI( li, r );
  return r;
}

static int
masxfs_levinfo_fs_opendir( masxfs_levinfo_t * li )
{
  int r = 0;

  if ( li )
  {
    if ( !li->fs.pdir )
    {
      int fd = masxfs_levinfo_open( li, MASXFS_CB_MODE_FS );

      if ( fd > 0 )
      {
        errno = 0;
        li->fs.pdir = fdopendir( fd );
        if ( !li->fs.pdir && errno )
          r = -1;
        QRLI( li, r );
        if ( r >= 0 && li->fs.pdir )
        {
          li->detype = MASXFS_ENTRY_DIR_NUM;
          r = masxfs_levinfo_fs_rewinddir( li );
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

static int
masxfs_levinfo_db_opendir( masxfs_levinfo_t * li _uUu_ )
{
  int r = -1;

  return r;
}

int
masxfs_levinfo_opendir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  int r = -1;

  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  switch ( mode )
  {
  case MASXFS_SCAN__MODE_NONE:
    r = -1;
    break;
  case MASXFS_SCAN__MODE_FS:
    r = masxfs_levinfo_fs_opendir( li );
    break;
  case MASXFS_SCAN__MODE_DB:
    r = masxfs_levinfo_db_opendir( li );
    break;
  }
  return r;
}

static int
masxfs_levinfo_fs_closedir( masxfs_levinfo_t * li )
{
  int r = -1;

  if ( li && li->fs.pdir )
  {
    errno = 0;
    r = closedir( li->fs.pdir );
    QRLI( li, r );
    li->fs.pdir = NULL;
    li->fd = 0;                                                      /*  closedir closes fd!  */
  /* r = masxfs_levinfo_fs_close( li ); */
  }
  else
    r = masxfs_levinfo_close( li, MASXFS_CB_MODE_FS );
#if 0
/* No */
  else
  r = -1;
#endif
  QRLI( li, r );
  return r;
}

static int
masxfs_levinfo_db_closedir( masxfs_levinfo_t * li _uUu_ )
{
  int r = -1;

  return r;
}

int
masxfs_levinfo_closedir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  int r = -1;

  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  switch ( mode )
  {
  case MASXFS_SCAN__MODE_NONE:
    r = -1;
    break;
  case MASXFS_SCAN__MODE_FS:
    r = masxfs_levinfo_fs_closedir( li );
    break;
  case MASXFS_SCAN__MODE_DB:
    r = masxfs_levinfo_db_closedir( li );
    break;
  }
  return r;
}

int
masxfs_levinfo_closedir_all_up( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  int r = 0;

  do
  {
#if 0
  /* No */
    if ( li && !li->fs.pdir )
      r = -1;
    QRLI( li, r );
#endif
    r = masxfs_levinfo_closedir( li, flags );
    QRLI( li, r );
    if ( !li->lidepth )
      break;
    li--;
  } while ( r >= 0 );

  QRLI( li, r );
  return r;
}

static masxfs_dirent_t *
masxfs_levinfo_fs_readdir( masxfs_levinfo_t * li )
{
  int r = 0;
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
      if ( !de && errno )
        r = -1;
      QRLI( li, r );
    }
  }
  QRLI( li, r );
  return de;
}

static masxfs_dirent_t *
masxfs_levinfo_db_readdir( masxfs_levinfo_t * li _uUu_ )
{
  int r _uUu_ = -1;
  masxfs_dirent_t *de = NULL;

  return de;
}

masxfs_dirent_t *
masxfs_levinfo_readdir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  int r _uUu_ = -1;
  masxfs_dirent_t *de = NULL;

  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  switch ( mode )
  {
  case MASXFS_SCAN__MODE_NONE:
    r = -1;
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
