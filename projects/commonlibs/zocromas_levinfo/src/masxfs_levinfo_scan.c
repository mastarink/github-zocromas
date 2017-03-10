#include "masxfs_levinfo_defs.h"
#include <string.h>
#include <unistd.h>
/* #include <stdlib.h> */

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
masxfs_levinfo_scan_li_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * cbs, void *data, masxfs_levinfo_flags_t flags, masxfs_depth_t maxdepth,
                            masxfs_depth_t reldepth )
{
  rDECL( 0 );
  int rc = 0;

  if ( ( flags & MASXFS_CB_MODE_DB ) )
    WARN( "BEFORE OPEN DIR %d", ( flags & MASXFS_CB_MODE_DB ) ? 1 : 0 );

  rC( masxfs_levinfo_opendir( li, flags ) );
  QRLI( li, rCODE );
  if ( !rCODE )
  {
    rC( masxfs_levinfo_scan_dirn_cbs( li, cbs, data, flags, maxdepth, reldepth ) );
    QRLI( li, rCODE );
    rc = masxfs_levinfo_closedir( li, flags );
    if ( !rCODE )
      rCODE = rc;
    QRLI( li, rCODE );
  }
  rRET;
}

static int
masxfs_levinfo_scan_entry_single_internal_1cb( masxfs_levinfo_t * lithis, masxfs_entry_callback_t * cb, void *data, masxfs_levinfo_flags_t tflags,
                                               masxfs_entry_type_t detype, masxfs_depth_t reldepth )
{
  rDECL( 0 );

  if ( ( tflags & MASXFS_CB_MODE_DB ) )
    WARN( "POINT DB" );

  masxfs_scan_fun_simple_t fun_simple = cb->fun_simple;
  masxfs_entry_type_bit_t entry_bit = 1 << detype;

  if ( !( tflags & MASXFS_CB_SKIP ) && ( cb->types & entry_bit ) && fun_simple )
  {
    rC( fun_simple( lithis, tflags, data, reldepth ) );
    QRLI( lithis, rCODE );
  }
  rRET;
}

static int
masxfs_levinfo_scan_entry_single_internal_cbs( masxfs_levinfo_t * liparent, masxfs_levinfo_t * lithis, masxfs_entry_callback_t * cbs, void *data,
                                               masxfs_levinfo_flags_t flags, masxfs_entry_type_t detype, masxfs_depth_t reldepth )
{
  rDECL( 0 );

  if ( ( flags & MASXFS_CB_MODE_DB ) )
    WARN( "POINT DB" );

  if ( liparent && lithis )
  {
    masxfs_entry_callback_t *cb = cbs;

    liparent->child_count_pair[!cb]++;

    if ( ( flags & MASXFS_CB_MODE_DB ) )
      WARN( "POINT DB" );
    for ( int ncb = 0; !rCODE && cb && cb->fun_simple; cb++, ncb++ )
    {
      masxfs_levinfo_flags_t tflags = 0;

      tflags = flags | cb->flags;
      if ( ( tflags & MASXFS_CB_MODE_DB ) )
        WARN( "POINT DB" );

      if ( ( tflags & MASXFS_CB_FROM_ROOT ) && reldepth == 1 )
      {
        masxfs_levinfo_t *lia _uUu_ = lithis - lithis->lidepth;

        if ( ( tflags & MASXFS_CB_MODE_DB ) )
          WARN( "POINT DB" );

        for ( masxfs_levinfo_t * li = lia; !rCODE && li < lithis; li++ )
        {
          if ( ( tflags & MASXFS_CB_MODE_DB ) )
            WARN( "POINT DB" );

          if ( li->lidepth == ( masxfs_depth_t ) ( li - lia ) )
          {
            if ( ( flags & MASXFS_CB_MODE_DB ) )
              WARN( "POINT DB" );
            rC( masxfs_levinfo_scan_entry_single_internal_1cb( li, cb, data, tflags & ~MASXFS_CB_FROM_ROOT, MASXFS_ENTRY_DIR_NUM, li - lithis ) );
            if ( ( flags & MASXFS_CB_MODE_DB ) )
              WARN( "POINT DB" );
          }
          else
          {
            DIE( "WHY %ld != %ld", ( long ) li->lidepth, ( long ) ( li - lia ) );
          }
        }
      }
      if ( ( flags & MASXFS_CB_MODE_DB ) )
        WARN( "POINT DB" );
      rC( masxfs_levinfo_scan_entry_single_internal_1cb( lithis, cb, data, tflags & ~MASXFS_CB_FROM_ROOT, detype ? detype : lithis->detype,
                                                         reldepth ) );
      if ( ( flags & MASXFS_CB_MODE_DB ) )
        WARN( "POINT DB" );
      if ( !( tflags & MASXFS_CB_MULTIPLE_CBS ) )
        break;
    }
  }
  else
  {
    if ( ( flags & MASXFS_CB_MODE_DB ) )
      WARN( "POINT DB" );
  }
  if ( ( flags & MASXFS_CB_MODE_DB ) )
    WARN( "POINT DB" );
  rRET;
}

static int
masxfs_levinfo_scan_entry_single_at_child_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * cbs, void *data, masxfs_levinfo_flags_t flags,
                                               masxfs_depth_t reldepth )
{
  if ( ( flags & MASXFS_CB_MODE_DB ) )
    WARN( "POINT DB" );

  return li ? masxfs_levinfo_scan_entry_single_internal_cbs( li->lidepth > 0 ? li - 1 : NULL, li, cbs, data, flags, li->detype, reldepth ) : -1;
}

static int
masxfs_levinfo_scan_down_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * cbs, void *data, masxfs_levinfo_flags_t flags,
                              masxfs_depth_t maxdepth, masxfs_depth_t reldepth )
{
  int r = 0;

  if ( ( flags & MASXFS_CB_MODE_DB ) )
    WARN( "POINT DB" );

  if ( li )
  {
  /* masxfs_dirent_t *pde = mode == MASXFS_SCAN__MODE_FS ? li->fs.pde : li->db.pde; */
    masxfs_dirent_t *pde = flags & MASXFS_CB_MODE_FS ? li->fs.pde : li->db.pde;

    if ( pde )
    {
      const char *name = pde->d_name;
      int d_type = pde->d_type;
      ino_t d_inode = pde->d_ino;

      masxfs_depth_t lidepth = li->lidepth;

      li++;
      lidepth++;
      reldepth++;
      {
        masxfs_levinfo_init( li, lidepth, name, masxfs_levinfo_de2entry( d_type ), d_inode );
        if ( li->lidepth == 0 )
          r = -1;
        QRLI( li, r );
        if ( r >= 0 )
        {
          r = masxfs_levinfo_scan_entry_single_at_child_cbs( li, cbs, data, flags, reldepth );
        }
        QRLI( li, r );
        if ( d_type == DT_DIR )
        {
          if ( r >= 0 )
            r = masxfs_levinfo_scan_li_cbs( li, cbs, data, flags, maxdepth, reldepth );
          QRLI( li, r );
          {
            char *path = masxfs_levinfo_li2path_up( li, 0 );

          /* WARN( "#DIR### [%s]", path ); */
            mas_free( path );
          }
        }
        else if ( d_type == DT_REG )
        {
          QRLI( li, r );
          {
            char *path = masxfs_levinfo_li2path_up( li, 0 );

          /* WARN( "#REG### [%s]", path ); */
            mas_free( path );
          }
        }
        else if ( d_type == DT_LNK )
        {
          QRLI( li, r );
          {
            char *path = masxfs_levinfo_li2path_up( li, 0 );

          /* WARN( "#LNK### [%s]", path ); */
            mas_free( path );
          }
        }
        else
        {
          QRLI( li, r );
          {
            char *path = masxfs_levinfo_li2path_up( li, 0 );

            WARN( "#???### [%s]", path );
            mas_free( path );
          }
        }
        masxfs_levinfo_reset( li, flags );
      }
      reldepth--;
      lidepth--;
      li--;
    }
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

static int
masxfs_levinfo_de_valid( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  int r = 0;

  if ( li )
  {
    masxfs_dirent_t *pde = ( flags & MASXFS_CB_MODE_FS ? li->fs.pde : li->db.pde );

    r = pde && !( pde->d_name[0] == '.' && ( ( pde->d_name[1] == '.' && pde->d_name[2] == 0 ) || pde->d_name[1] == 0 ) );
  }
  return r;
}

static int
masxfs_levinfo_scan_entry_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * cbs, void *data, masxfs_levinfo_flags_t flags,
                               masxfs_depth_t maxdepth, masxfs_depth_t reldepth )
{
  int r = 0;

  if ( ( flags & MASXFS_CB_MODE_DB ) )
    WARN( "POINT DB" );

  if ( li )
  {
    masxfs_levinfo_flags_t tflags = flags;

    if ( masxfs_levinfo_de_valid( li, tflags ) )
    {
      masxfs_levinfo_fix_type( li );
      if ( r >= 0 && ( tflags & MASXFS_CB_RECURSIVE ) && ( maxdepth == 0 || ( maxdepth > 0 && li->lidepth < maxdepth ) ) )
        r = masxfs_levinfo_scan_down_cbs( li, cbs, data, tflags, maxdepth, reldepth );
      QRLI( li, r );
    }
  }
  else
    r = -1;
  QRLI( li, r );
  return r;
}

/* 
 * bad:  r< 0
 * good: r>=0 */
static int
masxfs_levinfo_scan_dir_rest_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * cbs, void *data, masxfs_levinfo_flags_t flags,
                                  masxfs_depth_t maxdepth, masxfs_depth_t reldepth )
{
  rDECL( 0 );
  int n = 0;

  if ( ( flags & MASXFS_CB_MODE_DB ) )
    WARN( "POINT DB" );

  while ( rCODE >= 0 && masxfs_levinfo_readdir( li, flags ) )
  {
    rC( masxfs_levinfo_scan_entry_cbs( li, cbs, data, flags, maxdepth, reldepth ) );
    QRLI( li, rCODE );
    if ( !rCODE )
      n++;
  }
  QRLI( li, rCODE );
  rCODE = rCODE < 0 ? rCODE : n;
  return rCODE;
}

int
masxfs_levinfo_scan_dir_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * cbs, void *data, masxfs_levinfo_flags_t flags, masxfs_depth_t maxdepth,
                             masxfs_depth_t reldepth )
{
  rDECL( -1 );

  masxfs_entry_type_t detype = masxfs_levinfo_detype( li, flags );

  if ( ( flags & MASXFS_CB_MODE_DB ) )
    WARN( "POINT DB" );

  switch ( detype )
  {
  case MASXFS_ENTRY_DIR_NUM:
    rC( masxfs_levinfo_rewinddir( li, flags ) );
    QRLI( li, rCODE );
    rC( masxfs_levinfo_scan_dir_rest_cbs( li, cbs, data, flags, maxdepth, reldepth ) );
    QRLI( li, rCODE );
    break;
  case MASXFS_ENTRY_REG_NUM:
    rC( masxfs_levinfo_scan_entry_single_at_child_cbs( li, cbs, data, flags, reldepth ) );
    QRLI( li, rCODE );
    break;
  default:
    WARN( "#-=-#-=-#-=-#-=-#-=-#-=-#-=-#-=-#-=-#-=-#-=-#-=-#-=-#  %s (%d:%d:%d)", masxfs_levinfo_detype_name( li ), li->detype, detype,
          MASXFS_ENTRY_DIR_NUM );
    break;
  }
  rRET;
}

int
masxfs_levinfo_scan_dirn_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * cbs, void *data, masxfs_levinfo_flags_t flags,
                              masxfs_depth_t maxdepth, masxfs_depth_t reldepth )
{
  rDECL( -1 );

  if ( ( flags & MASXFS_CB_MODE_DB ) )
    WARN( "POINT DB" );

  if ( li )
    memset( li->child_count_pair, 0, sizeof( li->child_count_pair ) );
  rC( masxfs_levinfo_scan_dir_cbs( li, NULL, data, flags, maxdepth, reldepth ) );
  QRLI( li, rCODE );
  rC( masxfs_levinfo_scan_dir_cbs( li, cbs, data, flags, maxdepth, reldepth ) );
  QRLI( li, rCODE );
  rRET;
}

int
masxfs_levinfo_scan_depth_cbf( masxfs_levinfo_t * lia, masxfs_depth_t depth, masxfs_scan_fun_simple_t cbf, void *udata _uUu_,
                               masxfs_levinfo_flags_t flags )
{
  rDECL( -1 );

  if ( lia )
  {
#if 1
    for ( masxfs_depth_t d = 0; !rCODE && d < depth; d++ )
    {
      if ( lia[d].lidepth == d )
      {
        WARN( ">>o FROM ROOT '%s' (rel:%ld)", lia[d].name, ( long ) d - depth );
        rC( cbf( &lia[d], flags, udata, d - depth ) );
      }
      else
      {
        DIE( "FATAL ERROR" );
        rCODE = -1;
      }
    }
#else
    for ( masxfs_levinfo_t * li = lia; li < lia + depth; li++ )
    {
      if ( li->lidepth == ( masxfs_depth_t ) ( li - lia ) )
      {
        WARN( "FROM ROOT '%s' (rel:%ld)  -  %ld ? %ld", li->name, -depth, -li->lidepth, ( li - lia + depth ) );
        rC( cbf( li, flags, udata, li - lia + depth ) );
        if ( rCODE )
          break;
      }
      else
      {
        DIE( "FATAL ERROR" );
      }
    }
#endif
  }
  rRET;
}
