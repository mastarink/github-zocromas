/* #undef MAS_TRACING */
#include <assert.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

/* #include "duf_tracen_defs.h"                                         (* T; TT; TR ✗ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ✗ *) */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ✗ *) */
/* #include "duf_dodefs.h"                                              (* DOR ✗ *) */
#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_fmt_defs.h"

/* #include "duf_config.h" */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_pdi_ref.h"
#include "duf_pdi_pi_ref.h"
/* #include "duf_pdi_depth.h" */

#include "duf_levinfo.h"                                             /* duf_levinfo_calc_depth; duf_levinfo_clear_level_d; etc. ✗ */
#include "duf_levinfo_init.h"                                        /* duf_levinfo_init_level; duf_levinfo_init_level_d; etc. ✗ */
#include "duf_levinfo_dbinit.h"
#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */
#include "duf_levinfo_openclose.h"

/* #include "duf_pathinfo.h" */
#include "duf_pathinfo_depth.h"
/* #include "duf_pathinfo_ref.h" */

#include "duf_sccb_scanstage.h"                                      /* duf_nodetype_name, temporarily */
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

/* no side effects */
static
SR( PDI, levinfo_check_depth, const duf_depthinfo_t * pdi, duf_node_type_t node_type )
{
/* DUF_STARTR( r ); */
  assert( pdi );
#if 0
  {
    int delta;
    int rgd = 0;

  /* T( "@%d: check depth #%llu: %s - %llu", duf_pdi_depth(pdi), duf_levinfo_dirid( pdi ), duf_levinfo_path( pdi ), duf_levinfo_nameid( pdi ) ); */
    MAST_TRACE( temp, 3, "@%d: check depth #%llu: %s - %llu: %s", duf_pdi_depth( pdi ), duf_levinfo_dirid( pdi ), duf_levinfo_path( pdi ),
                duf_levinfo_nameid( pdi ), duf_levinfo_itemtruename( pdi ) );
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
      MAST_TRACE( depth, 0, "(%d) DEPTH: d=%d; max:%d; top:%d; delta:%d; R:%d; ", r, duf_pdi_depth( pdi ), duf_pdi_maxdepth( pdi ),
                  duf_pdi_topdepth( pdi ), delta, duf_pdi_recursive( pdi ) );
  /* }                                             */
  /* else if ( duf_pdi_reldepth( pdi ) > delta )   */
  /*   DUF_MAKE_ERROR( r, DUF_ERROR_MAX_DEPTH);    */
  }
#else /* 20160120.193955 */
  CR( pi_check_depth, &pdi->pathinfo, node_type, duf_pdi_recursive( pdi ), duf_pdi_linear( pdi ) );
#endif
/* DUF_ENDR( r ); */
  ER( PDI, levinfo_check_depth, const duf_depthinfo_t * pdi, duf_node_type_t node_type );
}

/* 20151026.104256 */
/* check depth; may change levinfo (for upper level) via duf_levinfo_countdown_dirs */
static
SR( PDI, levinfo_godown_i, duf_depthinfo_t * pdi, const char *itemname MAS_UNUSED, duf_node_type_t node_type, int flinear )
{
/* DUF_STARTR( r ); */
  assert( pdi );
#if 0
  assert( pdi->pathinfo.depth == duf_levinfo_calc_depth( pdi ) );

  DOR( r, duf_levinfo_check_depth( pdi, node_type ) );               /* no side effects */
  if ( DUF_NOERROR( r ) )
  {
    int d;

    d = ++pdi->pathinfo.depth;
    T( "d:%d", d );
    assert( d >= 0 );
    assert( pdi->pathinfo.depth >= 0 );
    assert( d == pdi->pathinfo.depth );

    assert( pdi->pathinfo.levinfo );

    MAST_TRACE( explain, 2, "level down: %d; ≪%s≫  [%s]", d, duf_nodetype_name( node_type ), duf_levinfo_itemshowname( pdi ) );
    if ( node_type == DUF_NODE_LEAF )
      MAST_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan leaf    =>           - %s", duf_pdi_depth( pdi ), duf_levinfo_itemshowname( pdi ) );
    else
    {
      MAST_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": scan node:   =>  by %5llu - %s", duf_pdi_depth( pdi ), duf_levinfo_dirid( pdi ),
                  duf_levinfo_itemshowname( pdi ) );
    }
    if ( node_type == DUF_NODE_NODE )
      duf_levinfo_countdown_dirs( pdi );                             /* may change levinfo (for upper level) */
    assert( pdi->pathinfo.depth - 1 /* not yet ... */  == duf_levinfo_calc_depth( pdi ) );
  }
#else
  CR( pi_godown, &pdi->pathinfo, node_type, duf_pdi_recursive( pdi ), flinear < 0 ? duf_pdi_linear( pdi ) : flinear );
#endif
/* DUF_ENDR( r ); */
  ER( PDI, levinfo_godown_i, duf_depthinfo_t * pdi, const char *itemname MAS_UNUSED, duf_node_type_t node_type, int flinear );
}

/* 20150901.173329 */
/* check depth; resets levinfo  (currenl level) with dirid,nfiles,ndirs; may change levinfo (for upper level) */
static
SR( PDI, levinfo_godown_dirid, duf_depthinfo_t * pdi, const char *itemname, unsigned long long dirid, duf_node_type_t node_type )
{
/* DUF_STARTR( r ); */
  assert( pdi );

  CR( levinfo_godown_i, pdi, itemname, node_type, -1 );              /* check depth; may change levinfo (for upper level) via duf_levinfo_countdown_dirs */
  if ( QNOERR )
    duf_levinfo_init_level( pdi, itemname, dirid, node_type );       /* resets levinfo (currenl level) */

  assert( pdi->pathinfo.depth == duf_levinfo_calc_depth( pdi ) );

/* DUF_ENDR( r ); */
  ER( PDI, levinfo_godown_dirid, duf_depthinfo_t * pdi, const char *itemname, unsigned long long dirid, duf_node_type_t node_type );
}

/* 20151026.104757 */
/* check depth; resets levinfo  (currenl level) without dirid,nfiles,ndirs; may change levinfo (for upper level) */
SR( PDI, levinfo_godown, duf_depthinfo_t * pdi, const char *itemname, duf_node_type_t node_type )
{
/* DUF_STARTR( r ); */
#if 0
  assert( pdi );

  DOR( r, duf_levinfo_godown_i( pdi, itemname, node_type, -1 ) );    /* check depth; may change levinfo (for upper level) via duf_levinfo_countdown_dirs */
  if ( DUF_NOERROR( r ) )
    duf_levinfo_init_level( pdi, itemname, 0, 0, 0, node_type );     /* resets levinfo  (currenl level) */
#else
  CR( levinfo_godown_dirid, pdi, itemname, 0 /* dirid */ ,
      node_type );
#endif

  assert( pdi->pathinfo.depth == duf_levinfo_calc_depth( pdi ) );

/* DUF_ENDR( r ); */
  ER( PDI, levinfo_godown, duf_depthinfo_t * pdi, const char *itemname, duf_node_type_t node_type );
}

/* 20150904.120835 */
static
SR( PDI, levinfo_godown_db, duf_depthinfo_t * pdi, duf_node_type_t node_type, duf_stmnt_t * pstmt )
{
/* DUF_STARTR( r ); */
  assert( pdi );

  CR( levinfo_check_depth, pdi, node_type );
  MAST_TRACE( temp, 50, "@@(%s) maxdepth:%d;", QERRNAME, duf_pdi_maxdepth( pdi ) );
  if ( QNOERR )
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

    if ( node_type == DUF_NODE_NODE )
      duf_levinfo_countdown_dirs( pdi );                             /* may change levinfo (for upper level) */
  /* ------------------------------------------- */
    assert( duf_levinfo_dirid( pdi ) == 0 );
    assert( !pdi->pathinfo.levinfo[d].itemname );
    duf_levinfo_dbinit_level_d( pdi, pstmt, node_type, d );
    assert( pdi->pathinfo.levinfo[d].itemname );

    assert( duf_levinfo_dirid( pdi ) != 0 );
    assert( d == pdi->pathinfo.depth );

    assert( pdi->pathinfo.depth == duf_levinfo_calc_depth( pdi ) );
    MAST_TRACE( explain, 2, "level down: %d; ≪%s≫  [%s]", d, duf_nodetype_name( node_type ), duf_levinfo_itemshowname( pdi ) );
  }
/* DUF_ENDR( r ); */
  ER( PDI, levinfo_godown_db, duf_depthinfo_t * pdi, duf_node_type_t node_type, duf_stmnt_t * pstmt );
}

/* 20151026.105741 */
/* check depth; resets levinfo  (currenl level); may change levinfo (for upper level)
 * does not really open dh?!
 * XXX equal XXX to duf_levinfo_godown - except DUF_ERROR_TOO_DEEP ? XXX
 */
SR( PDI, levinfo_godown_openat_dh, duf_depthinfo_t * pdi, const char *itemname, duf_node_type_t node_type )
{
/* DUF_STARTR( r ); */
  assert( pdi );
#if 0
  DOR_LOWERE( r, duf_levinfo_godown_dirid( pdi, itemname, 0, node_type ), DUF_ERROR_TOO_DEEP ); /* check depth; resets levinfo  (currenl level)
                                                                                                   with dirid,nfiles,ndirs;
                                                                                                   may change levinfo (for upper level) */
#else
  ERRLOWER( TOO_DEEP );
  CR( levinfo_godown, pdi, itemname, node_type );                    /* check depth; resets levinfo  (currenl level); may change levinfo (for upper level) */
  ERRUPPER( TOO_DEEP );
/* DOR_LOWERE( r, duf_levinfo_godown( pdi, itemname, node_type ), DUF_ERROR_TOO_DEEP ); (* check depth; resets levinfo  (currenl level); may change levinfo (for upper level) *) */
#endif
/* DUF_ENDR_LOWERE( r, DUF_ERROR_TOO_DEEP ); */
  ER( PDI, levinfo_godown_openat_dh, duf_depthinfo_t * pdi, const char *itemname, duf_node_type_t node_type );
}

/* 20150904.120814 */
/* XXX equal to duf_levinfo_godown_db - except DUF_ERROR_TOO_DEEP ? XXX */
SR( PDI, levinfo_godown_dbopenat_dh, duf_depthinfo_t * pdi, duf_node_type_t node_type, duf_stmnt_t * pstmt )
{
/* DUF_STARTR( r ); */
  assert( pdi );
  ERRLOWER( TOO_DEEP );
  CR( levinfo_godown_db, pdi, node_type, pstmt );
/* DOR_LOWERE( r, duf_levinfo_godown_db( pdi, node_type, pstmt ), DUF_ERROR_TOO_DEEP ); */
  ERRUPPER( TOO_DEEP );
  assert( pdi->pathinfo.depth == duf_levinfo_calc_depth( pdi ) );
/* DUF_ENDR_LOWERE( r, DUF_ERROR_TOO_DEEP ); */
  ER( PDI, levinfo_godown_dbopenat_dh, duf_depthinfo_t * pdi, duf_node_type_t node_type, duf_stmnt_t * pstmt );
}

SR( PDI, levinfo_goup, duf_depthinfo_t * pdi )
{
/* DUF_STARTR( r ); */
  assert( pdi );
  if ( duf_levinfo_is_leaf( pdi ) )
  {
    MAST_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan leaf  <=             - %s", duf_pdi_depth( pdi ), duf_levinfo_itemshowname( pdi ) );
  }
  else
  {
    MAST_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": scan node: <=    by %5llu - %s", duf_pdi_depth( pdi ), duf_levinfo_dirid( pdi ),
                duf_levinfo_itemshowname( pdi ) );
  }
  {
    int d;

    assert( pdi->pathinfo.depth == duf_levinfo_calc_depth( pdi ) );
    d = pdi->pathinfo.depth--;
    assert( d >= 0 );
    if ( duf_levinfo_opened_dh_d( pdi, d ) > 0 )
      CR( levinfo_closeat_dh_d, pdi, d );
    assert( !duf_levinfo_opened_here_dh_d( pdi, d ) || pdi->pathinfo.levinfo[d].lev_dh.dfd == 0 );
  /* if ( DUF_IS_ERROR( r ) )                                              */
  /*   DUF_SHOW_ERRORO( "(%d) close error; L%d", r, pdi->pathinfo.depth ); */
    MAST_TRACE( explain, 2, "level up:   %d", d );
    assert( pdi->pathinfo.levinfo );
    duf_levinfo_clear_level_d( pdi, d );
    d = pdi->pathinfo.depth;
  /* Not here: assert( d >= 0 ); */
  }
/* DUF_ENDR( r ); */
  ER( PDI, levinfo_goup, duf_depthinfo_t * pdi );
}
