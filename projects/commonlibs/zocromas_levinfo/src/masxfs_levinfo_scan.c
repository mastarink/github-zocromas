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
  if ( rGOOD )
  {
    rC( masxfs_levinfo_scan_dirn_cbs( li, cbs, data, flags, maxdepth, reldepth ) );
    QRLI( li, rCODE );
    rc = masxfs_levinfo_closedir( li, flags );
    if ( rGOOD )
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
    for ( int ncb = 0; rGOOD && cb && cb->fun_simple; cb++, ncb++ )
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

        for ( masxfs_levinfo_t * li = lia; rGOOD && li < lithis; li++ )
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
        SWARN( "POINT DB" );
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
    if ( rGOOD )
      n++;
  }
  QRLI( li, rCODE );
  rCODE = rCODE < 0 ? rCODE : n;
  return rCODE;
}

/*
MariaDB [masdufntdb]> SELECT * FROM dirfull LIMIT 40;
+---------+-----------+----------------+---------+---------+---------------------+
| node_id | parent_id | name           | name_id | data_id | mtim                |
+---------+-----------+----------------+---------+---------+---------------------+
|       2 |         1 | home           |       1 |       2 | 2017-02-01 09:56:44 |
|       3 |         2 | mastar         |       2 |       3 | 2017-03-11 13:49:51 |
|       4 |         3 | .mas           |       3 |       4 | 2016-11-20 12:39:53 |
|       5 |         4 | lib            |       4 |       5 | 2016-07-17 15:45:12 |
|       6 |         5 | big            |       5 |       6 | 2017-02-01 10:32:38 |
|       7 |         6 | misc           |       6 |       7 | 2016-03-22 19:33:28 |
|       8 |         7 | develop        |       7 |       8 | 2016-12-28 11:33:32 |
|       9 |         8 | autotools      |       8 |       9 | 2015-09-28 14:04:01 |
|      10 |         9 | zoc            |       9 |      10 | 2015-09-13 13:13:54 |
|      11 |        10 | projects       |      10 |      11 | 2017-03-11 14:08:36 |
|      12 |        11 | commonlibs     |      11 |      12 | 2017-03-07 11:29:34 |
|      13 |        12 | zocromas_xfs   |      12 |      13 | 2017-03-10 18:15:02 |
|      14 |        13 | mastest        |      13 |      14 | 2017-02-17 15:44:10 |
|      15 |        14 | tree           |      14 |      15 | 2017-02-17 15:46:56 |
|      16 |        15 | sh0            |      16 |      17 | 2015-05-27 23:14:15 |
|      17 |        15 | md5sha         |      31 |      32 | 2015-09-18 07:50:18 |
|      18 |        15 | tree.r         |      35 |      36 | 2017-02-17 15:46:56 |
|      19 |        18 | sh0            |      37 |      38 | 2015-05-27 23:14:15 |
|      20 |        18 | .auxdir        |      51 |      52 | 2017-02-17 15:46:17 |
|      21 |        20 | .build         |      52 |      53 | 2017-02-17 15:46:17 |
|      22 |        21 | src            |      56 |      57 | 2017-02-17 15:46:56 |
|      23 |        22 | inc            |      63 |      64 | 2015-09-14 11:26:20 |
|      24 |        22 | .libs          |      79 |      80 | 2015-09-13 10:40:32 |
|      25 |        22 | .deps          |     173 |     174 | 2015-09-15 14:17:50 |
|      26 |        20 | m4             |     331 |     332 | 2017-02-17 15:46:34 |
|      27 |        18 | md5sha         |     338 |     339 | 2015-09-18 07:50:18 |
|      28 |        18 | editing        |     344 |     345 | 2015-06-08 11:43:27 |
|      29 |        28 | .gvimvrb       |     345 |     346 | 2017-02-17 15:46:56 |
|      30 |        28 | mased          |     437 |     437 | 2015-09-29 08:18:44 |
|      31 |        18 | boxes          |     464 |     464 | 2015-05-27 21:58:22 |
|      32 |        18 | human          |     469 |     469 | 2017-02-17 15:46:05 |
|      33 |        32 | results        |     471 |     471 | 2017-02-17 15:45:30 |
|      34 |        32 | doc            |     472 |     472 | 2017-02-17 15:46:56 |
|      35 |        32 | run            |     482 |     482 | 2017-02-17 15:46:34 |
|      36 |        32 | examples       |     483 |     483 | 2015-09-02 16:00:12 |
|      37 |        18 | src            |     495 |     495 | 2017-02-17 15:46:17 |
|      38 |        37 | inc            |     500 |     500 | 2015-09-29 08:18:44 |
|      39 |        18 | debug          |     779 |     779 | 2015-10-19 16:57:45 |
|      40 |        18 | autom4te.cache |     783 |     783 | 2017-02-17 15:46:56 |
|      41 |        18 | test           |     787 |     787 | 2015-10-19 16:57:45 |
+---------+-----------+----------------+---------+---------+---------------------+
40 rows in set (0.00 sec)
 */
int
masxfs_levinfo_scan_dir_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * cbs, void *data, masxfs_levinfo_flags_t flags, masxfs_depth_t maxdepth,
                             masxfs_depth_t reldepth )
{
  rDECL( -1 );

  if ( ( flags & MASXFS_CB_MODE_DB ) )
    WARN( "POINT DB" );

  /* WARN( "'%s' : detype:%d", li ? li->name : NULL, li ? li->detype : 0 ); */
  if ( !( li && li->detype ) )
  {
    DIE( "WAW" );
  }
  masxfs_entry_type_t detype = masxfs_levinfo_detype( li, flags );

  if ( !( li && li->detype ) )
  {
    DIE( "WAW" );
  }
  /* WARN( "'%s' : detype:%d", li ? li->name : NULL, li ? li->detype : 0 ); */

/* ADIE( "'%s' : detype:%d", li ? li->name : NULL, li ? li->detype : 0 ); */

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
    for ( masxfs_depth_t d = 0; rGOOD && d < depth; d++ )
    {
      if ( lia[d].lidepth == d )
      {
        WARN( ">>o FROM ROOT '%s' (rel:%ld)", lia[d].name, ( long ) d - depth );
        rC( cbf( &lia[d], flags, udata, d - depth ) );
      }
      else
      {
        DIE( "FATAL ERROR" );
        rSETBAD;
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
