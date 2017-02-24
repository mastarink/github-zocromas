#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_defs.h"

#include "masxfs_structs.h"

#include "masxfs_levinfo_base.h"
#include "masxfs_levinfo_io.h"
#include "masxfs_levinfo_format.h"
#include "masxfs_levinfo_path.h"
#include "masxfs_levinfo.h"

#include "masxfs_levinfo_ref.h"

struct stat *
masxfs_levinfo_stat_val( masxfs_levinfo_t * li )
{
  return li ? li->stat : NULL;
}

const struct stat *
masxfs_levinfo_stat_ref( masxfs_levinfo_t * li, unsigned long tflags )
{
  int r = 0;
  struct stat *st = NULL;

  if ( li && ( tflags & MASXFS_CB_STAT ) )
  {
    r = masxfs_levinfo_stat( li );
    if ( r >= 0 )
      st = masxfs_levinfo_stat_val( li );
  }
  return st;
}

size_t
masxfs_levinfo_size_val( masxfs_levinfo_t * li )
{
  size_t size = 0;

  if ( li->stat )
    size = li->stat->st_size;
  return size;
}

size_t
masxfs_levinfo_size_ref( masxfs_levinfo_t * li, unsigned long tflags )
{
  size_t size = 0;

  if ( li && ( tflags & MASXFS_CB_STAT ) )
  {
    int r = 0;

    r = masxfs_levinfo_stat( li );
    if ( r >= 0 )
      size = masxfs_levinfo_size_val( li );
  }
  return size;
}

int
masxfs_levinfo_fd_val( masxfs_levinfo_t * li )
{
  return li ? li->fd : 0;
}

int
masxfs_levinfo_fd_ref( masxfs_levinfo_t * li, unsigned long tflags )
{
  int fd = 0;

  if ( li && ( tflags & MASXFS_CB_FD ) )
  {
    int r = 0;

    r = masxfs_levinfo_open( li );
    if ( r >= 0 )
      fd = masxfs_levinfo_fd_val( li );
  }
  return fd;
}

int masxfs_levinfo_is_open( masxfs_levinfo_t * li ) __attribute__ ( ( alias( "masxfs_levinfo_fd_val" ) ) );

masxfs_depth_t
masxfs_levinfo_depth_ref( masxfs_levinfo_t * li, unsigned long tflags _uUu_ )
{
  return li ? li->lidepth : 0;
}

ino_t
masxfs_levinfo_deinode_ref( masxfs_levinfo_t * li, unsigned long tflags _uUu_ )
{
  return li ? li->deinode : 0;
}

const char *
masxfs_levinfo_name_ref( masxfs_levinfo_t * li, unsigned long tflags )
{
  const char *name = NULL;

  if ( li && ( tflags & MASXFS_CB_NAME ) )
  {
    name = li->name;
  }
  return name;
}

const char *
masxfs_levinfo_path_ref( masxfs_levinfo_t * li, unsigned long tflags )
{
  const char *path = NULL;

  if ( li && ( tflags & MASXFS_CB_PATH ) )
  {
    if ( li->path )
      mas_free( li->path );
    path = li->path = masxfs_levinfo_li2path_up( li, ( li->detype == MASXFS_ENTRY_DIR_NUM && ( tflags & MASXFS_CB_TRAILINGSLASH ) ) ? '/' : 0 );
  }
  return path;
}

const char *
masxfs_levinfo_prefix_ref( masxfs_levinfo_t * li, char *p1, char *p2, char *p3, char *p4, unsigned long tflags )
{
  const char *prefix = NULL;

  if ( li && ( tflags & MASXFS_CB_PREFIX ) )
  {
    if ( li->prefix )
      mas_free( li->prefix );
    prefix = li->prefix = masxfs_levinfo_prefix( li, p1, p2, p3, p4, 0 );
  }
  return prefix;
}
