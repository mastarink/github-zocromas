#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_defs.h"

#include "masxfs_structs.h"

#include "masxfs_levinfo.h"
#include "masxfs_levinfo_io.h"

static int
masxfs_levinfo_open_at( masxfs_levinfo_t * li, int fdparent )
{
  int fd = -1;

  if ( li->fd )
    fd = li->fd;
  else if ( li && li->name )
  {
   /* TODO: O_NOFOLLOW :: If  pathname  is a  symbolic  link, then the open fails */
    int flags = ( li->detype == MASXFS_ENTRY_DIR_NUM || li->detype == MASXFS_ENTRY_UNKNOWN_NUM ? O_DIRECTORY : 0 ) /* | O_NOFOLLOW */  | O_RDONLY;

    if ( li->fd )
    {
      DIE( "ALREADY OPEN" );
    }
    errno = 0;
    fd = li->fd = openat( fdparent, li->name, flags );
    QRLI( li, fd );
    if ( fd > 0 && li->detype == MASXFS_ENTRY_UNKNOWN_NUM && ( flags & O_DIRECTORY ) )
    {
      li->detype = MASXFS_ENTRY_DIR_NUM;
    }
    if ( fd < 0 /* && li->detype == MASXFS_ENTRY_UNKNOWN_NUM */  )
    {
      DIE( "NOT OPEN (%d) %s %d %d %d", fdparent, li->name, li->detype == MASXFS_ENTRY_DIR_NUM, li->detype == MASXFS_ENTRY_UNKNOWN_NUM,
           flags & O_DIRECTORY ? 1 : 0 );
    }
  }
  return fd;
}

int
masxfs_levinfo_is_open( masxfs_levinfo_t * li )
{
  return li ? li->fd : 0;
}

int masxfs_levinfo_fd( masxfs_levinfo_t * li ) __attribute__ ( ( alias( "masxfs_levinfo_is_open" ) ) );

int
masxfs_levinfo_open( masxfs_levinfo_t * li )
{
  int r = 0;

  if ( li->lidepth > 0 )
    masxfs_levinfo_open_at( li, masxfs_levinfo_open( li - 1 ) );
  else if ( !li->fd && li->name && !*li->name )
  {
    errno = 0;
    li->fd = open( "/", O_DIRECTORY | /* O_NOFOLLOW | */ O_RDONLY );
  }
  if ( li->fd < 0 )
    r = -1;
  QRLI( li, r );
  return li->fd;
}

int
masxfs_levinfo_opendir( masxfs_levinfo_t * li )
{
  int r = 0;

  if ( li && !li->pdir )
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
        rewinddir( li->pdir );
      else
        r = -1;
      QRLI( li, r );
    }
    else
      r = -1;
    QRLI( li, r );
  }
  else
    r = -1;
  QRLI( li, r );
  return r;
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

  if ( li && li->pdir )
  {
    errno = 0;

  /* r=readdir_r(li->pdir, &li->de,...  ); No! */
    li->pde = de = readdir( li->pdir );
  /* li->de = *de; */
    if ( !de && errno )
      r = -1;
    QRLI( li, r );
  }
  else
    r = -1;
  QRLI( li, r );
  return de;
}

#if 0
int
masxfs_levinfo_have_more( masxfs_levinfo_t * li, masxfs_entry_type_t detype )
{
  int f = 0;

  if ( li )
  {
    long d = telldir( li->pdir );

    while ( masxfs_levinfo_readdir( li ) && ( masxfs_levinfo_de2entry( li->pde->d_type ) != detype ) );
    f = li && li->pde ? 1 : 0;
    seekdir( li->pdir, d );
    li->pde = NULL;
  }
  return f;
}
#endif

int
masxfs_levinfo_rewinddir( masxfs_levinfo_t * li )
{
  int r = 0;

  if ( li && li->pdir )
  {
    errno = 0;
    rewinddir( li->pdir );
    if ( errno )
      r = -1;
  }
  else
    r = -1;
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
      li->stat = mas_calloc( 1, sizeof( masxfs_stat_t ) );

    if ( !masxfs_levinfo_fd( li ) && li->lidepth > 0 )
      r = fstatat( masxfs_levinfo_open( li - 1 ), li->name, li->stat, AT_SYMLINK_NOFOLLOW );
    else
      r = fstat( masxfs_levinfo_open( li ), li->stat );
    QRLI( li, r );
  }
  else
    r = -1;
  QRLI( li, r );
  return r;
}
