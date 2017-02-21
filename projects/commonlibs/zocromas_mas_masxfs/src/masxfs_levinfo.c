#include <string.h>
#include <unistd.h>

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

masxfs_levinfo_t *masxfs_levinfo_li2lia( masxfs_levinfo_t * li ) __attribute__ ( ( alias( "masxfs_levinfo_root" ) ) );

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
    QRLI( li, r );
    rc = masxfs_levinfo_closedir( li );
    if ( !r )
      r = rc;
    QRLI( li, r );
  }
  QRLI( li, r );
  return r;
}

static int
masxfs_levinfo_scanentry_single_internal_cb( masxfs_levinfo_t * li, masxfs_levinfo_t * lithis, masxfs_entry_callback_t * cb,
                                             masxfs_entry_type_t detype, const char *dename, int fd, masxfs_stat_t * st )
{
  int r = 0;

  if ( cb )
  {
    masxfs_scan_fun_simple_t fun_simple = cb->fun_simple;

/*  masxfs_entry_type_bit_t entry_bit = 1 << detype; */

    if (  /* ( cb->types & entry_bit ) && */ fun_simple )
    {
      char *name = NULL;
      char *path = NULL;

      if ( cb->flags & MASXFS_CB_PATH )
        path = masxfs_levinfo_li2path_up( li, ( detype == MASXFS_ENTRY_DIR_NUM && ( cb->flags & MASXFS_CB_TRAILINGSLASH ) ) ? '/' : 0 );
      if ( dename && ( cb->flags & MASXFS_CB_NAME ) )
        name = mas_strdup( dename );
      if ( lithis && ( cb->flags & MASXFS_CB_FD ) )
        fd = masxfs_levinfo_open( lithis );
      if ( fd < 0 )
        r = -1;
      QRLI( li, r );
      if ( !r && lithis && ( cb->flags & MASXFS_CB_STAT ) )
      {
        r = masxfs_levinfo_stat( li );
        QRLI( li, r );
        if ( !r )
          st = li->stat;
      }
      if ( !r )
        r = fun_simple( path, name, fd, st );
      QRLI( li, r );
      if ( path )
        mas_free( path );
      if ( name )
        mas_free( name );
    }
  }
  else
    r = -1;
  QRLI( li, r );
  return r;
}

static int
masxfs_levinfo_scanentry_single_at_parent_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb )
{
  return masxfs_levinfo_scanentry_single_internal_cb( li, NULL, cb, de2entry( li->pde->d_type ), li->pde->d_name, 0, NULL /* st */  );
}

static int
masxfs_levinfo_scanentry_single_at_child_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb )
{
  return masxfs_levinfo_scanentry_single_internal_cb( li, li, cb, li->detype, NULL, 0, NULL /* st */  );
}

int
masxfs_levinfo_scandown_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, int recursive )
{
  int r = 0;

  const char *name = li->pde->d_name;
  int d_type = li->pde->d_type;

  li++;
  masxfs_levinfo_init( li, name, de2entry( d_type ) );
  if ( !r && cb && ( cb->flags & MASXFS_CB_AT_CHILD ) )
    r = masxfs_levinfo_scanentry_single_at_child_cb( li, cb );
  QRLI( li, r );
  if ( d_type == DT_DIR )
  {
    if ( !r )
      r = masxfs_levinfo_scanli_cb( li, cb, recursive );
    QRLI( li, r );
    {
      char *path = masxfs_levinfo_li2path_up( li, 0 );

    /* WARN( "#DIR### [%s]", path ); */
      mas_free( path );
    }
  }
  else if ( d_type == DT_REG )
  {
#if 0
    if ( !r )
      r = masxfs_levinfo_scanli_cb( li, cb, recursive );
#endif
    QRLI( li, r );
    {
      char *path = masxfs_levinfo_li2path_up( li, 0 );

    /* WARN( "#REG### [%s]", path ); */
      mas_free( path );
    }
  }
  else if ( d_type == DT_LNK )
  {
#if 0
    if ( !r )
      r = masxfs_levinfo_scanli_cb( li, cb, recursive );
#endif
    QRLI( li, r );
    {
      char *path = masxfs_levinfo_li2path_up( li, 0 );

    /* WARN( "#LNK### [%s]", path ); */
      mas_free( path );
    }
  }
  else
  {
#if 0
    if ( !r )
      r = masxfs_levinfo_scanli_cb( li, cb, recursive );
#endif
    QRLI( li, r );
    {
      char *path = masxfs_levinfo_li2path_up( li, 0 );

      WARN( "#???### [%s]", path );
      mas_free( path );
    }
  }
  masxfs_levinfo_reset( li );
  li--;
  return r;
}

static void
masxfs_levinfo_fix_type( masxfs_levinfo_t * li _uUu_ )
{
/* TODO - Don't remove next 20170217.123704 */
#if 0
  if ( li->pde->d_type == DT_UNKNOWN )
  {
    struct stat st;

# if 1
    r = fstat(  .... );
# else
    fpath = masxfs_normalize_path( path, li->pde->d_name );
    r = lstat( fpath, &st );
    if ( fpath )
      mas_free( fpath );
# endif
    if ( !r )
    {
      switch ( st.st_mode & S_IFMT )
      {
      case S_IFSOCK:
        li->pde->d_type = DT_SOCK;
        break;
      case S_IFLNK:
        li->pde->d_type = DT_LNK;
        break;
      case S_IFREG:
        li->pde->d_type = DT_REG;
        break;
      case S_IFBLK:
        li->pde->d_type = DT_BLK;
        break;
      case S_IFDIR:
        li->pde->d_type = DT_DIR;
        break;
      case S_IFCHR:
        li->pde->d_type = DT_CHR;
        break;
      case S_IFIFO:
        li->pde->d_type = DT_FIFO;
        break;
      default:
        li->pde->d_type = DT_UNKNOWN;
        break;
      }
    }
  }
#endif
}

static inline int
masxfs_levinfo_de_valid( masxfs_levinfo_t * li )
{
  return li && li->pde && !( li->pde->d_name[0] == '.' && ( ( li->pde->d_name[1] == '.' && li->pde->d_name[2] == 0 ) || li->pde->d_name[1] == 0 ) );
}

int
masxfs_levinfo_scanentry_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, int recursive )
{
  int r = 0;

  if ( li && li->pde )
  {
    if ( masxfs_levinfo_de_valid( li ) )
    {
      masxfs_levinfo_fix_type( li );
      if ( !r && cb && ( cb->flags & MASXFS_CB_AT_PARENT ) )
        r = masxfs_levinfo_scanentry_single_at_parent_cb( li, cb );
      QRLI( li, r );
      if ( !r && recursive )
        r = masxfs_levinfo_scandown_cb( li, cb, recursive || ( cb && ( cb->flags & MASXFS_CB_RECURSIVE ) ) );
      QRLI( li, r );
    }
  }
  else
    r = -1;
  QRLI( li, r );
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
      QRLI( li, r );
    }
  }
  else
    r = -1;
  QRLI( li, r );
  return r;
}

static int
masxfs_levinfo_scandir_rest_with( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, int recursive, masxfs_li_scanner_t scanner,
                                  masxfs_li_filter_t filter, masxfs_li_stopper_t stopper )
{
  int r = 0;

  while ( !r && masxfs_levinfo_readdir( li ) && ( !stopper || !stopper( li ) ) )
  {
    masxfs_entry_type_bit_t entry_bit = 1 << de2entry( li->pde->d_type );

    if ( cb && ( cb->types & entry_bit ) && ( !filter || !filter( li ) ) )
      r = scanner( li, cb, recursive );
  }
  QRLI( li, r );
  return r;
}

static int
masxfs_levinfo_scandir_all_with( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, int recursive, masxfs_li_scanner_t scanner,
                                 masxfs_li_filter_t filter, masxfs_li_stopper_t stopper )
{
  int r = 0;

  r = masxfs_levinfo_rewinddir( li );
  QRLI( li, r );
  if ( !r )
    r = masxfs_levinfo_scandir_rest_with( li, cb, recursive, scanner, filter, stopper );
  QRLI( li, r );
  return r;
}

int
masxfs_levinfo_scandir_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, int recursive )
{
  return masxfs_levinfo_scandir_all_with( li, cb, recursive, masxfs_levinfo_scanentry_cb, NULL, NULL );
}

int
masxfs_levinfo_scandir_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * callbacks, int recursive )
{
  return masxfs_levinfo_scandir_all_with( li, callbacks, recursive, masxfs_levinfo_scanentry_cbs, NULL, NULL );
}
