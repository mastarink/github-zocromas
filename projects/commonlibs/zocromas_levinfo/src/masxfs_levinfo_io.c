#include "masxfs_levinfo_defs.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
/* #include <sys/types.h> */
#include <sys/stat.h>
#include <fcntl.h>

#include <mastar/wrap/mas_memory.h>
/* #include <mastar/tools/mas_arg_tools.h> */
#include <mastar/minierr/minierr.h>

#include "masxfs_levinfo_structs.h"
/* #include "masxfs_structs.h" */

/* #include "masxfs_levinfo.h" */
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

static int
masxfs_levinfo_open_at( masxfs_levinfo_t * li, int fdparent )
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

int
masxfs_levinfo_open( masxfs_levinfo_t * li )
{
  int r = 0;

  if ( li->lidepth > 0 )
    r = masxfs_levinfo_open_at( li, masxfs_levinfo_open( li - 1 ) );
  else if ( !li->fd && li->name && !*li->name )
  {
    errno = 0;
    li->fd = open( "/", O_DIRECTORY | /* O_NOFOLLOW | */ O_RDONLY );
    if ( li->fd < 0 )
    {
      r = li->fd;
      li->fd = 0;
    }
  }
  if ( li->fd < 0 )
    r = -1;
  QRLI( li, r );
  return li->fd;
}

int
masxfs_levinfo_close( masxfs_levinfo_t * li )
{
  int r = 0;

  if ( li && li->fd )
  {
    errno = 0;
    r = close( li->fd );
    li->fd = 0;
    QRLI( li, r );
  }
/* else      */
/*   r = -1; */
  QRLI( li, r );
  return r;
}

int
masxfs_levinfo_close_all_up( masxfs_levinfo_t * li )
{
  int r = 0;

  do
  {
    r = masxfs_levinfo_close( li );
    QRLI( li, r );
    if ( !li->lidepth )
      break;
    li--;
  } while ( r >= 0 );

  QRLI( li, r );
  return r;
}

int
masxfs_levinfo_stat( masxfs_levinfo_t * li )
{
  int r = 0;

  if ( li )
  {
    if ( !li->stat )
    {
      li->stat = mas_calloc( 1, sizeof( masxfs_stat_t ) );

      if ( !masxfs_levinfo_fd_val( li, 0 ) && li->lidepth > 0 )
        r = fstatat( masxfs_levinfo_open( li - 1 ), li->name, li->stat, AT_SYMLINK_NOFOLLOW );
      else
        r = fstat( masxfs_levinfo_open( li ), li->stat );
      QRLI( li, r );
      if ( r >= 0 && li->stat )
        li->detype = masxfs_levinfo_stat2entry( li->stat );
    }
  }
  else
    r = -1;
  QRLI( li, r );
  return r;
}
