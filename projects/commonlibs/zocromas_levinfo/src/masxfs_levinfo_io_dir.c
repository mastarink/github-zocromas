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
  rDECL( 0 );

  rC( masxfs_levinfo_fs_opendir( li ) );
  QRLI( li, rCODE );
  if ( !rCODE && li && li->fs.pdir )
  {
    errno = 0;
    rewinddir( li->fs.pdir );
    if ( errno )
      rCODE = -1;
  }
  QRLI( li, rCODE );
  rRET;
}

static int
masxfs_levinfo_db_rewinddir( masxfs_levinfo_t * li _uUu_ )
{
  rDECL( 0 );

  DIE( "NOT IMPLEMENTED" );
  rCODE = -1;
  mas_mysqlpfs_mstmt_data_seek( li->db.mstmt, 0 );
  rRET;
}

int
masxfs_levinfo_rewinddir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECL( -1 );

  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  switch ( mode )
  {
  case MASXFS_SCAN__MODE_NONE:
    rCODE = -1;
    QRLI( li, rCODE );
    break;
  case MASXFS_SCAN__MODE_FS:
    rC( masxfs_levinfo_fs_rewinddir( li ) );
    QRLI( li, rCODE );
    break;
  case MASXFS_SCAN__MODE_DB:
    DIE( "NOT IMPLEMENTED" );
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
        if ( !rCODE && li->fs.pdir )
        {
          li->detype = MASXFS_ENTRY_DIR_NUM;
          rC( masxfs_levinfo_fs_rewinddir( li ) );
          QRLI( li, rCODE );
        }
        else
          rCODE = -1;
        QRLI( li, rCODE );
      }
      else
        rCODE = -1;
      QRLI( li, rCODE );
    }
    QRLI( li, rCODE );
  }
  else
    rCODE = -1;
  QRLI( li, rCODE );
  rRET;
}

static int
masxfs_levinfo_db_opendir( masxfs_levinfo_t * li _uUu_ )
{
  rDECL( 0 );
  DIE( "NOT IMPLEMENTED" );
  rCODE = -1;
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
    rCODE = -1;
    break;
  case MASXFS_SCAN__MODE_FS:
    rC( masxfs_levinfo_fs_opendir( li ) );
    break;
  case MASXFS_SCAN__MODE_DB:
    DIE( "NOT IMPLEMENTED" );
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
  rCODE = -1;
#endif
  QRLI( li, rCODE );
  rRET;
}

static int
masxfs_levinfo_db_closedir( masxfs_levinfo_t * li _uUu_ )
{
  rDECL( 0 );
  if ( li->db.mstmt )
  {
    WARN( "NOT IMPLEMENTED" );
    assert( 0 );
    DIE( "NOT IMPLEMENTED" );
    rCODE = -1;
  }
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
    rCODE = -1;
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
      rCODE = -1;
    QRLI( li, rCODE );
#endif
    rC( masxfs_levinfo_closedir( li, flags ) );
    QRLI( li, rCODE );
    if ( !li->lidepth )
      break;
    li--;
  } while ( !rCODE );

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

  DIE( "NOT IMPLEMENTED" );
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
