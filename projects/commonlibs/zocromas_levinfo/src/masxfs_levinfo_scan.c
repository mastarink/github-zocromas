#define R_GOOD( _r ) ( ( _r ) >= 0 )
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

#include "masxfs_levinfo_io.h"

#include "masxfs_levinfo_scan.h"

static int
masxfs_levinfo_scanf_dir_scanner( masxfs_levinfo_t * li, masxfs_scanner_t * scanner, masxfs_levinfo_flags_t more_flags, void *userdata,
                                  masxfs_depth_t reldepth );

int
masxfs_levinfo_scanf_li_scanner( masxfs_levinfo_t * li, masxfs_scanner_t * scanner, masxfs_levinfo_flags_t more_flags, void *userdata,
                                 masxfs_depth_t reldepth )
{
  rDECLBAD;

  masxfs_levinfo_flags_t flags = scanner->flags | more_flags;

  rC( masxfs_levinfo_opendir( li, flags ) );
  if ( rGOOD )
  {
    int rc = 0;

    rC( masxfs_levinfo_scanf_dir_scanner( li, scanner, more_flags, userdata, reldepth ) );
    rc = masxfs_levinfo_closedir( li, flags );                       /* sic! */
    if ( rGOOD )
      rCODE = rc;
    QRLI( li, rCODE );
  }
  rRET;
}

static int
masxfs_levinfo_scanf_entry_single_internal_1cb( masxfs_levinfo_t * lithis, masxfs_entry_filter_t * entry_pfilter, masxfs_entry_callback_t * cb,
                                                void *userdata, masxfs_levinfo_flags_t flags, masxfs_depth_t reldepth )
{
  rDECLBAD;
  {
    masxfs_entry_type_bit_t entry_bit = 1 << lithis->detype;

    if ( entry_pfilter && ( entry_pfilter->typeflags & entry_bit ) && cb )
    {
      masxfs_depth_t maxdepthc = cb->entry_filter.maxdepth;

      if ( lithis->lidepth )
      {
        if ( flags & MASXFS_CB_COUNT )
        {
          if ( lithis->detype != MASXFS_ENTRY_DIR_NUM )
            lithis[-1].leaf_count++;
          else
            lithis[-1].leaf_count += lithis->leaf_count;
          if ( !( flags & MASXFS_CB_SKIP_EMPTY ) || lithis->leaf_count || lithis->detype != MASXFS_ENTRY_DIR_NUM )
            lithis[-1].child_count_pair[1]++;
        }
        else
        {
          if ( !( flags & MASXFS_CB_SKIP_EMPTY ) || lithis->leaf_count || lithis->detype != MASXFS_ENTRY_DIR_NUM )
            lithis[-1].child_count_pair[0]++;
        }
      }

      if ( maxdepthc == 0 || ( maxdepthc > 0 && lithis->lidepth < maxdepthc - 1 ) )
      {
        masxfs_levinfo_flags_t tflags = flags | cb->flags;

        if ( !( flags & MASXFS_CB_SKIP ) && ( !( flags & MASXFS_CB_SKIP_EMPTY ) || lithis->detype != MASXFS_ENTRY_DIR_NUM || lithis->leaf_count ) )
        {
          int fun_called = 0;
          masxfs_depth_t depth = masxfs_levinfo_depth_ref( lithis, flags );

          if ( !cb->fun_counter && depth )
            cb->fun_top_depth = depth - 1;

          if ( cb && cb->fun_simple )
          {
            masxfs_scan_fun_simple_t fun_simple = cb->fun_simple;

//            const char *prefix = NULL;
//            prefix = masxfs_levinfo_prefix_ref( lithis, "    ", "└── ", "│   ", "├── ", cb->fun_top_depth + 1, tflags );

            rC( fun_simple( lithis, tflags, userdata, cb->fun_counter, reldepth ) );
            fun_called++;
          }
          else
            rSETGOOD;
          if ( rGOOD && cb && cb->fun_stat )
          {
            masxfs_scan_fun_stat_t fun_stat = cb->fun_stat;
            struct stat *st = NULL;

            rC( masxfs_levinfo_stat( lithis, tflags ) );
            if ( rGOOD )
            {
              if ( flags & MASXFS_CB_MODE_DB )
                st = lithis->db.stat;
              else if ( flags & MASXFS_CB_MODE_FS )
                st = lithis->fs.stat;
              if ( st )
              {
                const char *prefix = NULL;

                prefix = masxfs_levinfo_prefix_ref( lithis, "    ", "└── ", "│   ", "├── ", cb->fun_top_depth + 1, tflags );
                rC( fun_stat( masxfs_levinfo_name_ref( lithis, flags ), st, userdata, ( unsigned ) depth, cb->fun_counter, prefix, reldepth ) );
                fun_called++;
              }
            }
          }
          else
            rSETGOOD;
          if ( fun_called )
            cb->fun_counter++;
        }
        else
          rSETGOOD;
      }
      else
        rSETGOOD;
    }
    else
      rSETGOOD;
    QRLI( li, rCODE );
  }
  rRET;
}

static int
masxfs_levinfo_scanf_entry_single_internal_scanner( masxfs_levinfo_t * liparent, masxfs_levinfo_t * lithis, masxfs_scanner_t * scanner,
                                                    masxfs_levinfo_flags_t more_flags, void *userdata, masxfs_depth_t reldepth )
{
  rDECLBAD;
/* lithis never NULL */
  if ( liparent )
  {
  /* if ( lithis ) */
  /* { */
    rSETGOOD;
    masxfs_entry_filter_t *entry_pfilter = scanner->entry_pfilter;
    masxfs_entry_callback_t *cbs = scanner->cbs;
    masxfs_levinfo_flags_t flags = scanner->flags | more_flags;

    if ( cbs )
    {
      masxfs_entry_callback_t *cb = cbs;

      for ( int ncb = 0; rGOOD; cb++, ncb++ )
      {
        if ( ( flags & MASXFS_CB_UP_ROOT ) && reldepth <= 1 )        /* Once! ; ==1 for dir ; ==0 for file at initial path */
        {
          masxfs_levinfo_t *lia _uUu_ = lithis - lithis->lidepth;

          for ( masxfs_levinfo_t * li = lia; rGOOD && li < lithis; li++ )
          {
            assert( li->lidepth == ( masxfs_depth_t ) ( li - lia ) );
            if ( li->lidepth == ( masxfs_depth_t ) ( li - lia ) )
            {
              li->detype = MASXFS_ENTRY_DIR_NUM;
              rC( masxfs_levinfo_scanf_entry_single_internal_1cb( li, entry_pfilter, cb, userdata, flags & ~MASXFS_CB_UP_ROOT, li - lithis ) );
            }
            else
            {
              DIE( "WHY %ld != %ld", ( long ) li->lidepth, ( long ) ( li - lia ) );
            }
          }
        }
      /* assert( lithis->detype > MASXFS_ENTRY_UNKNOWN_NUM ); */
        rC( masxfs_levinfo_scanf_entry_single_internal_1cb( lithis, entry_pfilter, cb, userdata, flags & ~MASXFS_CB_UP_ROOT, reldepth ) );
        if ( ( flags & MASXFS_CB_SINGLE_CB ) || !cb || !cb->fun_simple )
          break;
      }
    }
  /* }                    */
  /* else                 */
  /*   QRLI( li, rCODE ); */
  }
  else
    QRLIM( li, rCODE, "liparent not set" );
  rRET;
}

/*
static int
masxfs_levinfo_scanf_entry_single_internal_cbs( masxfs_levinfo_t * liparent, masxfs_levinfo_t * lithis, masxfs_entry_filter_t * entry_pfilter,
                                                masxfs_entry_callback_t * cbs, void *userdata, masxfs_levinfo_flags_t flags, masxfs_depth_t reldepth )
{
  masxfs_scanner_t scanner = {.entry_pfilter = entry_pfilter,.cbs = cbs,.flags = flags };
  return masxfs_levinfo_scanf_entry_single_internal_scanner( liparent, lithis, &scanner, 0, userdata, reldepth );
} */

static int
masxfs_levinfo_scanf_entry_single_scanner( masxfs_levinfo_t * li, masxfs_scanner_t * scanner, masxfs_levinfo_flags_t more_flags,
                                           void *userdata, masxfs_depth_t reldepth )
{
  rDECLBAD;
/* masxfs_entry_filter_t *entry_pfilter = scanner->entry_pfilter; */
/* masxfs_entry_callback_t *cbs = scanner->cbs; */
/* masxfs_levinfo_flags_t flags = scanner->flags | more_flags; */

  if ( li )
  {
    assert( li->lidepth );
    rC( masxfs_levinfo_scanf_entry_single_internal_scanner( li->lidepth > 0 ? li - 1 : NULL, li, scanner, more_flags, userdata, reldepth ) );
  }
  else
    QRLI( li, rCODE );
  rRET;
}

static int
masxfs_levinfo_scanf_entry_scanner( masxfs_levinfo_t * li, masxfs_scanner_t * scanner, masxfs_levinfo_flags_t more_flags, void *userdata,
                                    masxfs_depth_t reldepth )
{
  rDECLBAD;

  if ( li )
  {
    if ( li->detype == MASXFS_ENTRY_DIR_NUM )
    {
      rC( masxfs_levinfo_scanf_dir_scanner( li, scanner, more_flags | MASXFS_CB_COUNT | MASXFS_CB_SKIP | MASXFS_CB_SINGLE_CB, userdata, reldepth ) );
      QRLI( li, rCODE );
    }
    rC( masxfs_levinfo_scanf_entry_single_scanner( li, scanner, more_flags, userdata, reldepth ) );
    QRLI( li, rCODE );
    if ( li->detype == MASXFS_ENTRY_DIR_NUM )
    {
      rC( masxfs_levinfo_scanf_li_scanner( li, scanner, more_flags, userdata, reldepth ) );
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
masxfs_levinfo_match_filter( masxfs_levinfo_t * li, masxfs_entry_filter_t * entry_pfilter )
{
  int f = 1;

  if ( entry_pfilter )
  {
    masxfs_depth_t maxdepthf = entry_pfilter->maxdepth;

    f = f && ( maxdepthf == 0 || ( maxdepthf > 0 && li->lidepth < maxdepthf - 1 ) );
  }
  return f;
}

int
masxfs_levinfo_scanf_down_scanner( masxfs_levinfo_t * li, masxfs_scanner_t * scanner, masxfs_levinfo_flags_t more_flags, void *userdata,
                                   masxfs_depth_t reldepth )
{
  rDECLBAD;

  if ( li )
  {
    masxfs_entry_filter_t *entry_pfilter = scanner->entry_pfilter;
    masxfs_levinfo_flags_t flags = scanner->flags | more_flags;
    masxfs_depth_t lidepth = li->lidepth;

    li++;
    lidepth++;
    reldepth++;
    {
      masxfs_entry_filter_t maxdepth_filter = {.maxdepth = scanner->maxdepth };

      if ( ( flags & MASXFS_CB_RECURSIVE ) && masxfs_levinfo_match_filter( li, &maxdepth_filter )
           && masxfs_levinfo_match_filter( li, entry_pfilter ) )
      {
        rC( masxfs_levinfo_scanf_entry_scanner( li, scanner, more_flags, userdata, reldepth ) );
      }
      else
      {
        rSETGOOD;
      }
      QRLI( li, rCODE );
      masxfs_levinfo_reset( li, flags );
    }
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
masxfs_levinfo_scanf_dir_rest_scanner( masxfs_levinfo_t * li, masxfs_scanner_t * scanner, masxfs_levinfo_flags_t more_flags,
                                       void *userdata, masxfs_depth_t reldepth )
{
  rDECLBAD;
  int n = 0;
  int has_data = 0;

  if ( li )
  {
    rSETGOOD;
    masxfs_levinfo_flags_t flags = scanner->flags | more_flags;

/* TODO filter here ?! 20171229.122350 */
    while ( rGOOD && rC( masxfs_levinfo_readdir( li, scanner->entry_pfilter, flags, &has_data ) ) && has_data )
    {
      rC( masxfs_levinfo_scanf_down_scanner( li, scanner, more_flags, userdata, reldepth ) );
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

static int
masxfs_levinfo_scanf_dir_scanner( masxfs_levinfo_t * li, masxfs_scanner_t * scanner, masxfs_levinfo_flags_t more_flags, void *userdata,
                                  masxfs_depth_t reldepth )
{
  rDECLBAD;
  masxfs_levinfo_flags_t flags = scanner->flags | more_flags;

  assert( masxfs_levinfo_detype( li, flags ) == MASXFS_ENTRY_DIR_NUM );
  rC( masxfs_levinfo_rewinddir( li, flags ) );
  rC( masxfs_levinfo_scanf_dir_rest_scanner( li, scanner, more_flags, userdata, reldepth ) );
  rRET;
}

/* starting point */
int
masxfs_levinfo_scanf_tree_scanner( masxfs_levinfo_t * li, masxfs_scanner_t * scanner, void *userdata, masxfs_depth_t reldepth )
{
  rDECLBAD;

  masxfs_levinfo_flags_t flags = scanner->flags;

  if ( scanner )
  {
    masxfs_entry_callback_t *cb = scanner->cbs;

    for ( int ncb = 0; rGOOD; cb++, ncb++ )
    {
      cb->fun_counter = 0;
      if ( ( flags & MASXFS_CB_SINGLE_CB ) || !cb || ( !cb->fun_simple && !cb->fun_stat ) )
        break;
    }
    if ( li )
    {
      do
      {
        if ( li->lidepth )
        {
          rC( masxfs_levinfo_scanf_entry_single_scanner( li, scanner, MASXFS_CB_COUNT | MASXFS_CB_SKIP | MASXFS_CB_SINGLE_CB, userdata, reldepth ) );
          QRLI( li, rCODE );

        // XXX XXX
          rC( masxfs_levinfo_scanf_entry_single_scanner
              ( li, scanner, ( flags & ( reldepth == 0 ? MASXFS_CB_SELF : MASXFS_CB_UP ) ) ? 0 : MASXFS_CB_SKIP, userdata, reldepth ) );
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

      rC( masxfs_levinfo_scanf_dir_scanner( li, scanner, 0 | MASXFS_CB_COUNT | MASXFS_CB_SKIP | MASXFS_CB_SINGLE_CB, userdata, reldepth ) );
      rC( masxfs_levinfo_scanf_dir_scanner( li, scanner, 0, userdata, reldepth ) );
      memset( li->child_count_pair, 0, sizeof( li->child_count_pair ) );
    }
    else
      QRLI( li, rCODE );
  }
  else
    QRG( rCODE );
  rRET;
}

#if 0
static int
masxfs_levinfo_scanf_dirn_scanner( masxfs_levinfo_t * li, masxfs_scanner_t * scanner, masxfs_levinfo_flags_t more_flags, void *userdata,
                                   masxfs_depth_t reldepth )
{
  rDECLBAD;

  if ( li )
  {
    rSETGOOD;
    masxfs_levinfo_flags_t flags = scanner->flags | more_flags;

    if ( masxfs_levinfo_detype( li, flags ) == MASXFS_ENTRY_DIR_NUM )
    {
    /* rC( masxfs_levinfo_scanf_dir_scanner( li, scanner, more_flags | MASXFS_CB_COUNT | MASXFS_CB_SKIP | MASXFS_CB_SINGLE_CB, userdata, reldepth ) ); */
      rC( masxfs_levinfo_scanf_dir_scanner( li, scanner, more_flags, userdata, reldepth ) );
      memset( li->child_count_pair, 0, sizeof( li->child_count_pair ) );
    }
  }
  else
    QRLI( li, rCODE );
  rRET;
}
#endif
