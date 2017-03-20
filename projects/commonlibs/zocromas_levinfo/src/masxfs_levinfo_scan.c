#define R_GOOD(_r) (_r>=0)
#include "masxfs_levinfo_defs.h"
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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
masxfs_levinfo_scan_li_cbs( masxfs_levinfo_t * li, masxfs_type_flags_t typeflags, masxfs_entry_callback_t * cbs, void *data,
                            masxfs_levinfo_flags_t flags, masxfs_depth_t maxdepth, masxfs_depth_t reldepth )
{
  rDECLBAD;

  rC( masxfs_levinfo_opendir( li, flags ) );
  if ( rGOOD )
  {
    int rc = 0;

    rC( masxfs_levinfo_scan_dirn_cbs( li, typeflags, cbs, data, flags, maxdepth, reldepth ) );
    rc = masxfs_levinfo_closedir( li, flags );
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

  if ( liparent && lithis && cbs )
  {
    masxfs_entry_callback_t *cb = cbs;

    rSETGOOD;
    for ( int ncb = 0; rGOOD; cb++, ncb++ )
    {
      if ( ( flags & MASXFS_CB_UP_ROOT ) && reldepth <= 1 )          /* Once! ; ==1 for dir ; ==0 for file at initial path */
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
      assert( lithis->detype > MASXFS_ENTRY_UNKNOWN_NUM );
      rC( masxfs_levinfo_scan_entry_single_internal_1cb( lithis, typeflags, cb, data, flags & ~MASXFS_CB_UP_ROOT, reldepth ) );
      if ( ( flags & MASXFS_CB_SINGLE_CB ) || !cb || !cb->fun_simple )
        break;
    }
  }
  else
  {
  }
  rRET;
}

static int
masxfs_levinfo_scan_entry_single_cbs( masxfs_levinfo_t * li, masxfs_type_flags_t typeflags, masxfs_entry_callback_t * cbs, void *data,
                                      masxfs_levinfo_flags_t flags, masxfs_depth_t reldepth )
{
  return li ? masxfs_levinfo_scan_entry_single_internal_cbs( li->lidepth > 0 ? li - 1 : NULL, li, typeflags, cbs, data, flags, reldepth ) : -1;
}

static int
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
    /* if ( li->detype == MASXFS_ENTRY_DIR_NUM )             */
    /*   WARN( "DOWN '%s' => '%s'", li[-1].name, li->name ); */
    {
      rC( masxfs_levinfo_scan_entry_single_cbs( li, typeflags, cbs, data, flags, reldepth ) );
      if ( li->detype == MASXFS_ENTRY_DIR_NUM )
      {
        rC( masxfs_levinfo_scan_li_cbs( li, typeflags, cbs, data, flags, maxdepth, reldepth ) );
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
    /* if ( li->detype == MASXFS_ENTRY_DIR_NUM )           */
    /*   WARN( "UP '%s' => '%s'", li->name, li[-1].name ); */
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

static int
masxfs_levinfo_scan_entry_cbs( masxfs_levinfo_t * li, masxfs_type_flags_t typeflags, masxfs_entry_callback_t * cbs, void *data,
                               masxfs_levinfo_flags_t flags, masxfs_depth_t maxdepth, masxfs_depth_t reldepth )
{
  rDECLBAD;

  if ( li )
  {
    rSETGOOD;
  /*NOREMOVE XXX if ( masxfs_levinfo_de_valid( li, flags ) ) */
    {
    /*NOREMOVE XXX masxfs_levinfo_fix_type( li ); */
    /* WARN("maxdepth:%d", maxdepth); */
      if ( ( flags & MASXFS_CB_RECURSIVE ) && ( maxdepth == 0 || ( maxdepth > 0 && li->lidepth < maxdepth - 1 ) ) )
        rC( masxfs_levinfo_scan_down_cbs( li, typeflags, cbs, data, flags, maxdepth, reldepth ) );
      masxfs_levinfo_reset( li + 1, flags );
      assert( !li[1].name );
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
masxfs_levinfo_scan_dir_rest_cbs( masxfs_levinfo_t * li, masxfs_type_flags_t typeflags, masxfs_entry_callback_t * cbs, void *data,
                                  masxfs_levinfo_flags_t flags, masxfs_depth_t maxdepth, masxfs_depth_t reldepth )
{
  rDECLBAD;
  int n = 0;
  int has_data = 0;

  if ( li )
  {
    while ( rC( masxfs_levinfo_readdir( li, flags, &has_data ) ) && has_data )
    {
    /* WARN("HAS DATA"); */
      rC( masxfs_levinfo_scan_entry_cbs( li, typeflags, cbs, data, flags, maxdepth, reldepth ) );
      assert( !li[1].name );
      if ( rBAD )
        break;
      n++;
    }
    /* WARN( "EOD" ); */
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

  masxfs_entry_type_t detype = masxfs_levinfo_detype( li, flags );

  switch ( detype )
  {
  case MASXFS_ENTRY_DIR_NUM:
    rC( masxfs_levinfo_rewinddir( li, flags ) );
    rC( masxfs_levinfo_scan_dir_rest_cbs( li, typeflags, cbs, data, flags, maxdepth, reldepth ) );
    break;
  case MASXFS_ENTRY_REG_NUM:
    rC( masxfs_levinfo_scan_entry_single_cbs( li, typeflags, cbs, data, flags, reldepth ) );
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

/* starting point */
int
masxfs_levinfo_scan_dirn_cbs( masxfs_levinfo_t * li, masxfs_type_flags_t typeflags, masxfs_entry_callback_t * cbs, void *data,
                              masxfs_levinfo_flags_t flags, masxfs_depth_t maxdepth, masxfs_depth_t reldepth )
{
  rDECLBAD;

  if ( li )
  {
    memset( li->child_count_pair, 0, sizeof( li->child_count_pair ) );
    /* WARN( "STAGE A" ); */
    rC( masxfs_levinfo_scan_dir_cbs( li, typeflags, cbs, data, flags | MASXFS_CB_COUNT | MASXFS_CB_SKIP | MASXFS_CB_SINGLE_CB, maxdepth, reldepth ) );
    /* WARN( "STAGE B" ); */
    rC( masxfs_levinfo_scan_dir_cbs( li, typeflags, cbs, data, flags, maxdepth, reldepth ) );
    /* WARN( "/STAGES" ); */
  }
  else
    QRLI( li, rCODE );
  rRET;
}
