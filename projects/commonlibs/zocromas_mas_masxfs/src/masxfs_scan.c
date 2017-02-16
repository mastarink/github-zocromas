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

#include "masxfs_structs.h"

#include "masxfs_pathinfo_base.h"
#include "masxfs_pathinfo.h"

#include "masxfs_scan.h"

#if 0
struct dirent
{
  ino_t d_ino;                                                       /* Inode number */
  off_t d_off;                                                       /* Not an offset; see below */
  unsigned short d_reclen;                                           /* Length of this record */
  unsigned char d_type;                                              /* Type of file; not supported
                                                                        by all filesystem types */
  char d_name[256];                                                  /* Null-terminated filename */
};

#endif
static masxfs_entry_type_t
masxfs_de2entry( int d_type )
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
masxfs_get_normalized_path( const char *path, const char *name, char **pnpath )
{
  char *npath = NULL;

  if ( pnpath )
    npath = *pnpath;
  if ( !npath )
  {
    npath = masxfs_normalize_path( path, name );
    if ( pnpath )
      *pnpath = npath;
  }
  return npath;
}

char *
masxfs_normalize_path( const char *path, const char *name )
{
  char *npath = NULL;

  if ( path )
  {
    size_t plen = 0, nlen = 0, len = 0;

    plen = strlen( path );
    if ( name )
      nlen = strlen( name );
    len = plen + nlen;
    if ( len > 0 )
    {
      npath = mas_malloc( len + 2 /* for 0 and possible '/'  */  );
      {
        const char *p = path;
        char *np = npath;

        while ( p && *p )
        {
          char c;

          *np++ = c = *p++;
          while ( c == '/' && *p == '/' )
            p++;
        }
        *np = 0;
        if ( np[-1] != '/' )
          *np++ = '/';
        *np = 0;
        if ( name )
          strcat( npath, name );
      }
    }
  }
  return npath;
}

int
masxfs_scanentry_cb( masxfs_dirent_t * de, const char *path, masxfs_entry_callback_t * cb, int recursive )
{
  int r = 0;
  const char *name = de->d_name;
  masxfs_scan_fun_simple_t fun_simple = cb->fun_simple;
  int d_type = de->d_type;
  char *fpath = NULL;

  if ( !( name[0] == '.' && ( ( name[1] == '.' && name[2] == 0 ) || name[1] == 0 ) ) )
  {
    if ( d_type == DT_UNKNOWN )
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
    }
    if ( !r )
    {
      masxfs_entry_type_bit_t entry_bit = 1 << masxfs_de2entry( d_type );

      if ( ( cb->types & entry_bit ) && fun_simple )
        fun_simple( path, name );
      if ( recursive && d_type == DT_DIR )
        r = masxfs_scanpath_cb( masxfs_get_normalized_path( path, name, &fpath ), cb, recursive );
    }
    if ( fpath )
      mas_free( fpath );
  }
  return r;
}

int
masxfs_scanentry( masxfs_dirent_t * de, const char *path, masxfs_entry_callback_t * callbacks, int recursive )
{
  int r = 0;

  for ( masxfs_entry_callback_t * cb = callbacks; cb && cb->fun_simple; cb++ )
  {
//  fprintf( stderr, "@@ %s/%s\n", path, de->d_name );
    r = masxfs_scanentry_cb( de, path, cb, recursive );
  }
  return r;
}

int
masxfs_scandir_cb( DIR * dir, const char *path, masxfs_entry_callback_t * cb, int recursive )
{
  int r = 0;
  masxfs_dirent_t *de;

  rewinddir( dir );
  while ( !r && ( de = readdir( dir ) ) )
    r = masxfs_scanentry_cb( de, path, cb, recursive );
  return r;
}

int
masxfs_scandir( DIR * dir, const char *path, masxfs_entry_callback_t * callbacks, int recursive )
{
  int r = 0;
  masxfs_dirent_t *de;

  rewinddir( dir );
  while ( !r && ( de = readdir( dir ) ) )
    r = masxfs_scanentry( de, path, callbacks, recursive );
  return r;
}

int
masxfs_scandir_r( DIR * dir, const char *path, masxfs_entry_callback_t * callbacks, int recursive )
{
  int r = 0;

  for ( masxfs_entry_callback_t * cb = callbacks; cb && cb->fun_simple; cb++ )
  {
    fprintf( stderr, "%p\n", cb );
    r = masxfs_scandir_cb( dir, path, cb, recursive );
  }
  return r;
}

int
masxfs_scanpath( const char *path, masxfs_entry_callback_t * callbacks, int recursive )
{
  int r = 0;

  if ( path )
  {
    DIR *dir = opendir( path );

    r = masxfs_scandir( dir, path, callbacks, recursive );
    closedir( dir );
  }
  else
    r = -1;
  return r;
}

int
masxfs_scanpath_cb( const char *path, masxfs_entry_callback_t * cb, int recursive )
{
  int r = 0;

  if ( path )
  {
    DIR *dir = opendir( path );

    r = masxfs_scandir_cb( dir, path, cb, recursive );
    closedir( dir );
  }
  else
    r = -1;
  return r;
}

int
masxfs_scanpath_real( const char *path, masxfs_entry_callback_t * callbacks, int recursive )
{
  int r = 0;

  if ( !path )
    path = ".";
  if ( path )
  {
    masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup( path, 128 );

#if 1
    r = masxfs_pathinfo_scan( pi, callbacks, recursive );
#else
    {
      char *real_path;

      real_path = masxfs_pathinfo_realpath( pi );
      r = masxfs_scanpath( real_path, callbacks, recursive );
      mas_free( real_path );
    }
#endif
    masxfs_pathinfo_delete( pi );
  }
  return r;
}
