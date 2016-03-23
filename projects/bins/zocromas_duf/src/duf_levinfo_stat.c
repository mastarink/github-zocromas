/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

/* #include "duf_tracen_defs.h"                                         (* T; TT; TR ♠ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ *) */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ♠ *) */
/* #include "duf_dodefs.h"                                              (* DOR ♠ *) */

# include "duf_se_only.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

/* #include "duf_config.h"                                              (* duf_get_config ✗ *) */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_dh.h"                                                  /* duf_openat_dh; duf_open_dh; duf_opened_dh; duf_close_dh; duf_statat_dh; etc. ✗ */

#include "duf_levinfo_ref_def.h"
#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */

#include "duf_pdi_structs.h"

#include "duf_levinfo_openclose.h"
/* ###################################################################### */
#include "duf_levinfo_stat.h"
/* ###################################################################### */

/* 20150904.105316 */
SR( PDI, levinfo_if_statat_dh_d, duf_depthinfo_t * pdi, int d )
{
/* DUF_STARTR( r ); */
  assert( pdi );
  assert( d >= 0 );

  if ( !duf_levinfo_stat_d( pdi, d ) )
    CR( levinfo_statat_dh_d, pdi, d );
/* DUF_ENDR( r ); */
  ER( PDI, levinfo_if_statat_dh_d, duf_depthinfo_t * pdi, int d );
}
/* *INDENT-OFF*  */
DUF_LEVINFO_F( mas_error_index_t, if_statat_dh )
DUF_LEVINFO_F_UP( mas_error_index_t, if_statat_dh )
/* *INDENT-ON*  */

/* 20150904.114634 */
/* may open upper level */
SR( PDI, levinfo_statat_dh_d, duf_depthinfo_t * pdi, int d )
{
/* DUF_STARTR( r ); */
  assert( pdi );
  assert( d >= 0 );

  assert( pdi->pathinfo.levinfo );
  assert( pdi->pathinfo.levinfo[d].itemname );
  assert( QISERR || d > 0 || !duf_levinfo_ptr_d( pdi, d - 1 ) );
  assert( QISERR || d > 0 || *duf_levinfo_itemtruename_d( pdi, d ) == 0 );
  assert( QISERR || d > 0 || 0 == strcmp( duf_levinfo_itemshowname_d( pdi, d ), "/" ) );

  {
    duf_dirhandle_t *pdhlev = duf_levinfo_pdh_d( pdi, d );
    duf_dirhandle_t *pdhuplev = duf_levinfo_pdh_d( pdi, d - 1 );

    assert( QISERR || d > 0 || !pdhuplev );
    assert( QISERR || d == 0 || pdhuplev );
    assert( QISERR || pdhlev );

    if ( d > 0 )
    {
      CR( levinfo_if_openat_dh_d, pdi, d - 1 );                      /* may open pdhuplev->dfd */
      assert( QISERR || d == 0 || ( pdhuplev && pdhuplev->dfd ) );
    }
    assert( QISERR || d == 0 || ( pdhuplev && pdhuplev->dfd ) );
    CR( statat_dh, pdhlev, pdhuplev, duf_levinfo_itemshowname_d( pdi, d ) );
    MAST_TRACE( levinfo, 10, "(%d)? levinfo statated %s", QERRIND, pdi->pathinfo.levinfo[d].itemname );

#if 0
    {
      if ( DUF_IS_ERROR_N( r, DUF_ERROR_STATAT_ENOENT ) )
      {
        _DUF_SHOW_ERROR( "No such entry %s/%s (%s)", duf_levinfo_path_d( pdi, d ), duf_levinfo_itemtruename_d( pdi, d ),
                         duf_levinfo_itemshowname_d( pdi, d ) );
        assert( 0 );
      }
    }
#endif
    assert( QISERR || pdhlev->st.st_dev );
    if ( QISERR_N( STAT_ENOENT ) || QISERR_N( STATAT_ENOENT ) )      /* FIXME ?? */
    {
      pdi->pathinfo.levinfo[d].deleted = 1;
      MAST_TRACE( levinfo, 10, "@@@(%d)? levinfo statated %s", QERRIND, pdi->pathinfo.levinfo[d].itemname );
    }
    pdi->pathinfo.levinfo[d].deleted_tested = 1;
    pdi->pathinfo.levinfo[d].stat_tested = 1;
  }
/* DUF_ENDR( r ); */
  ER( PDI, levinfo_statat_dh_d, duf_depthinfo_t * pdi, int d );
}
/* *INDENT-OFF*  */
DUF_LEVINFO_F( mas_error_index_t, statat_dh )
DUF_LEVINFO_F_UP( mas_error_index_t, statat_dh )
/* *INDENT-ON*  */

SRX( PDI, int, rd, 0, levinfo_if_deleted_d, duf_depthinfo_t * pdi, int d )
{
/* int rd = 0; */

  if ( !duf_levinfo_deleted_tested_d( pdi, d ) )
  {
    /* int r = 0; */

    ERRLOWER( STATAT_ENOENT );
    CR( levinfo_if_statat_dh_d, pdi, d );
    ERRUPPER( STATAT_ENOENT );
  /* DOR_LOWERE( r, duf_levinfo_if_statat_dh_d( pdi, d ), DUF_ERROR_STATAT_ENOENT ); */
  /* DUF_CLEAR_ERROR( r, DUF_ERROR_OPENAT_ENOENT, DUF_ERROR_STATAT_ENOENT ); */
  }
  rd = duf_levinfo_deleted_d( pdi, d );
/* assert( 0 ); */
  /* return rd; */
ERX( PDI, int, rd, 0, levinfo_if_deleted_d, duf_depthinfo_t * pdi, int d );
}

/* int                                                              */
/* duf_levinfo_deleted_d( const duf_depthinfo_t * pdi, int d ) */
/* {                                                                */
/*                                                                  */
/*   return duf_levinfo_ptr_d( pdi, d )->deleted ? 1 : 0;           */
/* }                                                                */
/*                                                                  */
/* (* *INDENT-OFF*  *)                                              */
/* DUF_LEVINFO_FC( int, item_deleted )                              */
/* DUF_LEVINFO_FC_UP( int, item_deleted )                           */
/* (* *INDENT-ON*  *)                                               */

/* *INDENT-OFF*  */
/* DUF_LEVINFO_3GET_BOOL( int, deleted, deleted ) */
DUF_LEVINFO_F( int, if_deleted )
DUF_LEVINFO_F_UP( int, if_deleted )
/* *INDENT-ON*  */

/************************************************************************/
