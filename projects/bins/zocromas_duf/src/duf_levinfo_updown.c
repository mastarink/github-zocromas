#include <string.h>
#include <assert.h>
#include <limits.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"

#include "duf_pdi.h"

#include "duf_levinfo.h"
#include "duf_levinfo_ref.h"
#include "duf_levinfo_openclose.h"

/* ###################################################################### */
#include "duf_levinfo_updown.h"
/* ###################################################################### */



static void
duf_levinfo_countdown_dirs( duf_depthinfo_t * pdi )
{
  /* assert( pdi );                */
  /* assert( pdi->levinfo );       */
  /* {                             */
  /*   int d = pdi->depth - 1;     */
  /*                               */
  /*   if ( d >= 0 )               */
  /*     pdi->levinfo[d].numdir--; */
  /* }                             */
  duf_levinfo_t *up;

  up = duf_levinfo_ptr_up( pdi );
  if ( up )
    up->numdir--;
}

int
duf_levinfo_godown( duf_depthinfo_t * pdi, unsigned long long dirid, const char *itemname, unsigned long long ndirs,
                    unsigned long long nfiles, int is_leaf )
{
  int r = 0;

  assert( pdi );
  {
    int d;

    d = ++pdi->depth;
    /* assume only level 0 may have dirid==0 -- AND: simply dirid not set */
    /* assert( d == 0 || ( d > 0 && dirid  ) ); */

    /*!! if ( d <= pdi->maxdepth ) */
    {
      assert( pdi->levinfo );
      assert( d >= 0 );
      memset( &pdi->levinfo[d], 0, sizeof( pdi->levinfo[d] ) );
      pdi->levinfo[d].lev_dh.dirid = dirid;

      pdi->levinfo[d].dirid = dirid;
      pdi->levinfo[d].numdir = ndirs;
      pdi->levinfo[d].numfile = nfiles;
      if ( itemname )
      {
        /* DUF_SHOW_ERROR( "BEFORE NEW LEVEL %d %s %p", d, pdi->levinfo[d].itemname, pdi->levinfo[d].itemname ); */
        pdi->levinfo[d].itemname = mas_strdup( itemname );
        /* DUF_SHOW_ERROR( "NEW LEVEL %d %s %p", d, pdi->levinfo[d].itemname, pdi->levinfo[d].itemname ); */
      }
      pdi->levinfo[d].is_leaf = is_leaf ? 1 : 0;
      DUF_TRACE( explain, 1, "level down: %d; ≪%s≫  [%s]", d, is_leaf ? "leaf" : "node", duf_levinfo_itemname( pdi ) );
      assert( pdi->depth >= 0 );
      if ( is_leaf )
        DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan leaf    =>           - %s", duf_pdi_depth( pdi ), duf_levinfo_itemname( pdi ) );
      else
      {
        duf_levinfo_countdown_dirs( pdi );
        DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": scan node:   =>  by %5llu - %s", duf_pdi_depth( pdi ), duf_levinfo_dirid( pdi ),
                   duf_levinfo_itemname( pdi ) );
      }
    }
    /* else                       */
    /* {                          */
    /*   pdi->depth--;            */
    /*   DUF_MAKE_ERROR( r, DUF_ERROR_MAX_DEPTH); */
    /* }                          */
    /* assert( duf_pdi_depth( pdi ) == 0 || ( duf_pdi_depth( pdi ) > 0 && duf_levinfo_dirid( pdi ) ) ); */
  }
  return r;
}

static int
duf_levinfo_check_depth( const duf_depthinfo_t * pdi, int is_leaf )
{
  DEBUG_STARTR( r );
  int delta;

  delta = ( is_leaf ? 1 : 0 );
  delta = 0;
  /* if ( duf_pdi_recursive( pdi ) )               */
  /* {                                             */
  if ( duf_pdi_depth( pdi ) > duf_pdi_maxdepth( pdi ) )
    DUF_MAKE_ERROR( r, DUF_ERROR_LEVINFO_SIZE );
  else if ( !duf_pdi_is_good_depth( pdi, delta ) )
    DUF_MAKE_ERROR( r, DUF_ERROR_TOO_DEEP );

  if ( r < 0 )
    DUF_TRACE( depth, 0, "(%d) DEPTH: d=%d; max:%d; top:%d; delta:%d; R:%d; ", r, duf_pdi_depth( pdi ), duf_pdi_maxdepth( pdi ),
               duf_pdi_topdepth( pdi ), delta, duf_pdi_recursive( pdi ) );
  /* }                                             */
  /* else if ( duf_pdi_reldepth( pdi ) > delta )   */
  /*   DUF_MAKE_ERROR( r, DUF_ERROR_MAX_DEPTH);    */
  DEBUG_ENDR( r );
}

int
duf_levinfo_godown_openat_dh( duf_depthinfo_t * pdi, unsigned long long dirid, const char *itemname, unsigned long long ndirs,
                              unsigned long long nfiles, int is_leaf )
{
  DEBUG_STARTR( r );
  int rd;

  DOR( r, duf_levinfo_check_depth( pdi, is_leaf ) );
  if ( r >= 0 )
  {
    /* DORQ( r, duf_levinfo_godown( pdi, dirid, itemname, ndirs, nfiles, is_leaf ), r == DUF_ERROR_TOO_DEEP ); */
    DOR_NOE( r, duf_levinfo_godown( pdi, dirid, itemname, ndirs, nfiles, is_leaf ), DUF_ERROR_TOO_DEEP );
    rd = r;
    DOR( r, duf_levinfo_openat_dh( pdi ) );
    DUF_TRACE( fs, 0, "(%d)? levinfo u/d openated %s; dfd:%d", r, duf_levinfo_itemname( pdi ),  duf_levinfo_dfd(pdi) );
    if ( r < 0 && rd >= 0 )
      duf_levinfo_goup( pdi );
  }
  /* DEBUG_ENDRQ( r, r == DUF_ERROR_TOO_DEEP ); */
  DEBUG_ENDR_NOE( r, DUF_ERROR_TOO_DEEP );
}

int
duf_levinfo_goup( duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  assert( pdi );

  if ( duf_levinfo_is_leaf( pdi ) )
    DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan leaf  <=             - %s", duf_pdi_depth( pdi ), duf_levinfo_itemname( pdi ) );
  else
    DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": scan node: <=    by %5llu - %s", duf_pdi_depth( pdi ), duf_levinfo_dirid( pdi ),
               duf_levinfo_itemname( pdi ) );
  {
    int d = pdi->depth--;

    if ( duf_levinfo_opened_dh_d( pdi, d ) > 0 )
      DOR( r, duf_levinfo_closeat_dh_d( pdi, d ) );

    assert( pdi->levinfo[d].lev_dh.dfd == 0 );

    DUF_TEST_R( r );
    if ( r < 0 )
      DUF_SHOW_ERROR( "(%d) close error; L%d", r, pdi->depth );
    DUF_TRACE( explain, 1, "level up:   %d", d );
    assert( pdi->levinfo );
    duf_levinfo_clear_level_d( pdi, d );
    d = pdi->depth;
  }
  DEBUG_ENDR( r );
}
