#include <string.h>

#include <mastar/wrap/mas_memory.h>

#include "masxfs_defs.h"
#include "masxfs_error.h"

#include "masxfs_structs.h"

#include "masxfs_levinfo_base.h"
#include "masxfs_levinfo_io.h"

#include "masxfs_levinfo_path.h"

#include "masxfs_levinfo.h"

masxfs_levinfo_t *
masxfs_levinfo_root( masxfs_levinfo_t * li )
{
  return li ? li - li->lidepth : NULL;
}

masxfs_levinfo_t *
masxfs_levinfo_li2lia( masxfs_levinfo_t * li )
{
  return masxfs_levinfo_root( li );
}

static masxfs_entry_type_t
de2entry( int d_type )
{
  masxfs_entry_type_t c = 0;

  switch ( d_type )
  {
  case DT_BLK:
    c = MASXFS_ENTRY_BLK_NUM;
    break;
  case DT_CHR:
    c = MASXFS_ENTRY_CHR_NUM;
    break;
  case DT_DIR:
    c = MASXFS_ENTRY_DIR_NUM;
    break;
  case DT_FIFO:
    c = MASXFS_ENTRY_FIFO_NUM;
    break;
  case DT_LNK:
    c = MASXFS_ENTRY_LNK_NUM;
    break;
  case DT_REG:
    c = MASXFS_ENTRY_REG_NUM;
    break;
  case DT_SOCK:
    c = MASXFS_ENTRY_SOCK_NUM;
    break;
  case DT_UNKNOWN:
    c = MASXFS_ENTRY_UNKNOWN_NUM;
    break;
  }
  return c;
}

static int
masxfs_levinfo_scanli_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, int recursive )
{
  int r = 0, rc = 0;

  r = masxfs_levinfo_opendir( li );
  if ( !r )
  {
    r = masxfs_levinfo_scandir_cb( li, cb, recursive );
    QRLI( li,  r );
    rc = masxfs_levinfo_closedir( li );
    if ( !r )
      r = rc;
    QRLI( li,  r );
  }
  QRLI( li,  r );
  return r;
}

int
masxfs_levinfo_scanentry_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, int recursive )
{
  int r = 0;

  if ( li && li->pde )
  {
    char *name = mas_strdup( li->pde->d_name );
    int d_type = li->pde->d_type;

  /* char *fpath = NULL; */

    if ( !( name[0] == '.' && ( ( name[1] == '.' && name[2] == 0 ) || name[1] == 0 ) ) )
    {
    /* TODO - Don't remove next 20170217.123704 */
#if 0
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
#endif
      if ( !r && cb )
      {
        masxfs_scan_fun_simple_t fun_simple = cb->fun_simple;
        masxfs_entry_type_bit_t entry_bit = 1 << de2entry( d_type );

        if ( ( cb->types & entry_bit ) && fun_simple )
        {
          char *path = masxfs_levinfo_li2path_up( li );

          r = fun_simple( path, name );
          QRLI( li,  r );
          mas_free( path );
        }
        if ( !r && recursive && d_type == DT_DIR )
        {
          li++;
          masxfs_levinfo_init( li, name );
          r = masxfs_levinfo_scanli_cb( li, cb, recursive );
          QRLI( li,  r );
          masxfs_levinfo_reset( li );
          li--;
        }
      }
    /* if ( fpath )         */
    /*   mas_free( fpath ); */
    }
    mas_free( name );
  }
  else
    r = -1;
  QRLI( li,  r );
  return r;
}

int
masxfs_levinfo_scanentry_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * callbacks, int recursive )
{
  int r = 0;

  if ( li )
  {
    for ( masxfs_entry_callback_t * cb = callbacks; !r && cb && cb->fun_simple; cb++ )
    {
      r = masxfs_levinfo_scanentry_cb( li, cb, recursive );
      QRLI( li,  r );
    }
  }
  else
    r = -1;
  QRLI( li,  r );
  return r;
}

int
masxfs_levinfo_scandir_with( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, int recursive, masxfs_li_scanner_t scanner )
{
  int r = 0;

  r = masxfs_levinfo_rewinddir( li );
  if ( !r )
  {
    while ( !r && masxfs_levinfo_readdir( li ) )
    {
      r = scanner( li, cb, recursive );
      QRLI( li,  r );
    }
  }
  else
    r = -1;
  QRLI( li,  r );
  return r;
}

int
masxfs_levinfo_scandir_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, int recursive )
{
  return masxfs_levinfo_scandir_with( li, cb, recursive, masxfs_levinfo_scanentry_cb );
}

int
masxfs_levinfo_scandir_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * callbacks, int recursive )
{
  return masxfs_levinfo_scandir_with( li, callbacks, recursive, masxfs_levinfo_scanentry_cbs );
}
