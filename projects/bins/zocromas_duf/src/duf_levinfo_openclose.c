/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs.h"                                         /* MAST_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

/* #include "duf_config.h"                                              (* duf_get_config ♠ *) */
#include "duf_config_util.h"                                         /* duf_get_trace_config ♠ */

#include "duf_dh.h"

#include "duf_levinfo_ref_def.h"
#include "duf_levinfo_ref.h"

/* ###################################################################### */
#include "duf_levinfo_openclose.h"
/* ###################################################################### */

int
duf_levinfo_if_openat_dh_d( duf_depthinfo_t * pdi, int d )
{
  DUF_STARTR( r );
  assert( pdi );
  assert( d >= 0 );

  if ( duf_levinfo_opened_dh_d( pdi, d ) <= 0 )
  {
    DOR( r, duf_levinfo_openat_dh_d( pdi, d ) );
    assert( DUF_IS_ERROR( r ) || !pdi->opendir || duf_levinfo_deleted_d( pdi, d ) || duf_levinfo_dfd_d( pdi, d ) > 0 );
  }
  MAST_TRACE( levinfo, 5, "%d", duf_levinfo_dfd_d( pdi, d ) );

  assert( DUF_IS_ERROR( r ) || !pdi->opendir || duf_levinfo_deleted_d( pdi, d ) || duf_levinfo_dfd_d( pdi, d ) > 0 );
  DUF_ENDR( r );
}
/* *INDENT-OFF*  */
DUF_LEVINFO_F( int, if_openat_dh )
DUF_LEVINFO_F_UP( int, if_openat_dh )
/* *INDENT-ON*  */

/* 20150904.120545 */
int
duf_levinfo_openat_dh_d( duf_depthinfo_t * pdi, int d )
{
  DUF_STARTR( r );
  assert( pdi );
  assert( d >= 0 );

  assert( pdi );
  assert( pdi->pathinfo.levinfo );
  assert( d >= 0 );

/* ????? What was this ???
   if(d>0)
   pdi->pathinfo.levinfo[d - 1].lev_dh.dfd = 0;
 */
  if ( pdi->opendir )
  {
    duf_levinfo_t *pli, *pliu;

#if 0
    pli = &pdi->pathinfo.levinfo[d];
    pliu = d > 0 ? &pdi->pathinfo.levinfo[d - 1] : NULL;
#else
    pli = duf_levinfo_ptr_d( pdi, d );
    pliu = duf_levinfo_ptr_d( pdi, d - 1 );
#endif
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
      DOR( r, duf_open_dh( pdhlev, duf_levinfo_itemshowname_d( pdi, d ) ) );
      assert( DUF_IS_ERROR( r ) || pdhlev->dfd > 0 );
      MAST_TRACE( levinfo, 0, "(%d)? levinfo openated %s; dfd:%d", r, duf_levinfo_itemshowname_d( pdi, d ), pdhlev->dfd );
    }
    else                                                             /* d > 0 ! */
    {
      DOR_LOWERE( r, duf_levinfo_if_openat_dh_d( pdi, d - 1 ), DUF_ERROR_FS_DISABLED );
      assert( DUF_IS_ERROR( r ) || pdhuplev->dfd );

      DOR_LOWERE( r, duf_openat_dh( pdhlev, pdhuplev, duf_levinfo_itemshowname_d( pdi, d ), duf_levinfo_is_leaf_d( pdi, d ) /* asfile */  ),
                  DUF_ERROR_OPENAT_ENOENT );
      assert( DUF_IS_ERROR( r ) || pdhlev->dfd > 0 );
      MAST_TRACE( levinfo, r < 0 ? 0 : 2, "(%s)? levinfo openated %s : %s; dfd:%d", mas_error_name_i( r ), duf_levinfo_path_d( pdi, d ),
                 duf_levinfo_itemshowname_d( pdi, d ), pdhlev->dfd );
    }
    assert( r <= 0 || pdhlev->dfd );
    if ( DUF_IS_ERROR_N( r, DUF_ERROR_OPEN_ENOENT ) || DUF_IS_ERROR_N( r, DUF_ERROR_OPENAT_ENOENT ) )
    {
      pdi->pathinfo.levinfo[d].deleted = 1;
      MAST_TRACE( levinfo, r < 0 ? 0 : 2, "@(%s)? levinfo [deleted] %s : %s; opendir:%d", mas_error_name_i( r ), duf_levinfo_path_d( pdi, d ),
                 duf_levinfo_itemshowname_d( pdi, d ), pdi->opendir );
      r = 0;
    }
    else
    {
      assert( DUF_IS_ERROR( r ) || !pdi->opendir || ( duf_levinfo_dfd_d( pdi, d ) > 0 ) );
    }
    pdi->pathinfo.levinfo[d].deleted_tested = 1;
  }
  else
  {
    MAST_TRACE( fs, 0, "@@@@@pdi->opendir not set" );
    MAST_TRACE( levinfo, 0, "pdi->opendir not set" );
  }
  assert( !pdi->opendir || DUF_IS_ERROR( r ) || pdi->pathinfo.levinfo[d].deleted || duf_levinfo_dfd_d( pdi, d ) > 0 );
  DUF_ENDR( r );
}
/* *INDENT-OFF*  */
DUF_LEVINFO_F( int, openat_dh )
DUF_LEVINFO_F_UP( int, openat_dh )
/* *INDENT-ON*  */

/************************************************************************/

/* 20150904.120515 */
/* returns handle >0 
 * <0 --- ERROR?
 * */
int
duf_levinfo_opened_dh_d( duf_depthinfo_t * pdi, int d )
{
  DUF_STARTR( r );
  assert( pdi );
  assert( d >= 0 );
#if 0
  if ( pdi->opendir || duf_levinfo_dfd_d( pdi, d ) )
    DOR( r, duf_opened_dh( &duf_levinfo_ptr_d( pdi, d )->lev_dh ) );
#else
  DOR( r, duf_levinfo_dfd_d( pdi, d ) );
#endif
#if 0
  if ( d < 0 )
    d = 0;
  assert( d > 0 );
#endif
/*
 * duf_levinfo_dfd_d( pdi, d )    returns : duf_levinfo_ptr_d( pdi, d )->lev_dh.dfd
 * duf_opened_dh( &duf_levinfo_ptr_d( pdi, d )->lev_dh ) returns :  duf_levinfo_ptr_d( pdi, d )->lev_dh->dfd
 */
  DUF_ENDR( r );
}

/* *INDENT-OFF*  */
DUF_LEVINFO_F( int, opened_dh )
DUF_LEVINFO_F_UP( int, opened_dh )
/* *INDENT-ON*  */

int
duf_levinfo_opened_here_dh_d( duf_depthinfo_t * pdi, int d )
{
  DUF_STARTR( r );
  if ( !duf_levinfo_opened_copy_d( pdi, d ) )
    DOR( r, duf_levinfo_opened_dh_d( pdi, d ) );
  DUF_ENDR( r );
}

/* *INDENT-OFF*  */
DUF_LEVINFO_F( int, opened_here_dh )
DUF_LEVINFO_F_UP( int, opened_here_dh )
/* *INDENT-ON*  */

/************************************************************************/
/* 20150904.120506 */
int
duf_levinfo_closeat_dh_d( duf_depthinfo_t * pdi, int d )
{
  DUF_STARTR( r );
  assert( pdi );
  assert( d >= 0 );

  if ( duf_levinfo_opened_here_dh_d( pdi, d ) > 0 )
  {
#if 0
    DOR( r, duf_close_dh( &duf_levinfo_ptr_d( pdi, d )->lev_dh ) );
#else
    DOR( r, duf_close_dh( duf_levinfo_pdh_d( pdi, d ) ) );
#endif
  }
  assert( !duf_levinfo_opened_here_dh_d( pdi, d ) || duf_levinfo_dfd_d( pdi, d ) == 0 );

  DUF_ENDR( r );
}

/* *INDENT-OFF*  */
DUF_LEVINFO_F( int, closeat_dh )
DUF_LEVINFO_F_UP( int, closeat_dh )
/* *INDENT-ON*  */
