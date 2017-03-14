#define R_GOOD(_r) (_r>=0)
#include "masxfs_levinfo_defs.h"
#include <string.h>
#include <errno.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include <mastar/qstd/qstd_mstmt.h>

#include "masxfs_levinfo_structs.h"
#include "masxfs_levinfo_mode.h"

#include "masxfs_levinfo_tools.h"
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
  rDECLBAD;

  rC( masxfs_levinfo_fs_opendir( li ) );
  if ( rGOOD && li && li->fs.pdir )
  {
    errno = 0;
    rewinddir( li->fs.pdir );
    if ( !errno )
      rSETGOOD;
    QRLI( li, rCODE );
  }
  rRET;
}

int
masxfs_levinfo_rewinddir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;

  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  switch ( mode )
  {
  case MASXFS_SCAN__MODE_NONE:
  /* rSETBAD; */
    QRLI( li, rCODE );
    break;
  case MASXFS_SCAN__MODE_FS:
    rC( masxfs_levinfo_fs_rewinddir( li ) );
    break;
  case MASXFS_SCAN__MODE_DB:
    WARN( "TO DB REWIND" );
    rC( masxfs_levinfo_db_rewinddir( li ) );
    break;
  }
  rRET;
}

static int
masxfs_levinfo_fs_opendir( masxfs_levinfo_t * li )
{
  rDECLBAD;

  if ( li )
  {
    rSETGOOD;
    if ( !li->fs.pdir )
    {
      int fd = masxfs_levinfo_open( li, MASXFS_CB_MODE_FS );

      if ( fd > 0 )
      {
        errno = 0;
        li->fs.pdir = fdopendir( fd );
        if ( !li->fs.pdir && errno )
          rSETBAD;
        QRLI( li, rCODE );
        if ( rGOOD && li->fs.pdir )
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
masxfs_levinfo_opendir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;

  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  switch ( mode )
  {
  case MASXFS_SCAN__MODE_NONE:
  /* rSETBAD; */
    QRLI( li, rCODE );
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
  rDECLBAD;

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
  rRET;
}

int
masxfs_levinfo_closedir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;

  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  switch ( mode )
  {
  case MASXFS_SCAN__MODE_NONE:
  /* rSETBAD; */
    QRLI( li, rCODE );
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
  rDECLBAD;

  do
  {
    rC( masxfs_levinfo_closedir( li, flags ) );
    if ( !li->lidepth )
      break;
    li--;
  } while ( rGOOD );

  rRET;
}

const char *
masxfs_levinfo_scanned_name( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  const char *name = NULL;

  rDECLBAD;
  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  if ( li )
  {
    switch ( mode )
    {
    case MASXFS_SCAN__MODE_NONE:
    /* rSETBAD; */
      QRLI( li, rCODE );
      break;
    case MASXFS_SCAN__MODE_FS:
      if ( li->fs.pde )
        name = li->fs.pde->d_name;
      break;
    case MASXFS_SCAN__MODE_DB:
      if ( li->db.mstmt )
        mas_qstd_mstmt_get_result_string_na( li->db.mstmt, 0, &name );
      break;
    }
  }
  else
    QRLI( li, rCODE );
  return name;
}

int
masxfs_levinfo_scanned_detype( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;
  int type = 0;

  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  if ( li )
  {
    switch ( mode )
    {
    case MASXFS_SCAN__MODE_NONE:
    /* rSETBAD; */
      QRLI( li, rCODE );
      break;
    case MASXFS_SCAN__MODE_FS:
      if ( li->fs.pde )
        type = masxfs_levinfo_de2entry( li->fs.pde->d_type );
      break;
    case MASXFS_SCAN__MODE_DB:
      type = ( li->db.detype );
      break;
    }
  }
  else
    QRLI( li, rCODE );
  return type;
}

ino_t
masxfs_levinfo_scanned_inode( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;
  ino_t inode = 0;

  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  if ( li )
  {
    switch ( mode )
    {
    case MASXFS_SCAN__MODE_NONE:
    /* rSETBAD; */
      QRLI( li, rCODE );
      break;
    case MASXFS_SCAN__MODE_FS:
      if ( li->fs.pde )
        inode = li->fs.pde->d_ino;
      break;
    case MASXFS_SCAN__MODE_DB:
      inode = li->db.deinode;
      break;
    }
  }
  return inode;
}

unsigned long long
masxfs_levinfo_scanned_nodeid( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;
  unsigned long long node_id = 0;

  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  if ( li )
  {
    switch ( mode )
    {
    case MASXFS_SCAN__MODE_NONE:
    /* rSETBAD; */
      QRLI( li, rCODE );
      break;
    case MASXFS_SCAN__MODE_FS:
      node_id = 0;
      break;
    case MASXFS_SCAN__MODE_DB:
      node_id = li->db.denode_id;
      break;
    }
  }
  return node_id;
}

static int
masxfs_levinfo_fs_readdir( masxfs_levinfo_t * li, int *phas_data )
{
  rDECLBAD;
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

int
masxfs_levinfo_readdir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags, int *phas_data )
{
  rDECLBAD;

  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  switch ( mode )
  {
  case MASXFS_SCAN__MODE_NONE:
  /* rSETBAD; */
    QRLI( li, rCODE );
    break;
  case MASXFS_SCAN__MODE_FS:
    rC( masxfs_levinfo_fs_readdir( li, phas_data ) );
    break;
  case MASXFS_SCAN__MODE_DB:
    WARN( "TO READ DB DIR \"%s\"", li->name );
    rC( masxfs_levinfo_db_readdir( li, phas_data ) );
    WARN( "READ DB DIR - done \"%s\" -> \"%s\"", li->name, masxfs_levinfo_scanned_name( li, flags ) );
    break;
  }
  rRET;
}
