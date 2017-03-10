#include "masxfs_levinfo_defs.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
/* #include <sys/types.h> */
#include <sys/stat.h>
#include <fcntl.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_levinfo_structs.h"
#include "masxfs_levinfo_mode.h"

#include "masxfs_levinfo_tools.h"
#include "masxfs_levinfo_ref.h"

#include "masxfs_levinfo_io.h"

/*
exiternal functions used:

 openat
 open
 close
 fstatat
 fstat
 
 */

static inline masxfs_scan_mode_t
masxfs_levinfo_flags_mode( masxfs_levinfo_flags_t flags )
{
  return ( flags & MASXFS_CB_MODE_DB ) ? MASXFS_SCAN__MODE_DB : ( ( flags & MASXFS_CB_MODE_FS ) ? MASXFS_SCAN__MODE_FS : MASXFS_SCAN__MODE_NONE );
}

static int
masxfs_levinfo_fs_open_at( masxfs_levinfo_t * li, int fdparent )
{
  int fd = -1;

  if ( li->fd )
    fd = li->fd;
  else if ( li && li->name )
  {
  /* TODO: O_NOFOLLOW :: If  pathname  is a  symbolic  link, then the open fails */
    int openflags = ( li->detype == MASXFS_ENTRY_DIR_NUM ? O_DIRECTORY : 0 ) /* | O_NOFOLLOW */  | O_RDONLY;

    errno = 0;
    fd = li->fd = openat( fdparent, li->name, openflags );
    if ( fd < 0 && errno == ENOENT && li->detype == MASXFS_ENTRY_LNK_NUM )
    {
    /* ignore dead symbolic link */
      fd = li->fd = 0;
    }
    else
    {
      QRLI( li, fd );
      if ( fd > 0 && li->detype == MASXFS_ENTRY_UNKNOWN_NUM && ( openflags & O_DIRECTORY ) )
        li->detype = MASXFS_ENTRY_DIR_NUM;
      if ( fd < 0 /* && li->detype == MASXFS_ENTRY_UNKNOWN_NUM */  )
        WARN( "NOT OPEN (%d) %s %d %d %d", fdparent, li->name, li->detype == MASXFS_ENTRY_DIR_NUM, li->detype == MASXFS_ENTRY_UNKNOWN_NUM,
              openflags & O_DIRECTORY ? 1 : 0 );
    }
  }
  return fd;
}

static int
masxfs_levinfo_db_open_at( masxfs_levinfo_t * li _uUu_, int fdparent _uUu_ )
{
  rDECL( 0 );
  DIE( "NOT IMPLEMENTED" );
  rCODE = -1;
  rRET;
}

static int _uUu_
masxfs_levinfo_open_at( masxfs_levinfo_t * li, int fdparent, masxfs_levinfo_flags_t flags )
{
  rDECL( 0 );

  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  switch ( mode )
  {
  case MASXFS_SCAN__MODE_NONE:
    rCODE = -1;
    break;
  case MASXFS_SCAN__MODE_FS:
    rC( masxfs_levinfo_fs_open_at( li, fdparent ) );
    break;
  case MASXFS_SCAN__MODE_DB:
    rC( masxfs_levinfo_db_open_at( li, fdparent ) );
    break;
  }
  rRET;
}

static int
masxfs_levinfo_fs_open( masxfs_levinfo_t * li )
{
  rDECL( 0 );

  if ( li->lidepth > 0 )
    rC( masxfs_levinfo_fs_open_at( li, masxfs_levinfo_fs_open( li - 1 ) ) );
  else if ( !li->fd && li->name && !*li->name )
  {
    errno = 0;
    li->fd = open( "/", O_DIRECTORY | /* O_NOFOLLOW | */ O_RDONLY );
    if ( li->fd < 0 )
    {
      rCODE = li->fd;
      li->fd = 0;
    }
  }
  if ( li->fd < 0 )
    rCODE = -1;
  QRLI( li, rCODE );
  return li->fd;
}

static int
masxfs_levinfo_db_open( masxfs_levinfo_t * li _uUu_ )
{
  rDECL( 0 );
  DIE( "NOT IMPLEMENTED" );
  rCODE = -1;
  rRET;
}

int
masxfs_levinfo_open( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECL( 0 );

  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  switch ( mode )
  {
  case MASXFS_SCAN__MODE_NONE:
    rCODE = -1;
    break;
  case MASXFS_SCAN__MODE_FS:
    rC( masxfs_levinfo_fs_open( li ) );
    break;
  case MASXFS_SCAN__MODE_DB:
    rC( masxfs_levinfo_db_open( li ) );
    break;
  }
  rRET;
}

static int
masxfs_levinfo_fs_close( masxfs_levinfo_t * li )
{
  rDECL( 0 );

  if ( li && li->fd )
  {
    errno = 0;
    rCODE = close( li->fd );
    li->fd = 0;
    QRLI( li, rCODE );
  }
/* else      */
/*   rCODE = -1; */
  QRLI( li, rCODE );
  rRET;
}

static int
masxfs_levinfo_db_close( masxfs_levinfo_t * li )
{
  rDECL( 0 );
  rC( masxfs_levinfo_fs_close( li ) );
  rRET;
}

int
masxfs_levinfo_close( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECL( 0 );

  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  switch ( mode )
  {
  case MASXFS_SCAN__MODE_NONE:
    rCODE = -1;
    QRLI( li, rCODE );
    break;
  case MASXFS_SCAN__MODE_FS:
    rC( masxfs_levinfo_fs_close( li ) );
    QRLI( li, rCODE );
    break;
  case MASXFS_SCAN__MODE_DB:
    rC( masxfs_levinfo_db_close( li ) );
    QRLI( li, rCODE );
    break;
  }
  rRET;
}

int
masxfs_levinfo_close_all_up( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECL( 0 );

  do
  {
    rC( masxfs_levinfo_close( li, flags ) );
    QRLI( li, rCODE );
    /* test li->lidepth BEFORE li-- */
  } while ( !rCODE && li->lidepth && li-- );

  QRLI( li, rCODE );
  rRET;
}

static int
masxfs_levinfo_fs_stat( masxfs_levinfo_t * li )
{
  rDECL( 0 );

  if ( li )
  {
    if ( !li->fs.stat )
    {
      li->fs.stat = mas_calloc( 1, sizeof( masxfs_stat_t ) );

      if ( !masxfs_levinfo_fd_val( li, 0 ) && li->lidepth > 0 )
        rC( fstatat( masxfs_levinfo_fs_open( li - 1 ), li->name, li->fs.stat, AT_SYMLINK_NOFOLLOW ) );
      else
        rC( fstat( masxfs_levinfo_fs_open( li ), li->fs.stat ) );
      QRLI( li, rCODE );
      if ( !rCODE && li->fs.stat )
        li->detype = masxfs_levinfo_stat2entry( li->fs.stat );
    }
  }
  else
    rCODE = -1;
  QRLI( li, rCODE );
  rRET;
}

static int
masxfs_levinfo_db_stat( masxfs_levinfo_t * li _uUu_ )
{
  rDECL( 0 );
  DIE( "NOT IMPLEMENTED" );
  rCODE = -1;
  rRET;
}

int
masxfs_levinfo_stat( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECL( 0 );
  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  switch ( mode )
  {
  case MASXFS_SCAN__MODE_NONE:
    rCODE = -1;
    break;
  case MASXFS_SCAN__MODE_FS:
    rC( masxfs_levinfo_fs_stat( li ) );
    break;
  case MASXFS_SCAN__MODE_DB:
    rC( masxfs_levinfo_db_stat( li ) );
    break;
  }
  rRET;
}
