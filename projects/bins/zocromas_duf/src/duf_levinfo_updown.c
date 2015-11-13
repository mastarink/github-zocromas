#include "duf_maintenance.h"

#include "duf_pdi_ref.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"

#include "duf_levinfo.h"
#include "duf_levinfo_init.h"
#include "duf_levinfo_dbinit.h"
#include "duf_levinfo_ref.h"
#include "duf_levinfo_openclose.h"



#include "duf_pstmt_levinfo.h"

#include "duf_sccb_scanstage.h" /* duf_nodetype_name, temporarily */
/* ###################################################################### */
#include "duf_levinfo_updown.h"
/* ###################################################################### */

/* 20151026.104307 */
/* may change levinfo (for upper level) */
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
  {
#ifndef DUF_NO_NUMS
    up->numdir--;
    /* T( "@@@numdir:%ld => %ld", up->numdir + 1, up->numdir ); */
#else
    up->numchild++;
    /* T( "@@@{%p:%p} numchild:%lld => %lld - %s", pdi, up, up->numchild - 1, up->numchild, duf_levinfo_path_up( pdi ) ); */
#endif
  }
}

/* no side effects */
static int
duf_levinfo_check_depth( const duf_depthinfo_t * pdi, duf_node_type_t node_type )
{
  DEBUG_STARTR( r );
  assert( pdi );
  {
    int delta;
    int rgd = 0;

    delta = ( node_type == DUF_NODE_LEAF ? 1 : 0 );
    delta = 0;
    /* if ( duf_pdi_recursive( pdi ) )               */
    /* {                                             */
    if ( duf_pdi_depth( pdi ) > duf_pdi_maxdepth( pdi ) )
    {
      DUF_MAKE_ERROR( r, DUF_ERROR_LEVINFO_SIZE );
    }
    else if ( !( rgd = duf_pdi_is_good_depth( pdi, delta ) ) )
    {
      DUF_MAKE_ERRORM( r, DUF_ERROR_TOO_DEEP, "depth(%d); maxdepth(%d);rgd:%d dd:%d; delta:%d", duf_pdi_depth( pdi ), duf_pdi_maxdepth( pdi ), rgd,
                       duf_pdi_reldepth( pdi ), delta );
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

/* 20151026.104256 */
/* check depth; may change levinfo (for upper level) via duf_levinfo_countdown_dirs */
static int
_duf_levinfo_godown( duf_depthinfo_t * pdi, const char *itemname DUF_UNUSED, duf_node_type_t node_type )
{
  DEBUG_STARTR( r );
  assert( pdi );

  assert( pdi->pathinfo.depth == duf_levinfo_calc_depth( pdi ) );

  DOR( r, duf_levinfo_check_depth( pdi, node_type ) ); /* no side effects */
  if ( DUF_NOERROR( r ) )
  {
    int d;

    d = ++pdi->pathinfo.depth;
    assert( d >= 0 );
    assert( pdi->pathinfo.depth >= 0 );
    assert( d == pdi->pathinfo.depth );

    assert( pdi->pathinfo.levinfo );

    DUF_TRACE( explain, 2, "level down: %d; ≪%s≫  [%s]", d, duf_nodetype_name( node_type ), duf_levinfo_itemshowname( pdi ) );
    if ( node_type == DUF_NODE_LEAF )
      DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan leaf    =>           - %s", duf_pdi_depth( pdi ), duf_levinfo_itemshowname( pdi ) );
    else
    {
      DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": scan node:   =>  by %5llu - %s", duf_pdi_depth( pdi ), duf_levinfo_dirid( pdi ),
                 duf_levinfo_itemshowname( pdi ) );
    }
    if ( node_type == DUF_NODE_NODE )
      duf_levinfo_countdown_dirs( pdi ); /* may change levinfo (for upper level) */
    assert( pdi->pathinfo.depth - 1 /* not yet ... */  == duf_levinfo_calc_depth( pdi ) );
  }

  DEBUG_ENDR( r );
}

/* 20150901.173329 */
/* check depth; resets levinfo  (currenl level) with dirid,nfiles,ndirs; may change levinfo (for upper level) */
static int
duf_levinfo_godown_dirid( duf_depthinfo_t * pdi, const char *itemname, unsigned long long dirid, duf_node_type_t node_type )
{
  DEBUG_STARTR( r );
  assert( pdi );

  DOR( r, _duf_levinfo_godown( pdi, itemname, node_type ) ); /* check depth; may change levinfo (for upper level) via duf_levinfo_countdown_dirs */
  if ( DUF_NOERROR( r ) )
    duf_levinfo_init_level( pdi, itemname, dirid, node_type ); /* resets levinfo (currenl level) */


  assert( pdi->pathinfo.depth == duf_levinfo_calc_depth( pdi ) );

  DEBUG_ENDR( r );
}

/* 20151026.104757 */
/* check depth; resets levinfo  (currenl level) without dirid,nfiles,ndirs; may change levinfo (for upper level) */
int
duf_levinfo_godown( duf_depthinfo_t * pdi, const char *itemname, duf_node_type_t node_type )
{
  DEBUG_STARTR( r );
#if 0
  assert( pdi );

  DOR( r, _duf_levinfo_godown( pdi, itemname, node_type ) ); /* check depth; may change levinfo (for upper level) via duf_levinfo_countdown_dirs */
  if ( DUF_NOERROR( r ) )
    duf_levinfo_init_level( pdi, itemname, 0, 0, 0, node_type ); /* resets levinfo  (currenl level) */
#else
  DOR( r, duf_levinfo_godown_dirid( pdi, itemname, 0 /* dirid */ ,
                                    node_type ) );
#endif

  assert( pdi->pathinfo.depth == duf_levinfo_calc_depth( pdi ) );

  DEBUG_ENDR( r );
}

/* 20150904.120835 */
static int
duf_levinfo_godown_db( duf_depthinfo_t * pdi, duf_node_type_t node_type, duf_stmnt_t * pstmt )
{
  DEBUG_STARTR( r );
  assert( pdi );

  DOR( r, duf_levinfo_check_depth( pdi, node_type ) );
  DUF_TRACE( temp, 5, "@@(%s) maxdepth:%d;", mas_error_name_i( r ), duf_pdi_maxdepth( pdi ) );
  if ( DUF_NOERROR( r ) )
  {
    int d;


    assert( pdi->pathinfo.depth == duf_levinfo_calc_depth( pdi ) );

    d = ++pdi->pathinfo.depth;
    assert( d >= 0 );
    assert( pdi->pathinfo.depth >= 0 );
    assert( d == pdi->pathinfo.depth );
    /* assume only level 0 may have dirid==0 -- AND: simply dirid not set */
    /* assert( d == 0 || ( d > 0 && dirid  ) ); */

    /*!! if ( d <= pdi->maxdepth ) */
    assert( pdi->pathinfo.levinfo );


    DUF_TRACE( explain, 2, "level down: %d; ≪%s≫  [%s]", d, duf_nodetype_name( node_type ), duf_levinfo_itemshowname( pdi ) );
    if ( node_type == DUF_NODE_NODE )
      duf_levinfo_countdown_dirs( pdi ); /* may change levinfo (for upper level) */
    /* ------------------------------------------- */
    assert( duf_levinfo_dirid( pdi ) == 0 );
    assert( !pdi->pathinfo.levinfo[d].itemname );
    duf_levinfo_dbinit_level_d( pdi, pstmt, node_type, d );
    assert( pdi->pathinfo.levinfo[d].itemname );

    assert( duf_levinfo_dirid( pdi ) != 0 );
    assert( d == pdi->pathinfo.depth );

    assert( pdi->pathinfo.depth == duf_levinfo_calc_depth( pdi ) );
  }
  DEBUG_ENDR( r );
}

/* 20151026.105741 */
/* check depth; resets levinfo  (currenl level); may change levinfo (for upper level)
 * does not really open dh?!
 * XXX equal XXX to duf_levinfo_godown - except DUF_ERROR_TOO_DEEP ? XXX
 */
int
duf_levinfo_godown_openat_dh( duf_depthinfo_t * pdi, const char *itemname, duf_node_type_t node_type )
{
  DEBUG_STARTR( r );
  assert( pdi );
#if 0
  DOR_LOWERE( r, duf_levinfo_godown_dirid( pdi, itemname, 0, node_type ), DUF_ERROR_TOO_DEEP ); /* check depth; resets levinfo  (currenl level)
                                                                                                   with dirid,nfiles,ndirs;
                                                                                                   may change levinfo (for upper level) */
#else
  DOR_LOWERE( r, duf_levinfo_godown( pdi, itemname, node_type ), DUF_ERROR_TOO_DEEP ); /* check depth; resets levinfo  (currenl level);
                                                                                          may change levinfo (for upper level) */
#endif
  DEBUG_ENDR_LOWERE( r, DUF_ERROR_TOO_DEEP );
}

/* 20150904.120814 */
/* XXX equal to duf_levinfo_godown_db - except DUF_ERROR_TOO_DEEP ? XXX */
int
duf_levinfo_godown_dbopenat_dh( duf_depthinfo_t * pdi, duf_node_type_t node_type, duf_stmnt_t * pstmt )
{
  DEBUG_STARTR( r );
  assert( pdi );

  DOR_LOWERE( r, duf_levinfo_godown_db( pdi, node_type, pstmt ), DUF_ERROR_TOO_DEEP );
  assert( pdi->pathinfo.depth == duf_levinfo_calc_depth( pdi ) );

  DEBUG_ENDR_LOWERE( r, DUF_ERROR_TOO_DEEP );
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

    assert( pdi->pathinfo.depth == duf_levinfo_calc_depth( pdi ) );


    d = pdi->pathinfo.depth--;

    assert( d >= 0 );
    if ( duf_levinfo_opened_dh_d( pdi, d ) > 0 )
      DOR( r, duf_levinfo_closeat_dh_d( pdi, d ) );

    assert( !duf_levinfo_opened_here_dh_d( pdi, d ) || pdi->pathinfo.levinfo[d].lev_dh.dfd == 0 );

    /* DUF_TEST_R( r ); */
    if ( DUF_IS_ERROR( r ) )
      DUF_SHOW_ERRORO( "(%d) close error; L%d", r, pdi->pathinfo.depth );
    DUF_TRACE( explain, 2, "level up:   %d", d );
    assert( pdi->pathinfo.levinfo );
    duf_levinfo_clear_level_d( pdi, d );
    d = pdi->pathinfo.depth;
    /* Not here: assert( d >= 0 ); */
  }
  DEBUG_ENDR( r );
}
