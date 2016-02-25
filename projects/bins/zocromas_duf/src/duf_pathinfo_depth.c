/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs.h"                                         /* DUF_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_fmt_defs.h"

#include "duf_config.h"                                              /* duf_get_config ♠ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */

#include "duf_levinfo_ref_def.h"
#include "duf_levinfo_context.h"
#include "duf_levinfo_credel.h"

#include "duf_li_credel.h"
#include "duf_li.h"

#include "duf_dh.h"

#include "duf_pathinfo_credel.h"
#include "duf_pathinfo_ref.h"
#include "duf_sccb_scanstage.h"

#include "duf_pathinfo_ref_def.h"
/* ###################################################################### */
#include "duf_pathinfo_depth.h"
/* ###################################################################### */

int
duf_pi_calc_depth( duf_pathinfo_t * pi )
{
  int d;

  assert( pi );
  d = pi->levinfo ? duf_li_calc_depth( pi->levinfo ) : 0;
  return d;
}

int
duf_pi_check_depth( const duf_pathinfo_t * pi, duf_node_type_t node_type, unsigned frecursive, unsigned flinear )
{
  DUF_STARTR( r );
  assert( pi );
  {
    int delta;
    int rgd = 0;

  /* T( "@%d: check depth #%llu: %s - %llu", duf_pdi_depth(pdi), duf_levinfo_dirid( pdi ), duf_levinfo_path( pdi ), duf_levinfo_nameid( pdi ) ); */
    DUF_TRACE( levinfo, 30, "@%d: check depth  %s -  %s", duf_pi_depth( pi ), duf_pi_path( pi ), duf_pi_itemtruename( pi ) );

    delta = ( node_type == DUF_NODE_LEAF ? 1 : 0 );
    delta = 0;

  /* if ( duf_pdi_recursive( pdi ) )               */
  /* {                                             */
    if ( duf_pi_depth( pi ) > duf_pi_maxdepth( pi ) )
    {
      DUF_MAKE_ERROR( r, DUF_ERROR_LEVINFO_SIZE );
    }
    else if ( !( rgd = duf_pi_is_good_depth( pi, delta, frecursive, flinear ) ) )
    {
      DUF_MAKE_ERRORM( r, DUF_ERROR_TOO_DEEP, "depth(%d); maxdepth(%d);rgd:%d dd:%d; delta:%d", duf_pi_depth( pi ), duf_pi_maxdepth( pi ), rgd,
                       duf_pi_deltadepth( pi ), delta );
    }

    if ( !DUF_NOERROR( r ) )
    {
      DUF_TRACE( depth, 0, "(%d) DEPTH: d=%d; max:%d; top:%d; delta:%d; R:%d; ", r, duf_pi_depth( pi ), duf_pi_maxdepth( pi ),
                 duf_pi_topdepth( pi ), delta, frecursive );
    }
  /* }                                             */
  /* else if ( duf_pdi_reldepth( pdi ) > delta )   */
  /*   DUF_MAKE_ERROR( r, DUF_ERROR_MAX_DEPTH);    */
  }
  DUF_ENDR( r );
}

static void
duf_pi_levinfo_countdown_dirs( duf_pathinfo_t * pi )
{
  duf_levinfo_t *up;

  up = duf_pi_ptr_up( pi );
  if ( up )
  {
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
    up->numdir--;
  /* T( "@@@numdir:%ld => %ld", up->numdir + 1, up->numdir ); */
#else
    up->numchild++;
  /* T( "@@@{%p:%p} numchild:%lld => %lld - %s", pdi, up, up->numchild - 1, up->numchild, duf_levinfo_path_up( pdi ) ); */
#endif
  }
}

int
duf_pi_godown( duf_pathinfo_t * pi, duf_node_type_t node_type, unsigned frecursive, unsigned flinear )
{
  DUF_STARTR( r );
  assert( pi );

  assert( pi->depth == duf_pi_calc_depth( pi ) );

  DOR( r, duf_pi_check_depth( pi, node_type, frecursive, flinear ) ); /* no side effects */
  if ( DUF_NOERROR( r ) )
  {
    int d;

    d = ++pi->depth;
    assert( d >= 0 );
    assert( pi->depth >= 0 );
    assert( d == pi->depth );

    assert( pi->levinfo );

    DUF_TRACE( explain, 2, "level down: %d; ≪%s≫  [%s]", d, duf_nodetype_name( node_type ), duf_pi_itemshowname( pi ) );
    if ( node_type == DUF_NODE_LEAF )
      DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan leaf    =>           - %s", duf_pi_depth( pi ), duf_pi_itemshowname( pi ) );
    else
    {
      DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": scan node:   =>  by %s", duf_pi_depth( pi ), duf_pi_itemshowname( pi ) );
    }
    if ( node_type == DUF_NODE_NODE )
      duf_pi_levinfo_countdown_dirs( pi );                           /* may change levinfo (for upper level) */
    assert( pi->depth - 1 /* not yet ... */  == duf_pi_calc_depth( pi ) );
  }

  DUF_ENDR( r );
}

int
duf_pi_godown_db( duf_pathinfo_t * pi, duf_node_type_t node_type, duf_stmnt_t * pstmt, unsigned frecursive, unsigned flinear )
{
  DUF_STARTR( r );
  assert( pi );

  DOR( r, duf_pi_check_depth( pi, node_type, frecursive, flinear ) );
  DUF_TRACE( levinfo, 5, "@@(%s) maxdepth:%d;", mas_error_name_i( r ), duf_pi_maxdepth( pi ) );
  if ( DUF_NOERROR( r ) )
  {
    int d;

    assert( pi->depth == duf_pi_calc_depth( pi ) );

    d = ++pi->depth;
    assert( d >= 0 );
    assert( pi->depth >= 0 );
    assert( d == pi->depth );

    assert( pi->levinfo );

    if ( node_type == DUF_NODE_NODE )
      duf_pi_countdown_dirs( pi );                                   /* may change levinfo (for upper level) */
  /* ------------------------------------------- */
  /* assert( duf_levinfo_dirid( pdi ) == 0 ); */
    assert( !pi->levinfo[d].itemname );
    duf_pi_dbinit_level_d( pi, pstmt, node_type, d );
    assert( pi->levinfo[d].itemname );

  /* assert( duf_levinfo_dirid( pdi ) != 0 ); */
    assert( d == pi->depth );

    assert( pi->depth == duf_pi_calc_depth( pi ) );
    DUF_TRACE( explain, 2, "level down: %d; ≪%s≫  [%s]", d, duf_nodetype_name( node_type ), duf_pi_itemshowname( pi ) );
  }
  DUF_ENDR( r );
}

void
duf_pi_countdown_dirs( duf_pathinfo_t * pi )
{
  duf_levinfo_t *up;

  up = duf_pi_ptr_up( pi );
  if ( up )
  {
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
    up->numdir--;
  /* T( "@@@numdir:%ld => %ld", up->numdir + 1, up->numdir ); */
#else
    up->numchild++;
  /* T( "@@@{%p:%p} numchild:%lld => %lld - %s", pdi, up, up->numchild - 1, up->numchild, duf_levinfo_path_up( pdi ) ); */
#endif
  }
}

int
duf_pi_is_good_depth_d( const duf_pathinfo_t * pi, int delta, unsigned frecursive, unsigned flinear, int d )
{
  int rd = 0;

  assert( frecursive == 1 || frecursive == 0 );
  assert( flinear == 1 || flinear == 0 );
/* if ( duf_pi_recursive( pi ) ) */
  {
    rd = d - duf_pi_maxdepth( pi ) < delta;                          /* d - pathinfo.maxdepth < delta */
    DUF_TRACE( levinfo, 60, "(%d>0) d:%d - pathinfo.maxdepth:%d < delta:%d", ( rd ), d, duf_pi_maxdepth( pi ), delta );
  }
  DUF_TRACE( levinfo, 30, "@@rd:%d max:%d; rel(%d):%d", rd, duf_pi_maxdepth( pi ), d, duf_pi_deltadepth_d( pi, d ) );
/* T( "@A(%d:%d) %d: %d : %d -> %d", frecursive, flinear, d, duf_pi_maxdepth( pi ), delta, rd ); */
  if ( flinear )
    rd = 1;
  else if ( rd && !frecursive /* && duf_pi_allow_dirs( pi ) */  )
  {
    rd = duf_pi_deltadepth_d( pi, d ) <= delta;                      /* d - topdepth <= delta */
    DUF_TRACE( levinfo, 40, "(%d>0) duf_pi_topdepth(pi):%d; duf_pi_reldepth_d( pdi, %d ):%d ? delta:%d;", ( rd ), d,
               duf_pi_topdepth( pi ), duf_pi_deltadepth_d( pi, d ), delta );
  /* T( "@B %d: %d : %d -> %d", d, duf_pi_maxdepth( pi ), delta, rd ); */
  }

/* rd= duf_pdi_topdepth( pdi ) + duf_pdi_reldepth( pdi ) < duf_pdi_maxdepth( pdi ); */
  return rd;
}

int
duf_pi_is_good_depth( const duf_pathinfo_t * pi, int delta, unsigned frecursive, unsigned flinear )
{
  int rd = 0;

  rd = duf_pi_is_good_depth_d( pi, delta, frecursive, flinear, duf_pi_depth( pi ) );
  return rd;
}
