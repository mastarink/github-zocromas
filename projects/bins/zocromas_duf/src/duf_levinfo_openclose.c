/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_dh.h"                                                  /* duf_openat_dh; duf_open_dh; duf_opened_dh; duf_close_dh; duf_statat_dh; etc. ✗ */

#include "duf_levinfo_ref_def.h"
#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */
#include "duf_levinfo_structs.h"

#include "duf_sccb_scanstage.h"                                      /* duf_scanstage_name; duf_scanstage_scanner; ✗ */

#include "duf_pdi_ref.h"
#include "duf_pdi_structs.h"

/* ###################################################################### */
#include "duf_levinfo_openclose.h"
/* ###################################################################### */

/* 20150904.120545 */
static
SR( PDI, levinfo_openat_dh_d, duf_depthinfo_t * pdi, int d )
{
  assert( pdi );
  assert( d >= 0 );

  assert( pdi );
  assert( pdi->pathinfo.levinfo );
  assert( d >= 0 );

/* ????? What was this ???
   if(d>0)
   pdi->pathinfo.levinfo[d - 1].lev_dh.dfd = 0;
 */
  if ( duf_pdi_opendir( pdi ) )
  {
    duf_levinfo_t *pli, *pliu;

    pli = duf_levinfo_ptr_d( pdi, d );
    pliu = duf_levinfo_ptr_d( pdi, d - 1 );
    duf_dirhandle_t *pdhlev = &pli->lev_dh;
    duf_dirhandle_t *pdhuplev = pliu ? &pliu->lev_dh : NULL;

    assert( !pdhlev->dfd );
    assert( duf_levinfo_itemshowname_d( pdi, d ) );
    if ( d == 0 )
    {
      assert( *duf_levinfo_itemshowname_d( pdi, d ) == '/' );
      assert( duf_levinfo_itemshowname_d( pdi, d )[0] == '/' );
      assert( duf_levinfo_itemshowname_d( pdi, d )[1] == 0 );
      assert( *duf_levinfo_itemtruename_d( pdi, d ) == 0 );
      CR( open_dh, pdhlev, CRX( levinfo_itemshowname_d, pdi, d ) );
      assert( QISERR || pdhlev->dfd > 0 );
      MAST_TRACE( levinfo, 0, "(%d)? levinfo openated %s; dfd:%d", QERRIND, duf_levinfo_itemshowname_d( pdi, d ), pdhlev->dfd );
    }
    else                                                             /* d > 0 ! */
    {
      {
        ERRLOWER( FS_DISABLED );
        CR( levinfo_if_openat_dh_d, pdi, d - 1 );
        ERRUPPER( FS_DISABLED );
      }
    /* DOR_LOWERE( r, duf_levinfo_if_openat_dh_d( pdi, d - 1 ), DUF_ERROR_FS_DISABLED ); */
      assert( QISERR || pdhuplev->dfd );

      {
        ERRLOWER( OPENAT_ENOENT );
        CR( openat_dh, pdhlev, pdhuplev, duf_levinfo_itemshowname_d( pdi, d ), duf_levinfo_is_leaf_d( pdi, d ) /* asfile */  );
        ERRUPPER( OPENAT_ENOENT );
      }
    /* DOR_LOWERE( r, duf_openat_dh( pdhlev, pdhuplev, duf_levinfo_itemshowname_d( pdi, d ), duf_levinfo_is_leaf_d( pdi, d ) (* asfile *)  ), */
    /*             DUF_ERROR_OPENAT_ENOENT );                                                                                                 */
      assert( QISERR || pdhlev->dfd > 0 );
      MAST_TRACE( levinfo, QISERR ? 0 : 2, "(%s)? levinfo openated %s : %s; dfd:%d", QERRNAME, duf_levinfo_path_d( pdi, d ),
                  duf_levinfo_itemshowname_d( pdi, d ), pdhlev->dfd );
    }
    assert( QISERR || QERRIND == 0 || pdhlev->dfd );
    if ( QISERR_N( OPEN_ENOENT ) || QISERR_N( OPENAT_ENOENT ) )
    {
      pdi->pathinfo.levinfo[d].deleted = 1;
      MAST_TRACE( levinfo, QISERR ? 0 : 2, "@(%s)? levinfo [deleted] %s : %s; opendir:%d", QERRNAME, duf_levinfo_path_d( pdi, d ),
                  duf_levinfo_itemshowname_d( pdi, d ), duf_pdi_opendir( pdi ) );
      QERRIND = 0;
    }
    else
    {
      assert( QISERR || !duf_pdi_opendir( pdi ) || ( duf_levinfo_dfd_d( pdi, d ) > 0 ) );
    }
    pdi->pathinfo.levinfo[d].deleted_tested = 1;
  }
  else
  {
    MAST_TRACE( fs, 0, "@@@@@duf_pdi_opendir(pdi) not set" );
    MAST_TRACE( levinfo, 0, "duf_pdi_opendir(pdi) not set" );
  }
  assert( !duf_pdi_opendir( pdi ) || QISERR || pdi->pathinfo.levinfo[d].deleted || duf_levinfo_dfd_d( pdi, d ) > 0 );
/* QT( "@d:%d dfd:%d - %s / %s", d, duf_levinfo_dfd_d( pdi, d ), duf_levinfo_itemshowname_d( pdi, d ), */
/*     duf_nodetype_name( duf_levinfo_node_type_d( pdi, d ) ) );                                       */
  ER( PDI, levinfo_openat_dh_d, duf_depthinfo_t * pdi, int d );
}
/* *INDENT-OFF*  */
DUF_LEVINFO_F( int, openat_dh )
DUF_LEVINFO_F_UP( int, openat_dh )
/* *INDENT-ON*  */

SR( PDI, levinfo_if_openat_dh_d, duf_depthinfo_t * pdi, int d )
{
  assert( pdi );
  assert( d >= 0 );

  if ( CRX( levinfo_opened_dh_d, pdi, d ) <= 0 )
  {
    CR( levinfo_openat_dh_d, pdi, d );
    assert( QISERR || !duf_pdi_opendir( pdi ) || duf_levinfo_deleted_d( pdi, d ) || duf_levinfo_dfd_d( pdi, d ) > 0 );
  }
  MAST_TRACE( levinfo, 5, "%d", duf_levinfo_dfd_d( pdi, d ) );

  assert( QISERR || !duf_pdi_opendir( pdi ) || duf_levinfo_deleted_d( pdi, d ) || duf_levinfo_dfd_d( pdi, d ) > 0 );
  ER( PDI, levinfo_if_openat_dh_d, duf_depthinfo_t * pdi, int d );
}
/* *INDENT-OFF*  */
DUF_LEVINFO_F( int, if_openat_dh )
DUF_LEVINFO_F_UP( int, if_openat_dh )
/* *INDENT-ON*  */

/************************************************************************/

/* 20150904.120515 */
/* returns handle >0 
 * <0 --- ERROR?
 * */
SRX( PDI, int, dfd, 0, levinfo_opened_dh_d, duf_depthinfo_t * pdi, int d )
{
  assert( pdi );
  assert( d >= 0 );
  dfd = CRX( levinfo_dfd_d, pdi, d );
  ERX( PDI, int, dfd, 0, levinfo_opened_dh_d, duf_depthinfo_t * pdi, int d );
}

/* *INDENT-OFF*  */
DUF_LEVINFO_F( int, opened_dh )
DUF_LEVINFO_F_UP( int, opened_dh )
/* *INDENT-ON*  */

SR( PDI, levinfo_opened_here_dh_d, duf_depthinfo_t * pdi, int d )
{
  if ( !CRX( levinfo_opened_copy_d, pdi, d ) )
    CR( levinfo_opened_dh_d, pdi, d );
  ER( PDI, levinfo_opened_here_dh_d, duf_depthinfo_t * pdi, int d );
}

/* *INDENT-OFF*  */
DUF_LEVINFO_F( int, opened_here_dh )
DUF_LEVINFO_F_UP( int, opened_here_dh )
/* *INDENT-ON*  */

/************************************************************************/
/* 20150904.120506 */
SR( PDI, levinfo_closeat_dh_d, duf_depthinfo_t * pdi, int d )
{
  assert( pdi );
  assert( d >= 0 );

  if ( duf_levinfo_opened_here_dh_d( pdi, d ) > 0 )
  {
    CR( close_dh, duf_levinfo_pdh_d( pdi, d ) );
  }
  assert( !duf_levinfo_opened_here_dh_d( pdi, d ) || duf_levinfo_dfd_d( pdi, d ) == 0 );

  ER( PDI, levinfo_closeat_dh_d, duf_depthinfo_t * pdi, int d );
}

/* *INDENT-OFF*  */
DUF_LEVINFO_F( int, closeat_dh )
DUF_LEVINFO_F_UP( int, closeat_dh )
/* *INDENT-ON*  */
