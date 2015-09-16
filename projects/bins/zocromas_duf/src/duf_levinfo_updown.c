/* #include <string.h> */
#include <assert.h>


#include <mastar/wrap/mas_std_def.h>
/* #include <mastar/wrap/mas_memory.h> */

#include "duf_maintenance.h"

/* #include "duf_pdi.h" */
#include "duf_pdi_ref.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"

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
  /* assert( pdi->pathinfo.levinfo );       */
  /* {                             */
  /*   int d = pdi->pathinfo.depth - 1;     */
  /*                               */
  /*   if ( d >= 0 )               */
  /*     pdi->pathinfo.levinfo[d].numdir--; */
  /* }                             */
  duf_levinfo_t *up;

  up = duf_levinfo_ptr_up( pdi );
  if ( up )
    up->numdir--;
}

static int
duf_levinfo_check_depth( const duf_depthinfo_t * pdi, int is_leaf )
{
  DEBUG_STARTR( r );
  assert( pdi );
  {
    int delta;
    int rgd = 0;

    delta = ( is_leaf ? 1 : 0 );
    delta = 0;
    /* if ( duf_pdi_recursive( pdi ) )               */
    /* {                                             */
    if ( duf_pdi_depth( pdi ) > duf_pdi_maxdepth( pdi ) )
    {
      DUF_MAKE_ERROR( r, DUF_ERROR_LEVINFO_SIZE );
    }
    else if ( !( rgd = duf_pdi_is_good_depth( pdi, delta ) ) )
    {
      DUF_MAKE_ERROR( r, DUF_ERROR_TOO_DEEP );
    }

    if ( !DUF_NOERROR( r ) )
      DUF_TRACE( depth, 0, "(%d) DEPTH: d=%d; max:%d; top:%d; delta:%d; R:%d; ", r, duf_pdi_depth( pdi ), duf_pdi_maxdepth( pdi ),
                 duf_pdi_topdepth( pdi ), delta, duf_pdi_recursive( pdi ) );
    /* }                                             */
    /* else if ( duf_pdi_reldepth( pdi ) > delta )   */
    /*   DUF_MAKE_ERROR( r, DUF_ERROR_MAX_DEPTH);    */
  }
  DEBUG_ENDR( r );
}

static int
_duf_levinfo_godown( duf_depthinfo_t * pdi, const char *itemname, int is_leaf )
{
  DEBUG_STARTR( r );
  assert( pdi );

  DOR( r, duf_levinfo_check_depth( pdi, is_leaf ) );
  if ( DUF_NOERROR( r ) )
  {
    int d;

    d = ++pdi->pathinfo.depth;
    assert( d >= 0 );
    assert( pdi->pathinfo.depth >= 0 );
    assert( d == pdi->pathinfo.depth );

    assert( pdi->pathinfo.levinfo );

    DUF_TRACE( explain, 2, "level down: %d; ≪%s≫  [%s]", d, is_leaf ? "leaf" : "node", duf_levinfo_itemshowname( pdi ) );
    if ( is_leaf )
      DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan leaf    =>           - %s", duf_pdi_depth( pdi ), duf_levinfo_itemshowname( pdi ) );
    else
    {
      duf_levinfo_countdown_dirs( pdi );
      DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": scan node:   =>  by %5llu - %s", duf_pdi_depth( pdi ), duf_levinfo_dirid( pdi ),
                 duf_levinfo_itemshowname( pdi ) );
    }
  }

  DEBUG_ENDR( r );
}

/* 20150901.173329 */
static int
duf_levinfo_godown_dnn( duf_depthinfo_t * pdi, unsigned long long dirid, const char *itemname, unsigned long long ndirs,
                        unsigned long long nfiles, int is_leaf )
{
  DEBUG_STARTR( r );
  assert( pdi );

  DOR( r, _duf_levinfo_godown( pdi, itemname, is_leaf ) );
  if ( DUF_NOERROR( r ) )
    duf_levinfo_init_level( pdi, itemname, dirid, ndirs, nfiles, is_leaf );
  DEBUG_ENDR( r );
}

int
duf_levinfo_godown( duf_depthinfo_t * pdi, const char *itemname, int is_leaf )
{
  DEBUG_STARTR( r );
  assert( pdi );

  DOR( r, _duf_levinfo_godown( pdi, itemname, is_leaf ) );
  if ( DUF_NOERROR( r ) )
    duf_levinfo_init_level( pdi, itemname, 0, 0, 0, is_leaf );

  DEBUG_ENDR( r );
}

/* 20150904.120835 */
static int
duf_levinfo_godown_db( duf_depthinfo_t * pdi, int is_leaf, duf_stmnt_t * pstmt )
{
  DEBUG_STARTR( r );
  assert( pdi );

  DOR( r, duf_levinfo_check_depth( pdi, is_leaf ) );
  DUF_TRACE( temp, 5, "@@(%s) maxdepth:%d;", duf_error_name( r ), duf_pdi_maxdepth( pdi ) );
  if ( DUF_NOERROR( r ) )
  {
    int d;

    d = ++pdi->pathinfo.depth;
    assert( d >= 0 );
    assert( pdi->pathinfo.depth >= 0 );
    assert( d == pdi->pathinfo.depth );
    /* assume only level 0 may have dirid==0 -- AND: simply dirid not set */
    /* assert( d == 0 || ( d > 0 && dirid  ) ); */

    /*!! if ( d <= pdi->maxdepth ) */
    assert( pdi->pathinfo.levinfo );


    DUF_TRACE( explain, 2, "level down: %d; ≪%s≫  [%s]", d, is_leaf ? "leaf" : "node", duf_levinfo_itemshowname( pdi ) );
    if ( is_leaf )
      DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan leaf    =>           - %s", duf_pdi_depth( pdi ), duf_levinfo_itemshowname( pdi ) );
    else
    {
      duf_levinfo_countdown_dirs( pdi );
      DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": scan node:   =>  by %5llu - %s", duf_pdi_depth( pdi ), duf_levinfo_dirid( pdi ),
                 duf_levinfo_itemshowname( pdi ) );
    }
    /* ------------------------------------------- */
    assert( duf_levinfo_dirid( pdi ) == 0 );
    assert( !pdi->pathinfo.levinfo[d].itemname );
    DOR( r, duf_levinfo_dbinit_level_d( pdi, pstmt, is_leaf, d ) );
    assert( duf_levinfo_dirid( pdi ) != 0 );
  }
  DEBUG_ENDR( r );
}

/*
 * 1. check depth
 * 2. duf_levinfo_godown_dnn
 * 2.1. check depth
 * */
/* XXX equal to duf_levinfo_godown ? XXX */
/* 20150904.120828 */
int
duf_levinfo_godown_openat_dh( duf_depthinfo_t * pdi, const char *itemname, int is_leaf )
{
  DEBUG_STARTR( r );
  assert( pdi );

  DOR_NOE( r, duf_levinfo_godown_dnn( pdi, 0, itemname, 0, 0, is_leaf ), DUF_ERROR_TOO_DEEP );

  DEBUG_ENDR_NOE( r, DUF_ERROR_TOO_DEEP );
}

/* XXX equal to duf_levinfo_godown_db ? XXX */
/* 20150904.120814 */
int
duf_levinfo_godown_dbopenat_dh( duf_depthinfo_t * pdi, int is_leaf, duf_stmnt_t * pstmt )
{
  DEBUG_STARTR( r );
  assert( pdi );

  DOR_NOE( r, duf_levinfo_godown_db( pdi, is_leaf, pstmt ), DUF_ERROR_TOO_DEEP );
  DEBUG_ENDR_NOE( r, DUF_ERROR_TOO_DEEP );
}

int
duf_levinfo_goup( duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  assert( pdi );

  if ( duf_levinfo_is_leaf( pdi ) )
    DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan leaf  <=             - %s", duf_pdi_depth( pdi ), duf_levinfo_itemshowname( pdi ) );
  else
    DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": scan node: <=    by %5llu - %s", duf_pdi_depth( pdi ), duf_levinfo_dirid( pdi ),
               duf_levinfo_itemshowname( pdi ) );
  {
    int d;

    d = pdi->pathinfo.depth--;

    assert( d >= 0 );
    if ( duf_levinfo_opened_dh_d( pdi, d ) > 0 )
      DOR( r, duf_levinfo_closeat_dh_d( pdi, d ) );

    assert( !duf_levinfo_opened_here_dh_d( pdi, d ) || pdi->pathinfo.levinfo[d].lev_dh.dfd == 0 );

    DUF_TEST_R( r );
    if ( !DUF_NOERROR( r ) )
      DUF_SHOW_ERROR( "(%d) close error; L%d", r, pdi->pathinfo.depth );
    DUF_TRACE( explain, 2, "level up:   %d", d );
    assert( pdi->pathinfo.levinfo );
    duf_levinfo_clear_level_d( pdi, d );
    d = pdi->pathinfo.depth;
    /* Not here: assert( d >= 0 ); */
  }
  DEBUG_ENDR( r );
}
