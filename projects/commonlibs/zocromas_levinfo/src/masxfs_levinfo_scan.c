#include "masxfs_levinfo_defs.h"
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_levinfo_structs.h"
/* #include "masxfs_structs.h" */

#include "masxfs_levinfo_base.h"
#include "masxfs_levinfo_tools.h"
/* #include "masxfs_levinfo_io.h" */
#include "masxfs_levinfo_io_dir.h"
#include "masxfs_levinfo_ref.h"

#include "masxfs_levinfo_path.h"

#include "masxfs_levinfo_scan.h"



int
masxfs_levinfo_scan_li_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, void *data, unsigned long flags, masxfs_depth_t maxdepth )
{
  int r = 0, rc = 0;

  r = masxfs_levinfo_opendir( li );
  QRLI( li, r );
  if ( r >= 0 )
  {
    r = masxfs_levinfo_scan_dirn_cb( li, cb, data, flags, maxdepth );
    QRLI( li, r );
    rc = masxfs_levinfo_fs_closedir( li );
    if ( r >= 0 )
      r = rc;
    QRLI( li, r );
  }
  return r;
}

static int
masxfs_levinfo_scan_entry_single_internal_1cb( masxfs_levinfo_t * lithis, masxfs_entry_callback_t * cb, void *data, unsigned long tflags,
                                              masxfs_entry_type_t detype )
{
  int r = 0;
  masxfs_scan_fun_simple_t fun_simple = cb->fun_simple;
  masxfs_entry_type_bit_t entry_bit = 1 << detype;

  if ( !( tflags & MASXFS_CB_SKIP ) && ( cb->types & entry_bit ) && fun_simple )
  {
    if ( r >= 0 )
      r = fun_simple( lithis, tflags, data );
    QRLI( lithis, r );
  }
  return r;
}

static int
masxfs_levinfo_scan_entry_single_internal_cb( masxfs_levinfo_t * liparent, masxfs_levinfo_t * lithis, masxfs_entry_callback_t * cb, void *data,
                                             unsigned long flags, masxfs_entry_type_t detype )
{
  int r = 0;

  if ( liparent && lithis )
  {
    liparent->child_count_pair[!cb]++;

    for ( int ncb = 0; r >= 0 && cb && cb->fun_simple; cb++, ncb++ )
    {
      unsigned long tflags = 0;

      tflags = flags | cb->flags;
      r = masxfs_levinfo_scan_entry_single_internal_1cb( lithis, cb, data, tflags, detype );
      if ( !( tflags & MASXFS_CB_MULTIPLE_CBS ) )
        break;
    }
  }
  return r;
}

static int
masxfs_levinfo_scan_entry_single_at_child_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, void *data, unsigned long flags )
{
  return li ? masxfs_levinfo_scan_entry_single_internal_cb( li->lidepth > 0 ? li - 1 : NULL, li, cb, data, flags, li->detype ) : -1;
}

int
masxfs_levinfo_scan_down_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, void *data, unsigned long flags, masxfs_depth_t maxdepth )
{
  int r = 0;

  if ( li && li->fs.pde )
  {
    const char *name = li->fs.pde->d_name;
    int d_type = li->fs.pde->d_type;
    ino_t d_inode = li->fs.pde->d_ino;

    unsigned long tflags = flags;

    if ( cb )
      tflags |= cb->flags;

    masxfs_depth_t lidepth = li->lidepth;

    li++;
    lidepth++;
    {
      masxfs_levinfo_init( li, lidepth, name, masxfs_levinfo_de2entry( d_type ), d_inode );
      if ( li->lidepth == 0 )
        r = -1;
      QRLI( li, r );
      if ( r >= 0 )
      {
        r = masxfs_levinfo_scan_entry_single_at_child_cb( li, cb, data, flags );
      }
      QRLI( li, r );
      if ( d_type == DT_DIR )
      {
        if ( r >= 0 )
          r = masxfs_levinfo_scan_li_cb( li, cb, data, flags, maxdepth );
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
          r = masxfs_levinfo_scan_li_cb( li, cb, data, flags );
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
          r = masxfs_levinfo_scan_li_cb( li, cb, data, flags );
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
          r = masxfs_levinfo_scan_li_cb( li, cb, flags );
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
  }
  else
    r = -1;
  QRLI( li, r );
  return r;
}

static void
masxfs_levinfo_fix_type( masxfs_levinfo_t * li _uUu_ )
{
/* TODO - Don't remove next 20170217.123704 */
#if 0
  if ( li->fs.pde->d_type == DT_UNKNOWN )
  {
    struct stat st;

# if 1
    r = fstat(  .... );
# else
    fpath = masxfs_normalize_path( path, li->fs.pde->d_name );
    r = lstat( fpath, &st );
    if ( fpath )
      mas_free( fpath );
# endif
    if ( r >= 0 )
    {
      switch ( st.st_mode & S_IFMT )
      {
      case S_IFSOCK:
        li->fs.pde->d_type = DT_SOCK;
        break;
      case S_IFLNK:
        li->fs.pde->d_type = DT_LNK;
        break;
      case S_IFREG:
        li->fs.pde->d_type = DT_REG;
        break;
      case S_IFBLK:
        li->fs.pde->d_type = DT_BLK;
        break;
      case S_IFDIR:
        li->fs.pde->d_type = DT_DIR;
        break;
      case S_IFCHR:
        li->fs.pde->d_type = DT_CHR;
        break;
      case S_IFIFO:
        li->fs.pde->d_type = DT_FIFO;
        break;
      default:
        li->fs.pde->d_type = DT_UNKNOWN;
        break;
      }
    }
  }
#endif
}

static inline int
masxfs_levinfo_de_valid( masxfs_levinfo_t * li )
{
  return li && li->fs.pde && !( li->fs.pde->d_name[0] == '.' && ( ( li->fs.pde->d_name[1] == '.' && li->fs.pde->d_name[2] == 0 ) || li->fs.pde->d_name[1] == 0 ) );
}

int
masxfs_levinfo_scan_entry_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, void *data, unsigned long flags, masxfs_depth_t maxdepth )
{
  int r = 0;

  if ( li && li->fs.pde )
  {
    if ( masxfs_levinfo_de_valid( li ) )
    {
      unsigned long tflags = flags;

      if ( cb )
        tflags |= cb->flags;
      masxfs_levinfo_fix_type( li );
      if ( r >= 0 && ( tflags & MASXFS_CB_RECURSIVE ) && ( maxdepth == 0 || ( maxdepth > 0 && li->lidepth < maxdepth ) ) )
        r = masxfs_levinfo_scan_down_cb( li, cb, data, flags, maxdepth );
      QRLI( li, r );
    }
  }
  else
    r = -1;
  QRLI( li, r );
  return r;
}

static int
masxfs_levinfo_scan_dir_rest( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, void *data, unsigned long flags, masxfs_depth_t maxdepth )
{
  int r = 0;
  int n = 0;

  while ( r >= 0 && masxfs_levinfo_fs_readdir( li ) )
  {
    r = masxfs_levinfo_scan_entry_cb( li, cb, data, flags, maxdepth );
    QRLI( li, r );
    if ( r >= 0 )
      n++;
  }
  QRLI( li, r );
  r = r < 0 ? r : n;
  return r;
}

int
masxfs_levinfo_scan_dirn_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, void *data, unsigned long flags, masxfs_depth_t maxdepth )
{
  int r = 0;

  if ( li )
    memset( li->child_count_pair, 0, sizeof( li->child_count_pair ) );
  if ( r >= 0 )
    r = masxfs_levinfo_scan_dir_cb( li, NULL, data, flags, maxdepth );
  QRLI( li, r );
  if ( r >= 0 )
    r = masxfs_levinfo_scan_dir_cb( li, cb, data, flags, maxdepth );
  QRLI( li, r );
  return r;
}

int
masxfs_levinfo_scan_dir_cb( masxfs_levinfo_t * li, masxfs_entry_callback_t * cb, void *data, unsigned long flags, masxfs_depth_t maxdepth )
{
  int r = 0;

  masxfs_entry_type_t detype = masxfs_levinfo_detype( li );

  switch ( detype )
  {
  case MASXFS_ENTRY_DIR_NUM:
    r = masxfs_levinfo_fs_rewinddir( li );
    QRLI( li, r );
    if ( r >= 0 )
      r = masxfs_levinfo_scan_dir_rest( li, cb, data, flags, maxdepth );
    QRLI( li, r );
    break;
  case MASXFS_ENTRY_REG_NUM:
    r = masxfs_levinfo_scan_entry_single_at_child_cb( li, cb, data, flags );
    QRLI( li, r );
    break;
  default:
    WARN( "#-=-#-=-#-=-#-=-#-=-#-=-#-=-#-=-#-=-#-=-#-=-#-=-#-=-#  %s (%d:%d:%d)", masxfs_levinfo_detype_name( li ), li->detype, detype,
          MASXFS_ENTRY_DIR_NUM );
    break;
  }
  return r;
}

int
masxfs_levinfo_scan_depth( masxfs_levinfo_t * lia, masxfs_depth_t depth, masxfs_scan_fun_simple_t cb, void *udata _uUu_, unsigned long flags )
{
  int r = 0;

  if ( lia )
  {
    for ( masxfs_levinfo_t * li = lia; li < lia + depth; li++ )
    {
      if ( li->lidepth == ( size_t ) ( li - lia ) )
      {
        int r _uUu_ = 0;

        r = cb( li, flags, udata );
        if ( r )
          break;
      }
      else
      {
        DIE( "FATAL ERROR" );
      }
    }
  }
  return r;
}

