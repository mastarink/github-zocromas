#define R_GOOD(_r) ((_r)>=0)
#include "masxfs_levinfo_defs.h"
/* #include <stdio.h> */
#include <string.h>
#include <errno.h>

#include <unistd.h>
/* #include <sys/types.h> */
#include <sys/stat.h>
#include <fcntl.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_levinfo_structs.h"

#include "masxfs_levinfo_tools.h"

#include "masxfs_levinfo_fs.h"

/*
exiternal functions used:

 openat
 open
 close
 fstatat
 fstat

 */

int
masxfs_levinfo_fs_open_at( masxfs_levinfo_t * li, int fdparent )
{
  rDECLBAD;

  if ( li )
  {
    if ( li->fd )
      rCODE = li->fd;
    else if ( li && li->name )
    {
    /* TODO: O_NOFOLLOW :: If  pathname  is a  symbolic  link, then the open fails */
      int openflags = ( li->detype == MASXFS_ENTRY_DIR_NUM ? O_DIRECTORY : 0 ) /* | O_NOFOLLOW */  | O_RDONLY;

      errno = 0;
      rCODE = li->fd = openat( fdparent, li->name, openflags );
      if ( rCODE < 0 && errno == ENOENT && li->detype == MASXFS_ENTRY_LNK_NUM )
      {
      /* ignore dead symbolic link */
        rCODE = li->fd = 0;
      }
      else
      {
        QRLI( li, rCODE );
        if ( rCODE > 0 && li->detype == MASXFS_ENTRY_UNKNOWN_NUM && ( openflags & O_DIRECTORY ) )
          li->detype = MASXFS_ENTRY_DIR_NUM;
        if ( rCODE < 0 /* && li->detype == MASXFS_ENTRY_UNKNOWN_NUM */  )
          WARN( "NOT OPEN (%d) %s %d %d %d", fdparent, li->name, li->detype == MASXFS_ENTRY_DIR_NUM, li->detype == MASXFS_ENTRY_UNKNOWN_NUM,
                openflags & O_DIRECTORY ? 1 : 0 );
      }
    }
  }
  else
    QRLI( li, rCODE );
  rRET;
}

int
masxfs_levinfo_fs_open( masxfs_levinfo_t * li )
{
  rDECLBAD;
  if ( li )
  {
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
    {
      rSETBAD;
      QRLI( li, rCODE );
    }
    rCODE = li->fd;
  }
  else
    QRLI( li, rCODE );
  rRET;
}

int
masxfs_levinfo_fs_close( masxfs_levinfo_t * li )
{
  rDECLBAD;

  if ( li )
  {
    rSETGOOD;                                                        /* ! */
    if ( li->fd )
    {
      errno = 0;
      rCODE = close( li->fd );
      li->fd = 0;
      QRLI( li, rCODE );
    }
  }
  else
    QRLI( li, rCODE );
  rRET;
}

int
masxfs_levinfo_fs_stat( masxfs_levinfo_t * li, masxfs_stat_t ** pstat )
{
  rDECLBAD;

  if ( li )
  {
    rSETGOOD;
    if ( !li->fs.stat )
    {
      li->fs.stat = mas_calloc( 1, sizeof( masxfs_stat_t ) );

      if ( !li->fd /*masxfs_levinfo_fd_val( li, 0 ) */  && li->lidepth > 0 )
        rC( fstatat( masxfs_levinfo_fs_open( li - 1 ), li->name, li->fs.stat, AT_SYMLINK_NOFOLLOW ) );
      else
        rC( fstat( masxfs_levinfo_fs_open( li ), li->fs.stat ) );
      if ( rGOOD && li->fs.stat )
        li->detype = masxfs_levinfo_stat2entry( li->fs.stat );
      if ( pstat )
        *pstat = li->fs.stat;
    }
  }
  else
    QRLI( li, rCODE );
  rRET;
}
