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

#include "masxfs_levinfo_scanned.h"

#include "masxfs_levinfo_scan.h"

int
masxfs_levinfo_scan_li_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * cbs, void *data, masxfs_levinfo_flags_t flags, masxfs_depth_t maxdepth,
                            masxfs_depth_t reldepth )
{
  rDECLBAD;
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
  rDECLGOOD;                                                         /* sic! 20170314.185257 */

  masxfs_scan_fun_simple_t fun_simple = cb->fun_simple;
  masxfs_entry_type_bit_t entry_bit = 1 << detype;

  /* WARN( "[%s]", lithis ? lithis->name : NULL ); */
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

      /* WARN( "[%s] RD:%ld", lithis ? lithis->name : NULL, ( long ) reldepth ); */
      if ( ( tflags & MASXFS_CB_FROM_ROOT ) && reldepth <= 1 ) /* Once! ; ==1 for dir ; ==0 for file at initial path */
      {
        masxfs_levinfo_t *lia _uUu_ = lithis - lithis->lidepth;

        /* WARN( "[%s]", lithis ? lithis->name : NULL ); */
        for ( masxfs_levinfo_t * li = lia; rGOOD && li < lithis; li++ )
        {
          /* WARN( "[%s]", lithis ? lithis->name : NULL ); */
          if ( li->lidepth == ( masxfs_depth_t ) ( li - lia ) )
          {
            /* WARN( "[%s]", lithis ? lithis->name : NULL ); */
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
masxfs_levinfo_scan_entry_single_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * cbs, void *data, masxfs_levinfo_flags_t flags,
                                      masxfs_depth_t reldepth )
{
  return li ? masxfs_levinfo_scan_entry_single_internal_cbs( li->lidepth > 0 ? li - 1 : NULL, li, cbs, data, flags, li->detype, reldepth ) : -1;
}

#if 0
static int
masxfs_levinfo_init_scan_entry_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * cbs, void *data, masxfs_levinfo_flags_t flags,
                                    masxfs_depth_t maxdepth, masxfs_depth_t reldepth, masxfs_depth_t lidepth, const char *name,
                                    masxfs_entry_type_t detype, ino_t d_inode, unsigned long long node_id, masxfs_stat_t * destat )
{
  rDECLBAD;
  masxfs_levinfo_init( li, lidepth, name, detype, d_inode, node_id, destat );
  rC( masxfs_levinfo_scan_entry_single_cbs( li, cbs, data, flags, reldepth ) );
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
  rRET;
}
#endif

static int
masxfs_levinfo_scan_down_cbs( masxfs_levinfo_t * li, masxfs_entry_callback_t * cbs, void *data, masxfs_levinfo_flags_t flags,
                              masxfs_depth_t maxdepth, masxfs_depth_t reldepth )
{
  rDECLBAD;

  if ( li )
  {
    const char *name = masxfs_levinfo_scanned_name( li, flags );
    masxfs_entry_type_t detype = masxfs_levinfo_scanned_detype( li, flags );
    ino_t d_inode = masxfs_levinfo_scanned_inode( li, flags );
    unsigned long long node_id = masxfs_levinfo_scanned_nodeid( li, flags );

  /*last (zeroes stat), at least
   * after refer to stat,
   * like size masxfs_levinfo_scanned_size!:
   * */ masxfs_stat_t *destat = masxfs_levinfo_scanned_stat( li, flags, TRUE );

    masxfs_depth_t lidepth = li->lidepth;

    li++;
    lidepth++;
    reldepth++;

  /* TODO function masxfs_levinfo_init_scan_entry_cbs  */
    {
      masxfs_levinfo_init( li, lidepth, name, detype, d_inode, node_id, destat );
      rC( masxfs_levinfo_scan_entry_single_cbs( li, cbs, data, flags, reldepth ) );
      if ( detype == MASXFS_ENTRY_DIR_NUM )
      {
        rC( masxfs_levinfo_scan_li_cbs( li, cbs, data, flags, maxdepth, reldepth ) );
      }
      else if ( detype == MASXFS_ENTRY_REG_NUM )
      {
      /* rSETBAD; */
        /* DIE( "WOW" ); */
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
    rSETGOOD;
  /* if ( flags & MASXFS_CB_MODE_DB )     */
  /*   WARN( "######### -1- DOWN (db)" ); */
    if ( masxfs_levinfo_de_valid( li, flags ) )
    {
      masxfs_levinfo_fix_type( li );
    /* if ( flags & MASXFS_CB_MODE_DB )     */
    /*   WARN( "######### -2- DOWN (db)" ); */
      if ( ( flags & MASXFS_CB_RECURSIVE ) && ( maxdepth == 0 || ( maxdepth > 0 && li->lidepth < maxdepth ) ) )
      {
      /* if ( flags & MASXFS_CB_MODE_DB )                                                                         */
      /*   WARN( "######### TO DOWN (db) \"%s\" -> \"%s\"", li->name, masxfs_levinfo_scanned_name( li, flags ) ); */
        rC( masxfs_levinfo_scan_down_cbs( li, cbs, data, flags, maxdepth, reldepth ) );
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
    rC( masxfs_levinfo_scan_entry_single_cbs( li, cbs, data, flags, reldepth ) );
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

/* starting point */
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
