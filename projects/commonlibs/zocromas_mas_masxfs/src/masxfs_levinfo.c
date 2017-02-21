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

masxfs_entry_type_t
masxfs_levinfo_de2entry( int d_type )
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
masxfs_levinfo_scanli_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, unsigned long flags )
{
  int r = 0, rc = 0;

  r = masxfs_levinfo_opendir( li );
  if ( r >= 0 )
  {
    r = masxfs_levinfo_scandir_cb( li, cb, flags );
    QRLI( li, r );
    rc = masxfs_levinfo_closedir( li );
    if ( r >= 0 )
      r = rc;
    QRLI( li, r );
  }
  QRLI( li, r );
  return r;
}

static int
masxfs_levinfo_scanentry_single_internal_cb( masxfs_levinfo_t * li, masxfs_levinfo_t * lithis, masxfs_entry_callback_t * cb, unsigned long flags,
                                             masxfs_entry_type_t detype, const char *dename, ino_t deinode _uUu_, int fd, masxfs_stat_t * st )
{
  int r = 0;

  if ( li )
  {
    if ( cb )
    {
      li->child_count++;
    }
    else
    {
      li->child_count_z++;
    }
  }
  if ( cb )
  {
    masxfs_scan_fun_simple_t fun_simple = cb->fun_simple;
    masxfs_entry_type_bit_t entry_bit = 1 << detype;

    if ( ( cb->types & entry_bit ) && fun_simple )
    {
      char *name = NULL;
      char *path = NULL;
      unsigned long tflags = flags | cb->flags;

      if ( tflags & MASXFS_CB_PATH )
        path = masxfs_levinfo_li2path_up( li, (  /* detype == MASXFS_ENTRY_DIR_NUM && */ ( tflags & MASXFS_CB_TRAILINGSLASH ) ) ? '/' : 0 );
    /* printf("%d %d --- %s\n", detype, MASXFS_ENTRY_DIR_NUM, path); */
      if ( dename && ( tflags & MASXFS_CB_NAME ) )
        name = mas_strdup( dename );
      if ( lithis && ( tflags & MASXFS_CB_FD ) )
        fd = masxfs_levinfo_open( lithis );
      if ( fd < 0 )
        r = -1;
      QRLI( li, r );
      if ( r >= 0 && lithis && ( tflags & MASXFS_CB_STAT ) )
      {
        r = masxfs_levinfo_stat( lithis );
        QRLI( lithis, r );
        if ( r >= 0 )
          st = lithis->stat;
      }
      size_t depth = 0;

      if ( lithis )
        depth = lithis->lidepth;
      else
        depth = li->lidepth + 1;
      if ( r >= 0 )
        r = fun_simple( path, name, deinode, depth, li, fd, st, li ? li->child_count_z : 0, li ? li->child_count : 0 );
      QRLI( li, r );
      if ( path )
        mas_free( path );
      if ( name )
        mas_free( name );
    }
  }
  return r;
}

static int
masxfs_levinfo_scanentry_single_at_parent_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, unsigned long flags )
{
  return li ? masxfs_levinfo_scanentry_single_internal_cb( li, NULL, cb, flags, masxfs_levinfo_de2entry( li->pde->d_type ), li->pde->d_name,
                                                           li->pde->d_ino, 0 /* fd */ , NULL /* st */  ) : -1;
}

static int
masxfs_levinfo_scanentry_single_at_child_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, unsigned long flags )
{
  return li ? masxfs_levinfo_scanentry_single_internal_cb( li->lidepth > 0 ? li - 1 : NULL, li, cb, flags, li->detype, li->name, li->deinode,
                                                           0 /* fd */ , NULL /* st */  ) : -1;
}

int
masxfs_levinfo_scandown_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, unsigned long flags )
{
  int r = 0;

  const char *name = li->pde->d_name;
  int d_type = li->pde->d_type;
  ino_t d_inode = li->pde->d_ino;

  unsigned long tflags = flags;

  if ( cb )
    tflags |= cb->flags;

  li++;
  {
    masxfs_levinfo_init( li, name, masxfs_levinfo_de2entry( d_type ), d_inode );
    if ( r >= 0 && !( tflags & MASXFS_CB_AT_PARENT ) )
    {
      if ( 0 && !cb )
        fprintf( stderr, "CBZ %d\n", __LINE__ );
      r = masxfs_levinfo_scanentry_single_at_child_cb( li, cb, flags );
    }
    QRLI( li, r );
    if ( d_type == DT_DIR )
    {
      if ( r >= 0 )
        r = masxfs_levinfo_scanli_cb( li, cb, flags );
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
      if ( r >= 0 )
        r = masxfs_levinfo_scanli_cb( li, cb, flags );
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
      if ( r >= 0 )
        r = masxfs_levinfo_scanli_cb( li, cb, flags );
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
      if ( r >= 0 )
        r = masxfs_levinfo_scanli_cb( li, cb, flags );
#endif
      QRLI( li, r );
      {
        char *path = masxfs_levinfo_li2path_up( li, 0 );

        WARN( "#???### [%s]", path );
        mas_free( path );
      }
    }
    masxfs_levinfo_reset( li );
  }
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
    if ( r >= 0 )
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
masxfs_levinfo_scanentry_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, unsigned long flags )
{
  int r = 0;

  if ( li && li->pde )
  {
    if ( masxfs_levinfo_de_valid( li ) )
    {
      unsigned long tflags = flags;

      if ( cb )
        tflags |= cb->flags;
      masxfs_levinfo_fix_type( li );
      if ( r >= 0 && ( tflags & MASXFS_CB_AT_PARENT ) )              /* TODO: OBSOLETE */
        r = masxfs_levinfo_scanentry_single_at_parent_cb( li, cb, flags );
      QRLI( li, r );
      if ( r >= 0 && ( tflags & MASXFS_CB_RECURSIVE ) )
        r = masxfs_levinfo_scandown_cb( li, cb, flags );
      QRLI( li, r );
    }
  }
  else
    r = -1;
  QRLI( li, r );
  return r;
}

int
masxfs_levinfo_scanentry_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * callbacks, unsigned long flags )
{
  int r = 0;

  if ( li )
  {
    for ( masxfs_entry_callback_t * cb = callbacks; r >= 0 && cb && cb->fun_simple; cb++ )
    {
      r = masxfs_levinfo_scanentry_cb( li, cb, flags );
      QRLI( li, r );
    }
  }
  else
    r = -1;
  QRLI( li, r );
  return r;
}

static int
masxfs_levinfo_scandir_rest_with( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, unsigned long flags, masxfs_li_scanner_t scanner,
                                  masxfs_li_filter_t filter, masxfs_li_stopper_t stopper )
{
  int r = 0;
  int n = 0;

  while ( r >= 0 && masxfs_levinfo_readdir( li ) && ( !stopper || !stopper( li ) ) && ( !filter || !filter( li ) ) )
  {
    if ( scanner )
      r = scanner( li, cb, flags );
    if ( r >= 0 )
      n++;
  }
  QRLI( li, r );
  r = r < 0 ? r : n;
  return r;
}

static int
masxfs_levinfo_scandir_all_with( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, unsigned long flags, masxfs_li_scanner_t scanner,
                                 masxfs_li_filter_t filter, masxfs_li_stopper_t stopper )
{
  int r = 0;

  r = masxfs_levinfo_rewinddir( li );
  QRLI( li, r );
  if ( r >= 0 )
    r = masxfs_levinfo_scandir_rest_with( li, cb, flags, scanner, filter, stopper );
  QRLI( li, r );
  return r;
}

int
masxfs_levinfo_scandir_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, unsigned long flags )
{
  int r = 0;

  if ( li )
  {
    li->child_count_z = 0;
    li->child_count = 0;
  }
  r = masxfs_levinfo_scandir_all_with( li, NULL, flags, masxfs_levinfo_scanentry_cb, NULL, NULL );
  QRLI( li, r );
  if ( r >= 0 )
    r = masxfs_levinfo_scandir_all_with( li, cb, flags, masxfs_levinfo_scanentry_cb, NULL, NULL );
  QRLI( li, r );
  return r;
}

int
masxfs_levinfo_scandir_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * callbacks, unsigned long flags )
{
  int r = 0;

  r = masxfs_levinfo_scandir_all_with( li, NULL, flags, masxfs_levinfo_scanentry_cb, NULL, NULL );
  QRLI( li, r );
  if ( r >= 0 )
    r = masxfs_levinfo_scandir_all_with( li, callbacks, flags, masxfs_levinfo_scanentry_cbs, NULL, NULL );
  QRLI( li, r );
  return r;
}

char *
masxfs_levinfo_prefix( masxfs_levinfo_t * li _uUu_, char *p1 _uUu_, char *p2 _uUu_, char *p3 _uUu_ )
{
  return NULL;
}
