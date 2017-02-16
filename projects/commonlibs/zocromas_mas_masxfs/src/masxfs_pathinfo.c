#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "masxfs_defs.h"
#include "masxfs_error.h"

#include "masxfs_structs.h"

#include "masxfs_levinfo_base.h"
#include "masxfs_levinfo.h"

#include "masxfs_pathinfo_base.h"
#include "masxfs_pathinfo.h"

char *
masxfs_pathinfo_realpath( masxfs_pathinfo_t * pi )
{
  return masxfs_levinfo_realpath( pi->levinfo, pi->depth );
}

masxfs_dir_t *
masxfs_pathinfo_opendir( masxfs_pathinfo_t * pi )
{
  masxfs_levinfo_t *li = pi->levinfo + pi->depth;
  char *real_path = masxfs_pathinfo_realpath( pi );

  li->dir = opendir( real_path );
  mas_free( real_path );
  return li->dir;
}

masxfs_dir_t *
masxfs_levinfo_opendir( masxfs_levinfo_t * li )
{
  char *real_path = masxfs_levinfo_right_realpath( li );

  li->dir = opendir( real_path );
  mas_free( real_path );
  return li->dir;
}

void
masxfs_pathinfo_closedir( masxfs_pathinfo_t * pi )
{
  masxfs_levinfo_t *li = pi->levinfo + pi->depth;

  closedir( li->dir );
}

void
masxfs_levinfo_closedir( masxfs_levinfo_t * li )
{
  closedir( li->dir );
}

static masxfs_entry_type_t
de2entry( int d_type )
{
  masxfs_entry_type_t r = 0;

  switch ( d_type )
  {
  case DT_BLK:
    r = MASXFS_ENTRY_BLK;
    break;
  case DT_CHR:
    r = MASXFS_ENTRY_CHR;
    break;
  case DT_DIR:
    r = MASXFS_ENTRY_DIR;
    break;
  case DT_FIFO:
    r = MASXFS_ENTRY_FIFO;
    break;
  case DT_LNK:
    r = MASXFS_ENTRY_LNK;
    break;
  case DT_REG:
    r = MASXFS_ENTRY_REG;
    break;
  case DT_SOCK:
    r = MASXFS_ENTRY_SOCK;
    break;
  case DT_UNKNOWN:
    r = MASXFS_ENTRY_UNKNOWN;
    break;
  }
  return r;
}

char *
masxfs_pathinfo_normal_path( masxfs_pathinfo_t * pi, const char *name )
{
  char *path = masxfs_pathinfo_realpath( pi );

  if ( name )
  {
    path = mas_strcat_x( path, "/" );
    path = mas_strcat_x( path, name );
  }
  return path;
}

int
masxfs_levinfo_scanentry_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, int recursive )
{
  int r = 0;
  const char *name = li->de->d_name;
  masxfs_scan_fun_simple_t fun_simple = cb->fun_simple;
  int d_type = li->de->d_type;
  char *fpath = NULL;

  if ( !( name[0] == '.' && ( ( name[1] == '.' && name[2] == 0 ) || name[1] == 0 ) ) )
  {
/*  if ( d_type == DT_UNKNOWN )
    {
      struct stat st;

      fpath = masxfs_normalize_path( path, name );
      d_type = DT_UNKNOWN;
      r = lstat( fpath, &st );
      if ( !r )
      {
        switch ( st.st_mode & S_IFMT )
        {
        case S_IFSOCK:
          d_type = DT_SOCK;
          break;
        case S_IFLNK:
          d_type = DT_LNK;
          break;
        case S_IFREG:
          d_type = DT_REG;
          break;
        case S_IFBLK:
          d_type = DT_BLK;
          break;
        case S_IFDIR:
          d_type = DT_DIR;
          break;
        case S_IFCHR:
          d_type = DT_CHR;
          break;
        case S_IFIFO:
          d_type = DT_FIFO;
          break;
        default:
          d_type = DT_UNKNOWN;
          break;
        }
      }
    } */
    if ( !r )
    {
      masxfs_entry_type_bit_t entry_bit = 1 << de2entry( d_type );

      if ( ( cb->types & entry_bit ) && fun_simple )
      {
      /* char *path = masxfs_pathinfo_realpath( li->pi ); */
        char *path = masxfs_levinfo_right_realpath( li );

        fun_simple( path, name );
        mas_free( path );
      }
      if ( recursive && d_type == DT_DIR )
      {
        masxfs_levinfo_init( li, li->de->d_name );
        li++;
        {
          char *path = masxfs_levinfo_right_realpath( li );

          r = masxfs_levinfo_scanpath_cb( li, cb, recursive );
          mas_free( path );
        }
        li--;
	masxfs_levinfo_close(li);
      }
    }
    if ( fpath )
      mas_free( fpath );
  }
  return r;
}

int
masxfs_levinfo_scandir_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, int recursive )
{
  int r = 0;
  int n = 0;

  rewinddir( li->dir );
  while ( !r && ( li->de = readdir( li->dir ) ) )
  {
    r = masxfs_levinfo_scanentry_cb( li, cb, recursive );
    n++;
  }
  return r;
}

int
masxfs_levinfo_scanpath_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, int recursive )
{
  int r = 0;

  masxfs_levinfo_opendir( li );

  r = masxfs_levinfo_scandir_cb( li, cb, recursive );
  masxfs_levinfo_closedir( li );
  return r;
}

int
masxfs_levinfo_scanentry( masxfs_levinfo_t * li, masxfs_entry_callback_t * callbacks, int recursive )
{
  int r = 0;

  for ( masxfs_entry_callback_t * cb = callbacks; cb && cb->fun_simple; cb++ )
  {
    r = masxfs_levinfo_scanentry_cb( li, cb, recursive );
  }
  return r;
}

int
masxfs_levinfo_scandir( masxfs_levinfo_t * li, masxfs_entry_callback_t * callbacks, int recursive )
{
  int r = 0;

  if ( li )
  {
    rewinddir( li->dir );
    while ( !r && ( li->de = readdir( li->dir ) ) )
    {
      r = masxfs_levinfo_scanentry( li, callbacks, recursive );
    }
  }
  else
    r = -1;
  return r;
}

int
masxfs_pathinfo_scan( masxfs_pathinfo_t * pi, masxfs_entry_callback_t * callbacks, int recursive )
{
  int r = 0;
  masxfs_levinfo_t *li = pi->levinfo + pi->depth;

  masxfs_pathinfo_opendir( pi );
  r = masxfs_levinfo_scandir( li, callbacks, recursive );
  masxfs_pathinfo_closedir( pi );

  return r;
}
