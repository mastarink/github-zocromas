#define R_GOOD(_r) ((_r)>=0)
#include "masxfs_levinfo_defs.h"
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_levinfo_structs.h"

#include "masxfs_levinfo_base.h"
#include "masxfs_levinfo_io_dir.h"
#include "masxfs_levinfo_ref.h"

#include "masxfs_levinfo_scan.h"

int
masxfs_levinfo_scan_li_cbs( masxfs_levinfo_t * li, masxfs_type_flags_t typeflags, masxfs_entry_callback_t * cbs, void *data,
                            masxfs_levinfo_flags_t flags, masxfs_depth_t maxdepth, masxfs_depth_t reldepth )
{
  rDECLBAD;

  rC( masxfs_levinfo_opendir( li, flags ) );
  if ( rGOOD )
  {
    int rc = 0;

    rC( masxfs_levinfo_scan_dirn_cbs( li, typeflags, cbs, data, flags, maxdepth, reldepth ) );
    rc = masxfs_levinfo_closedir( li, flags ); /* sic! */
    if ( rGOOD )
      rCODE = rc;
    QRLI( li, rCODE );
  }
  rRET;
}

static int
masxfs_levinfo_scan_entry_single_internal_1cb( masxfs_levinfo_t * lithis, masxfs_type_flags_t typeflags, masxfs_entry_callback_t * cb, void *data,
                                               masxfs_levinfo_flags_t flags, masxfs_depth_t reldepth )
{
  rDECLBAD;
  {
    masxfs_entry_type_bit_t entry_bit = 1 << lithis->detype;

    if ( ( typeflags & entry_bit ) )
    {
      if ( lithis->lidepth )
      {
        lithis[-1].child_count_pair[( flags & MASXFS_CB_COUNT ) ? 1 : 0]++;
      }
      {
        masxfs_levinfo_flags_t tflags = flags | cb->flags;

        if ( cb && cb->fun_simple && !( tflags & MASXFS_CB_SKIP ) )
        {
          masxfs_scan_fun_simple_t fun_simple = cb->fun_simple;

          rC( fun_simple( lithis, tflags, data, reldepth ) );
        }
        else
          rSETGOOD;
      }
    }
    else
      rSETGOOD;
    QRLI( li, rCODE );
  }
  rRET;
}

static int
masxfs_levinfo_scan_entry_single_internal_cbs( masxfs_levinfo_t * liparent, masxfs_levinfo_t * lithis, masxfs_type_flags_t typeflags,
                                               masxfs_entry_callback_t * cbs, void *data, masxfs_levinfo_flags_t flags, masxfs_depth_t reldepth )
{
  rDECLBAD;

  if ( liparent )
  {
    if ( lithis )
    {
      rSETGOOD;
      if ( cbs )
      {
        masxfs_entry_callback_t *cb = cbs;

        for ( int ncb = 0; rGOOD; cb++, ncb++ )
        {
          if ( ( flags & MASXFS_CB_UP_ROOT ) && reldepth <= 1 )      /* Once! ; ==1 for dir ; ==0 for file at initial path */
          {
            masxfs_levinfo_t *lia _uUu_ = lithis - lithis->lidepth;

            for ( masxfs_levinfo_t * li = lia; rGOOD && li < lithis; li++ )
            {
              assert( li->lidepth == ( masxfs_depth_t ) ( li - lia ) );
              if ( li->lidepth == ( masxfs_depth_t ) ( li - lia ) )
              {
                li->detype = MASXFS_ENTRY_DIR_NUM;
                rC( masxfs_levinfo_scan_entry_single_internal_1cb( li, typeflags, cb, data, flags & ~MASXFS_CB_UP_ROOT, li - lithis ) );
              }
              else
              {
                DIE( "WHY %ld != %ld", ( long ) li->lidepth, ( long ) ( li - lia ) );
              }
            }
          }
          /* assert( lithis->detype > MASXFS_ENTRY_UNKNOWN_NUM ); */
          rC( masxfs_levinfo_scan_entry_single_internal_1cb( lithis, typeflags, cb, data, flags & ~MASXFS_CB_UP_ROOT, reldepth ) );
          if ( ( flags & MASXFS_CB_SINGLE_CB ) || !cb || !cb->fun_simple )
            break;
        }
      }
    }
    else
      QRLI( li, rCODE );
  }
  else
    QRLIM( li, rCODE, "liparent not set" );
  rRET;
}

int
masxfs_levinfo_scan_entry_single_cbs( masxfs_levinfo_t * li, masxfs_type_flags_t typeflags, masxfs_entry_callback_t * cbs, void *data,
                                      masxfs_levinfo_flags_t flags, masxfs_depth_t reldepth )
{
  rDECLBAD;
  if ( li )
  {
    assert( li->lidepth );
    rC( masxfs_levinfo_scan_entry_single_internal_cbs( li->lidepth > 0 ? li - 1 : NULL, li, typeflags, cbs, data, flags, reldepth ) );
  }
  else
    QRLI( li, rCODE );
  rRET;
}

int
masxfs_levinfo_scan_entry_cbs( masxfs_levinfo_t * li, masxfs_type_flags_t typeflags _uUu_, masxfs_entry_callback_t * cbs _uUu_, void *data _uUu_,
                               masxfs_levinfo_flags_t flags _uUu_, masxfs_depth_t maxdepth _uUu_, masxfs_depth_t reldepth _uUu_ )
{
  rDECLBAD;

  if ( li )
  {
    rC( masxfs_levinfo_scan_entry_single_cbs( li, typeflags, cbs, data, flags, reldepth ) );
    QRLI( li, rCODE );
    if ( li->detype == MASXFS_ENTRY_DIR_NUM )
    {
      rC( masxfs_levinfo_scan_li_cbs( li, typeflags, cbs, data, flags, maxdepth, reldepth ) );
      QRLI( li, rCODE );
    }
    else if ( li->detype == MASXFS_ENTRY_REG_NUM )
    {
    /* rSETBAD; */
    /* DIE( "WOW" ); */
    }
    else if ( li->detype == MASXFS_ENTRY_LNK_NUM )
    {
    /* rSETBAD; */
    }
    else
    {
    /* rSETBAD; */
    }
  }
  else
    QRLI( li, rCODE );
  rRET;
}

int
masxfs_levinfo_scan_down_cbs( masxfs_levinfo_t * li, masxfs_type_flags_t typeflags, masxfs_entry_callback_t * cbs, void *data,
                              masxfs_levinfo_flags_t flags, masxfs_depth_t maxdepth, masxfs_depth_t reldepth )
{
  rDECLBAD;

  if ( li )
  {
    masxfs_depth_t lidepth = li->lidepth;

    li++;
    lidepth++;
    reldepth++;
    if ( ( flags & MASXFS_CB_RECURSIVE ) && ( maxdepth == 0 || ( maxdepth > 0 && li->lidepth < maxdepth - 1 ) ) )
    {
      rC( masxfs_levinfo_scan_entry_cbs( li, typeflags, cbs, data, flags, maxdepth, reldepth ) );
    }
    else
    {
      rSETGOOD;
    }
    QRLI( li, rCODE );
    masxfs_levinfo_reset( li, flags );
    reldepth--;
    lidepth--;
    li--;
  }
  else
    QRLI( li, rCODE );
  rRET;
}

/* NOREMOVE XXX */
#if 0
static void
masxfs_levinfo_fix_type( masxfs_levinfo_t * li _uUu_ )
{
/* TODO - Don't remove next 20170217.123704 */
# if 0
  if ( li->fs.pde->d_type == DT_UNKNOWN )
  {
    struct stat st;

#  if 1
    r = fstat(  .... );
#  else
    fpath = masxfs_normalize_path( path, li->fs.pde->d_name );
    r = lstat( fpath, &st );
    if ( fpath )
      mas_free( fpath );
#  endif
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
# endif
}
#endif

/*
 * bad:  r< 0
 * good: r>=0 */
static int
masxfs_levinfo_scan_dir_rest_cbs( masxfs_levinfo_t * li, masxfs_type_flags_t typeflags, masxfs_entry_callback_t * cbs, void *data,
                                  masxfs_levinfo_flags_t flags, masxfs_depth_t maxdepth, masxfs_depth_t reldepth )
{
  rDECLBAD;
  int n = 0;
  int has_data = 0;

  if ( li )
  {
    rSETGOOD;
    while ( rGOOD && rC( masxfs_levinfo_readdir( li, flags, &has_data ) ) && has_data )
    {
      rC( masxfs_levinfo_scan_down_cbs( li, typeflags, cbs, data, flags, maxdepth, reldepth ) );
      QRLI( li, rCODE );
      assert( !li[1].name );
      n++;
    }
    if ( rGOOD )
      rCODE = n;
  }
  else
    QRLI( li, rCODE );
  rRET;
}

int
masxfs_levinfo_scan_dir_cbs( masxfs_levinfo_t * li, masxfs_type_flags_t typeflags, masxfs_entry_callback_t * cbs, void *data,
                             masxfs_levinfo_flags_t flags, masxfs_depth_t maxdepth, masxfs_depth_t reldepth )
{
  rDECLBAD;

  assert( masxfs_levinfo_detype( li, flags ) == MASXFS_ENTRY_DIR_NUM );
  rC( masxfs_levinfo_rewinddir( li, flags ) );
  QRLI( li, rCODE );
  rC( masxfs_levinfo_scan_dir_rest_cbs( li, typeflags, cbs, data, flags, maxdepth, reldepth ) );
  QRLI( li, rCODE );
  rRET;
}

/* starting point */
int
masxfs_levinfo_scan_tree_cbs( masxfs_levinfo_t * li, masxfs_type_flags_t typeflags, masxfs_entry_callback_t * cbs, void *data,
                              masxfs_levinfo_flags_t flags, masxfs_depth_t maxdepth, masxfs_depth_t reldepth )
{
  rDECLBAD;
  if ( li )
  {
    masxfs_levinfo_flags_t cflags = flags | MASXFS_CB_COUNT | MASXFS_CB_SKIP | MASXFS_CB_SINGLE_CB;

  /* WARN( "PRE '%s'", li->name ); */
    do
    {
      masxfs_levinfo_flags_t sflags = 0;

      sflags = flags | ( ( flags & ( reldepth == 0 ? MASXFS_CB_SELF : MASXFS_CB_UP ) ) ? 0 : MASXFS_CB_SKIP );
      if ( li->lidepth )
      {
        rC( masxfs_levinfo_scan_entry_single_cbs( li, typeflags, cbs, data, cflags, reldepth ) );
        QRLI( li, rCODE );

        rC( masxfs_levinfo_scan_entry_single_cbs( li, typeflags, cbs, data, sflags, reldepth ) );
        QRLI( li, rCODE );
      }
      if ( reldepth <= 0 )
      {
        li++;
        reldepth++;
      }
    } while ( reldepth <= 0 );
    reldepth--;
    li--;
  /* WARN( "D%d; START: '%s' %d; max:%d", li->lidepth, li->name, reldepth, maxdepth ); */
    rC( masxfs_levinfo_scan_dirn_cbs( li, typeflags, cbs, data, flags, maxdepth, reldepth ) );
    QRLI( li, rCODE );
    memset( li->child_count_pair, 0, sizeof( li->child_count_pair ) );
  }
  else
    QRLI( li, rCODE );
  rRET;
}

int
masxfs_levinfo_scan_dirn_cbs( masxfs_levinfo_t * li, masxfs_type_flags_t typeflags, masxfs_entry_callback_t * cbs, void *data,
                              masxfs_levinfo_flags_t flags, masxfs_depth_t maxdepth, masxfs_depth_t reldepth )
{
  rDECLBAD;

  if ( li )
  {
    rSETGOOD;
    if ( masxfs_levinfo_detype( li, flags ) == MASXFS_ENTRY_DIR_NUM )
    {
      masxfs_levinfo_flags_t cflags = flags | MASXFS_CB_COUNT | MASXFS_CB_SKIP | MASXFS_CB_SINGLE_CB;

      rC( masxfs_levinfo_scan_dir_cbs( li, typeflags, cbs, data, cflags, maxdepth, reldepth ) );
      QRLI( li, rCODE );
      rC( masxfs_levinfo_scan_dir_cbs( li, typeflags, cbs, data, flags, maxdepth, reldepth ) );
      QRLI( li, rCODE );
      memset( li->child_count_pair, 0, sizeof( li->child_count_pair ) );
    }
  }
  else
    QRLI( li, rCODE );
  rRET;
}
