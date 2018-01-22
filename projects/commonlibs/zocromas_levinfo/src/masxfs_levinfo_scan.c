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
#include "masxfs_levinfo_tools.h"

#include "masxfs_levinfo_io.h"

#include "masxfs_levinfo_scan.h"

static unsigned entry_scanner_cnt = 0;

/* scan all childs, for open  */
static int
masxfs_levinfo_scanf_dir_scanner( masxfs_levinfo_t * li, masxfs_scanner_t * scanner, masxfs_levinfo_flags_t more_flags, void *userdata,
                                  masxfs_depth_t reldepth );

/* scan all childs, for closed: 
 * 1. open
 * 2. scanf_dir_scanner
 * 3. close
 * */
int
masxfs_levinfo_scanf_li_scanner( masxfs_levinfo_t * li, masxfs_scanner_t * scanner, masxfs_levinfo_flags_t more_flags, void *userdata,
                                 masxfs_depth_t reldepth )
{
  rDECLBAD;

  masxfs_levinfo_flags_t flags = scanner->flags | more_flags;

  assert( !masxfs_levinfo_opened_dir( li, flags ) );
  rC( masxfs_levinfo_opendir( li, flags, scanner->entry_pfilter ) );
  assert( masxfs_levinfo_opened_dir( li, flags ) );
  if ( rGOOD )
  {
/* scan all childs, for open  */
    rC( masxfs_levinfo_scanf_dir_scanner( li, scanner, more_flags, userdata, reldepth ) );
#if 1
    rC( masxfs_levinfo_closedir_force( li, flags, rCODE ) );
#else
    {
      int rc = 0;

      rc = masxfs_levinfo_closedir( li, flags );                     /* sic! */
      if ( rGOOD )
        rCODE = rc;
    }
#endif
    QRLI( li, rCODE );
  }
  assert( !masxfs_levinfo_opened_dir( li, flags ) );
  rRET;
}

/* scan self */
static int
masxfs_levinfo_scanf_entry_single_internal_1cb( masxfs_levinfo_t * lithis, masxfs_entry_filter_t * entry_pfilter, masxfs_entry_callback_t * cb,
                                                void *userdata, masxfs_levinfo_flags_t flags, masxfs_depth_t reldepth )
{
  rDECLBAD;
  {
    if ( cb )
    {
      /* masxfs_depth_t maxdepthc = cb->entry_filter.maxdepth; */

      if ( 1 /* && maxdepthc == 0 || ( maxdepthc > 0 && lithis->lidepth < maxdepthc - 1 ) */ )
      {
        masxfs_levinfo_flags_t tflags = flags | cb->flags;

        if ( !( flags & MASXFS_CB_SKIP ) && ( !( flags & MASXFS_CB_SKIP_EMPTY ) || lithis->detype != MASXFS_ENTRY_DIR_NUM || lithis->leaf_count )
             && masxfs_levinfo_stat_valid( lithis, entry_pfilter, flags ) && masxfs_levinfo_xstat_valid( lithis, entry_pfilter, flags ) )
        {
          int fun_called = 0;
          masxfs_depth_t depth = masxfs_levinfo_depth_ref( lithis, flags );

          if ( !cb->fun_counter && depth )
            cb->fun_top_depth = depth - 1;

          if ( cb && cb->fun_simple && !( flags & MASXFS_CB_USE_STAT_CB ) )
          {
            masxfs_scan_fun_simple_t fun_simple = cb->fun_simple;

            rC( fun_simple( lithis, tflags, userdata, cb->fun_counter, reldepth ) );
            fun_called++;
          }
          else if ( rGOOD && cb && cb->fun_stat && ( flags & MASXFS_CB_USE_STAT_CB ) )
          {
            masxfs_scan_fun_stat_t fun_stat = cb->fun_stat;
            struct stat *st = NULL;

            rC( masxfs_levinfo_stat( lithis, tflags, NULL /* entry_pfilter */ , &st /* stat */  ) );
            if ( rGOOD )
            {
#if 0
              if ( flags & MASXFS_CB_MODE_DB )
                st = lithis->db.stat;
              else if ( flags & MASXFS_CB_MODE_FS )
                st = lithis->fs.stat;
#endif
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

/* scan self, possibly UP_ROOT;
 *
 * called for nodes with parent only
 * */
static int
masxfs_levinfo_scanf_entry_single_internal_scanner( masxfs_levinfo_t * lithis, masxfs_scanner_t * scanner,
                                                    masxfs_levinfo_flags_t more_flags, masxfs_levinfo_flags_t no_flags, void *userdata,
                                                    masxfs_depth_t reldepth )
{
  rDECLGOOD;

/* lithis never NULL; surely has parent */
  if ( scanner )
  {
    masxfs_entry_filter_t *entry_pfilter = scanner->entry_pfilter;
    masxfs_levinfo_flags_t flags = ( scanner->flags | more_flags ) & ~no_flags;

    masxfs_entry_type_bit_t entry_bit = 1 << lithis->detype;

    if ( entry_pfilter && ( entry_pfilter->typeflags & entry_bit ) )
    {
      if ( lithis->lidepth )
      {
        int indx = ( flags & MASXFS_CB_COUNT ) ? 1 : 0;

        if ( flags & MASXFS_CB_COUNT )
        {
          if ( lithis->detype != MASXFS_ENTRY_DIR_NUM )
            lithis[-1].leaf_count++;
          else
            lithis[-1].leaf_count += lithis->leaf_count;
        }
        {
          if ( !( flags & MASXFS_CB_SKIP_EMPTY ) || lithis->leaf_count || lithis->detype != MASXFS_ENTRY_DIR_NUM )
            lithis[-1].child_count_pair[indx]++;
        }
      }

      for ( masxfs_entry_callback_t * cb = scanner->cbs; cb && rGOOD && ( flags & MASXFS_CB_SINGLE_CB ) && ( cb->fun_simple || cb->fun_stat ); cb++ )
      {
        if ( ( flags & MASXFS_CB_UP_ROOT ) && reldepth <= 1 )        /* Once! ; ==1 for dir ; ==0 for file at initial path */
        {
        /* Here when/for --store -- to register upper (no scanned) dirs */
          for ( masxfs_levinfo_t * li = lithis - lithis->lidepth; rGOOD && li < lithis; li++ )
          {
            assert( li->lidepth == ( masxfs_depth_t ) ( li - lithis + lithis->lidepth ) );
            li->detype = MASXFS_ENTRY_DIR_NUM;                       /* force : it's dir */
          /* scan self */
            rC( masxfs_levinfo_scanf_entry_single_internal_1cb( li, entry_pfilter, cb, userdata, flags & ~MASXFS_CB_UP_ROOT, li - lithis ) );
          }
        }

#if 0
	if ( lithis && lithis->lidepth > 0 && 0 == strcmp( lithis->name, "libsql.bash" ) && ( flags & MASXFS_CB_COUNT ) )
          WARN( "# ?? ##### %s / %s : %ld :: %d %p", lithis[-1].name, lithis->name, lithis[-1].leaf_count, ( flags & MASXFS_CB_COUNT ) ? 1 : 0, cb );
#endif
      /* scan self */
        rC( masxfs_levinfo_scanf_entry_single_internal_1cb( lithis, entry_pfilter, cb, userdata, flags & ~MASXFS_CB_UP_ROOT, reldepth ) );
      }
    }
  }
  rRET;
}

 /* scan self, possibly UP_ROOT, checked */
static int _uUu_
masxfs_levinfo_scanf_entry_single_scanner( masxfs_levinfo_t * li, masxfs_scanner_t * scanner, masxfs_levinfo_flags_t more_flags,
                                           masxfs_levinfo_flags_t no_flags, void *userdata, masxfs_depth_t reldepth )
{
  rDECLBAD;

  if ( li )
  {
    assert( li->lidepth );
  /* scan self, possibly UP_ROOT */
    if ( li->lidepth > 0 )                                           // if has parent
      rC( masxfs_levinfo_scanf_entry_single_internal_scanner( li, scanner, more_flags, no_flags, userdata, reldepth ) );
    else
      QRLIM( li, rCODE, "no parent" );
  }
  else
    QRLI( li, rCODE );
  rRET;
}

/* scan self & all childs, for closed */
static int
masxfs_levinfo_scanf_entry_scanner( masxfs_levinfo_t * li, masxfs_scanner_t * scanner, masxfs_levinfo_flags_t more_flags, void *userdata,
                                    masxfs_depth_t reldepth )
{
  rDECLBAD;
  masxfs_levinfo_flags_t flags = scanner->flags | more_flags;

  if ( li )
  {
    entry_scanner_cnt++;
    assert( !masxfs_levinfo_opened_dir( li, more_flags ) );
    assert( flags & ( MASXFS_CB_MODE_FS | MASXFS_CB_MODE_DB ) );
#if 0
    rC( masxfs_levinfo_opendir( li, flags, scanner->entry_pfilter ) );
#endif
    if ( li->detype == MASXFS_ENTRY_DIR_NUM )
    {
#if 0
    /* scan all childs, for open  */
      rC( masxfs_levinfo_scanf_dir_scanner( li, scanner, more_flags | MASXFS_CB_COUNT | MASXFS_CB_SKIP | MASXFS_CB_SINGLE_CB, userdata, reldepth ) );
#else
    /* scan all childs, for closed -- count only?! */
      rC( masxfs_levinfo_scanf_li_scanner( li, scanner, more_flags | MASXFS_CB_COUNT | MASXFS_CB_SKIP | MASXFS_CB_SINGLE_CB, userdata, reldepth ) );
#endif
      QRLI( li, rCODE );
    }
  /* scan self, possibly UP_ROOT, checked */
    rC( masxfs_levinfo_scanf_entry_single_scanner( li, scanner, more_flags, 0, userdata, reldepth ) );
    QRLI( li, rCODE );
    if ( li->detype == MASXFS_ENTRY_DIR_NUM )
    {
#if 0
    /* scan all childs, for open  */
      rC( masxfs_levinfo_scanf_dir_scanner( li, scanner, more_flags, userdata, reldepth ) );
#else
    /* scan all childs, for closed */
      rC( masxfs_levinfo_scanf_li_scanner( li, scanner, more_flags, userdata, reldepth ) );
#endif
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
#if 0
    rC( masxfs_levinfo_closedir_force( li, flags, rCODE ) );
#endif
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

/* scan one child, for open  */
int
masxfs_levinfo_scanf_down_scanner( masxfs_levinfo_t * li, masxfs_scanner_t * scanner, masxfs_levinfo_flags_t more_flags, void *userdata,
                                   masxfs_depth_t reldepth )
{
  rDECLBAD;

  masxfs_levinfo_flags_t flags = scanner->flags | more_flags;

  if ( li )
  {
    masxfs_entry_filter_t *entry_pfilter = scanner->entry_pfilter;
    masxfs_depth_t lidepth = li->lidepth;

    assert( masxfs_levinfo_opened_dir( li, flags ) );

    li++;
    lidepth++;
    reldepth++;
    {
      masxfs_entry_filter_t maxdepth_filter = {.maxdepth = scanner->maxdepth };

      if ( ( flags & MASXFS_CB_RECURSIVE ) && masxfs_levinfo_match_filter( li, &maxdepth_filter )
           && masxfs_levinfo_match_filter( li, entry_pfilter ) )
      {
/* scan self & all childs, for closed */
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

/* scan remaining childs, for open  */
static int
masxfs_levinfo_scanf_dir_remaining_scanner( masxfs_levinfo_t * li, masxfs_scanner_t * scanner, masxfs_levinfo_flags_t more_flags,
                                            void *userdata, masxfs_depth_t reldepth )
{
  rDECLBAD;
  int n = 0;
  int has_data = 0;

  if ( li )
  {
    rSETGOOD;
    masxfs_levinfo_flags_t flags = scanner->flags | more_flags;

    assert( masxfs_levinfo_opened_dir( li, flags ) );

/* TODO filter here ?! 20171229.122350 */
    while ( rGOOD && rC( masxfs_levinfo_readdir( li, scanner->entry_pfilter, flags, &has_data ) ) && has_data )
    {
    /* scan one child, for open  */
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

/* scan all childs, for open  */
static int
masxfs_levinfo_scanf_dir_scanner( masxfs_levinfo_t * li, masxfs_scanner_t * scanner, masxfs_levinfo_flags_t more_flags, void *userdata,
                                  masxfs_depth_t reldepth )
{
  rDECLBAD;
  masxfs_levinfo_flags_t flags = scanner->flags | more_flags;

  assert( masxfs_levinfo_opened_dir( li, flags ) );

/* assert( masxfs_levinfo_detype( li, flags ) == MASXFS_ENTRY_DIR_NUM ); */
  rC( masxfs_levinfo_rewinddir( li, flags ) );
/* scan remaining childs, for open  */
  rC( masxfs_levinfo_scanf_dir_remaining_scanner( li, scanner, more_flags, userdata, reldepth ) );
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
  /* masxfs_entry_callback_t *cb = scanner->cbs; */
    for ( masxfs_entry_callback_t * cb = scanner->cbs; rGOOD && ( flags & MASXFS_CB_SINGLE_CB ) && cb && ( cb->fun_simple || cb->fun_stat ); cb++ )
    {
      cb->fun_counter = 0;
#if 0
      if ( ( flags & MASXFS_CB_SINGLE_CB ) || !cb || ( !cb->fun_simple && !cb->fun_stat ) )
        break;
#endif
    }
    if ( li )
    {
#if 0
/*
    this `do' currently does ... what?
   TRY:
          run    --treefs mastest/  --no-empty-dirs --name='*.sh' --cb-up-root --empty-dirs    ## OK with `head` and self XXX
          run    --treefs mastest/  --no-empty-dirs --name='*.sh' --empty-dirs --cb-from-root --cb-up --cb-self    ## OK with `head` and self XXX
          run    --treefs mastest/  --no-empty-dirs --name='*.sh' --cb-from-root --cb-up --cb-self    ## OK with `head` and self XXX
          run    --treefs mastest/  --no-empty-dirs --name='*.sh' --empty-dirs --cb-from-root --cb-up ## OK with `head` but without (skipped) self?
          run    --treefs mastest/  --no-empty-dirs --name='*.sh' --cb-up-root                        ## OK, without head, but with self

          run    --treefs mastest/  --no-empty-dirs --name='*.sh' --cb-up-root  ## OK, but FIXME without `head`, with self XXX
          run    --treefs mastest/  --no-empty-dirs --name='*.sh'   --cb-up-root   --cb-self ## XXX BAD - FIXME

         Attention  :  	  --cb-up-root with  --cb-from-root  breaks!
          run    --treefs mastest/  --no-empty-dirs --name='*.sh' --cb-up  --cb-from-root --cb-up-root --empty-dirs # XXX multi-headed - useless?
          run    --treefs mastest/  --no-empty-dirs --name='*.sh' --cb-up-root  --cb-up --cb-from-root ## XXX BADLY - FIXME
          run    --treefs mastest/  --no-empty-dirs --name='*.sh' --cb-up-root --empty-dirs --cb-from-root --cb-self    ## XXX double-headed!! XXX
*/
      do
      {
        if ( li->lidepth )
        {
        /* scan self, possibly UP_ROOT, checked */
          rC( masxfs_levinfo_scanf_entry_single_scanner
              ( li, scanner, MASXFS_CB_COUNT | MASXFS_CB_SKIP | MASXFS_CB_SINGLE_CB, MASXFS_CB_SKIP_EMPTY, userdata, reldepth ) );
          QRLI( li, rCODE );

        // XXX XXX
          masxfs_levinfo_flags_t xflags = ( flags & ( reldepth == 0 ? MASXFS_CB_SELF : MASXFS_CB_UP ) );

# if 0
          WARN( "%lx : %lx - %s %s %s - %s rd:%d", flags, xflags, flags & MASXFS_CB_SELF ? "SELF" : "", flags & MASXFS_CB_UP ? "UP" :
                "", flags & MASXFS_CB_FROM_ROOT ? "FROM_ROOT" : "", li->name, reldepth );
# endif
        /* scan self, possibly UP_ROOT, checked */
          rC( masxfs_levinfo_scanf_entry_single_scanner( li, scanner, xflags ? 0 : MASXFS_CB_SKIP, MASXFS_CB_SKIP_EMPTY, userdata, reldepth ) );
          QRLI( li, rCODE );
        }
        if ( reldepth <= 0 )
        {
          li++;
          reldepth++;
        }
      } while ( reldepth <= 0 );
      WARN( "==================================================" );
      reldepth--;
      li--;
#endif

#if 0
/* scan all childs, for open  */
      rC( masxfs_levinfo_scanf_dir_scanner( li, scanner, 0 | MASXFS_CB_COUNT | MASXFS_CB_SKIP | MASXFS_CB_SINGLE_CB, userdata, reldepth ) );
/* scan all childs, for open  */
      rC( masxfs_levinfo_scanf_dir_scanner( li, scanner, 0, userdata, reldepth ) );
#elif 0
/* scan all childs, for closed */
      rC( masxfs_levinfo_scanf_li_scanner( li, scanner, 0 | MASXFS_CB_COUNT | MASXFS_CB_SKIP | MASXFS_CB_SINGLE_CB, userdata, reldepth ) );
/* scan all childs, for closed */
      rC( masxfs_levinfo_scanf_li_scanner( li, scanner, 0, userdata, reldepth ) );
#else
      rC( masxfs_levinfo_scanf_entry_scanner( li, scanner, 0, userdata, reldepth ) );
#endif
    /* *li->child_count_pair={0}; */
      memset( li->child_count_pair, 0, sizeof( li->child_count_pair ) );
      WARN( "entry_scanner_cnt:%u", entry_scanner_cnt );
    }
    else
      QRLI( li, rCODE );
  }
  else
    QRG( rCODE );
  rRET;
}
