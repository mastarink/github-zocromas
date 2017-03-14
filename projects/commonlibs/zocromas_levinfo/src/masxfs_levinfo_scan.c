#define R_GOOD(_r) (_r>=0)
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
  rDECLGOOD;
  int rc = 0;

  rC( masxfs_levinfo_opendir( li, flags ) );
  if ( rGOOD )
  {
    rC( masxfs_levinfo_scan_dirn_cbs( li, cbs, data, flags, maxdepth, reldepth ) );
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
  rDECLGOOD;

  masxfs_scan_fun_simple_t fun_simple = cb->fun_simple;
  masxfs_entry_type_bit_t entry_bit = 1 << detype;

  if ( !( tflags & MASXFS_CB_SKIP ) && ( cb->types & entry_bit ) && fun_simple )
    rC( fun_simple( lithis, tflags, data, reldepth ) );
  rRET;
}

static int
masxfs_levinfo_scan_entry_single_internal_cbs( masxfs_levinfo_t * liparent, masxfs_levinfo_t * lithis, masxfs_entry_callback_t * cbs, void *data,
                                               masxfs_levinfo_flags_t flags, masxfs_entry_type_t detype, masxfs_depth_t reldepth )
{
  rDECLBAD;

  if ( liparent && lithis )
  {
    masxfs_entry_callback_t *cb = cbs;

    liparent->child_count_pair[!cb]++;
    rSETGOOD;
    for ( int ncb = 0; rGOOD && cb && cb->fun_simple; cb++, ncb++ )
    {
      masxfs_levinfo_flags_t tflags = 0;

      tflags = flags | cb->flags;

      if ( ( tflags & MASXFS_CB_FROM_ROOT ) && reldepth == 1 )
      {
        masxfs_levinfo_t *lia _uUu_ = lithis - lithis->lidepth;

        for ( masxfs_levinfo_t * li = lia; rGOOD && li < lithis; li++ )
        {

          if ( li->lidepth == ( masxfs_depth_t ) ( li - lia ) )
          {
            rC( masxfs_levinfo_scan_entry_single_internal_1cb( li, cb, data, tflags & ~MASXFS_CB_FROM_ROOT, MASXFS_ENTRY_DIR_NUM, li - lithis ) );
          }
          else
          {
            DIE( "WHY %ld != %ld", ( long ) li->lidepth, ( long ) ( li - lia ) );
          }
        }
      }
      rC( masxfs_levinfo_scan_entry_single_internal_1cb( lithis, cb, data, tflags & ~MASXFS_CB_FROM_ROOT, detype ? detype : lithis->detype,
                                                         reldepth ) );
      if ( !( tflags & MASXFS_CB_MULTIPLE_CBS ) )
        break;
    }
  }
  else
  {
  }
  rRET;
}

static int
masxfs_levinfo_scan_entry_single_at_child_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * cbs, void *data, masxfs_levinfo_flags_t flags,
                                               masxfs_depth_t reldepth )
{
  return li ? masxfs_levinfo_scan_entry_single_internal_cbs( li->lidepth > 0 ? li - 1 : NULL, li, cbs, data, flags, li->detype, reldepth ) : -1;
}

static int
masxfs_levinfo_scan_down_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * cbs, void *data, masxfs_levinfo_flags_t flags,
                              masxfs_depth_t maxdepth, masxfs_depth_t reldepth )
{
  rDECLBAD;

  if ( li )
  {
  /* masxfs_dirent_t *pde = flags & MASXFS_CB_MODE_FS ? li->fs.pde : li->db.pde; */

  /* if ( pde ) */
    {
      const char *name = masxfs_levinfo_scanned_name( li, flags );
      int detype = masxfs_levinfo_scanned_detype( li, flags );
      ino_t d_inode = masxfs_levinfo_scanned_inode( li, flags );
      unsigned long long node_id = masxfs_levinfo_scanned_nodeid( li, flags );

      masxfs_depth_t lidepth = li->lidepth;

      li++;
      lidepth++;
      reldepth++;
      {
        masxfs_levinfo_init( li, lidepth, name, detype, d_inode, node_id );
      /* if ( flags & MASXFS_CB_MODE_DB )                                                                            */
      /*   WARN( "!!!!!!!!! DOWN (db) \?\?!: \"%s\" ~~ %d : %d", li->name, detype, detype == MASXFS_ENTRY_DIR_NUM ); */
        rC( masxfs_levinfo_scan_entry_single_at_child_cbs( li, cbs, data, flags, reldepth ) );
        if ( detype == MASXFS_ENTRY_DIR_NUM )
        {
          rC( masxfs_levinfo_scan_li_cbs( li, cbs, data, flags, maxdepth, reldepth ) );
        }
        else if ( detype == MASXFS_ENTRY_REG_NUM )
        {
        /* rSETBAD; */
        }
        else if ( detype == MASXFS_ENTRY_LNK_NUM )
        {
        /* rSETBAD; */
        }
        else
        {
        /* rSETBAD; */
        }
        masxfs_levinfo_reset( li, flags );
      }
      reldepth--;
      lidepth--;
      li--;
    }
  }
  else
    QRLI( li, rCODE );
  rRET;
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
  int b = 0;

  if ( li )
  {
    const char *name = masxfs_levinfo_scanned_name( li, flags );

    b = name && !( name[0] == '.' && ( ( name[1] == '.' && name[2] == 0 ) || name[1] == 0 ) );
  }
  return b;
}

static int
masxfs_levinfo_scan_entry_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * cbs, void *data, masxfs_levinfo_flags_t flags,
                               masxfs_depth_t maxdepth, masxfs_depth_t reldepth )
{
  rDECLBAD;

  if ( li )
  {
    masxfs_levinfo_flags_t tflags = flags;

    rSETGOOD;
  /* if ( flags & MASXFS_CB_MODE_DB )     */
  /*   WARN( "######### -1- DOWN (db)" ); */
    if ( masxfs_levinfo_de_valid( li, tflags ) )
    {
      masxfs_levinfo_fix_type( li );
    /* if ( flags & MASXFS_CB_MODE_DB )     */
    /*   WARN( "######### -2- DOWN (db)" ); */
      if ( ( tflags & MASXFS_CB_RECURSIVE ) && ( maxdepth == 0 || ( maxdepth > 0 && li->lidepth < maxdepth ) ) )
      {
      /* if ( flags & MASXFS_CB_MODE_DB )                                                                         */
      /*   WARN( "######### TO DOWN (db) \"%s\" -> \"%s\"", li->name, masxfs_levinfo_scanned_name( li, flags ) ); */
        rC( masxfs_levinfo_scan_down_cbs( li, cbs, data, tflags, maxdepth, reldepth ) );
      }
      else
      {
      /* if ( flags & MASXFS_CB_MODE_DB )    */
      /*   WARN( "######### NO DOWN (db)" ); */
      }
    }
    else
    {
    /* if ( flags & MASXFS_CB_MODE_DB )                                                                  */
    /*   WARN( "######### -- DOWN (db) ---- not valid '%s'", masxfs_levinfo_scanned_name( li, flags ) ); */
    }
  }
  else
    QRLI( li, rCODE );
  rRET;
}

/* 
 * bad:  r< 0
 * good: r>=0 */
static int
masxfs_levinfo_scan_dir_rest_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * cbs, void *data, masxfs_levinfo_flags_t flags,
                                  masxfs_depth_t maxdepth, masxfs_depth_t reldepth )
{
  rDECLGOOD;
  int n = 0;
  int has_data = 0;

  while ( rGOOD && rC( masxfs_levinfo_readdir( li, flags, &has_data ) ) && has_data )
  {
  /* if ( flags & MASXFS_CB_MODE_DB )       */
  /*   WARN( "********* SCAN entry (db)" ); */
    rC( masxfs_levinfo_scan_entry_cbs( li, cbs, data, flags, maxdepth, reldepth ) );
  /* if ( ( flags & MASXFS_CB_MODE_DB ) && rBAD ) */
  /*   WARN( "r:%d", rCODE );                     */
    if ( rGOOD )
      n++;
  }
  if ( rGOOD )
    rCODE = n;
/* if ( flags & MASXFS_CB_MODE_DB )                */
/*   WARN( "END SCAN (db) rest cbs: %d;", rCODE ); */
  rRET;
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
 */
int
masxfs_levinfo_scan_dir_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * cbs, void *data, masxfs_levinfo_flags_t flags, masxfs_depth_t maxdepth,
                             masxfs_depth_t reldepth )
{
  rDECLBAD;

  masxfs_entry_type_t detype = masxfs_levinfo_detype( li, flags );

  switch ( detype )
  {
  case MASXFS_ENTRY_DIR_NUM:
    rC( masxfs_levinfo_rewinddir( li, flags ) );
    if ( flags & MASXFS_CB_MODE_DB && rBAD )
      WARN( "############# r:%d", rCODE );
    rC( masxfs_levinfo_scan_dir_rest_cbs( li, cbs, data, flags, maxdepth, reldepth ) );
    if ( flags & MASXFS_CB_MODE_DB && rBAD )
      WARN( "############# r:%d", rCODE );
    break;
  case MASXFS_ENTRY_REG_NUM:
    rC( masxfs_levinfo_scan_entry_single_at_child_cbs( li, cbs, data, flags, reldepth ) );
    if ( flags & MASXFS_CB_MODE_DB && rBAD )
      WARN( "############# r:%d", rCODE );
    break;
  default:
    WARN( "#-=-#-=-#-=-#-=-#-=-#-=-#-=-#-=-#-=-#-=-#-=-#-=-#-=-#  %s (%d:%d:%d)", masxfs_levinfo_detype_name( li ), li->detype, detype,
          MASXFS_ENTRY_DIR_NUM );
    break;
  }
  if ( flags & MASXFS_CB_MODE_DB && rBAD )
    WARN( "############# r:%d", rCODE );
  rRET;
}

int
masxfs_levinfo_scan_dirn_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * cbs, void *data, masxfs_levinfo_flags_t flags,
                              masxfs_depth_t maxdepth, masxfs_depth_t reldepth )
{
  rDECLBAD;

  if ( li )
  {
    memset( li->child_count_pair, 0, sizeof( li->child_count_pair ) );
    rC( masxfs_levinfo_scan_dir_cbs( li, NULL, data, flags, maxdepth, reldepth ) );
    rC( masxfs_levinfo_scan_dir_cbs( li, cbs, data, flags, maxdepth, reldepth ) );
  }
  else
    QRLI( li, rCODE );
  rRET;
}

int
masxfs_levinfo_scan_depth_cbf( masxfs_levinfo_t * lia, masxfs_depth_t depth, masxfs_scan_fun_simple_t cbf, void *udata _uUu_,
                               masxfs_levinfo_flags_t flags )
{
  rDECLBAD;

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
  else
    QRLI( li, rCODE );
  rRET;
}
