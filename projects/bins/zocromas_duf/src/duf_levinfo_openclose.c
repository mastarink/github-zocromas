#include <string.h>
#include <assert.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_dh.h"

#include "duf_levinfo_ref_def.h"
#include "duf_levinfo_ref.h"

/* ###################################################################### */
#include "duf_levinfo_openclose.h"
/* ###################################################################### */

int
duf_levinfo_if_openat_dh_d( duf_depthinfo_t * pdi, int d )
{
  DEBUG_STARTR( r );
  assert( pdi );
  assert( d >= 0 );

  if ( duf_levinfo_opened_dh_d( pdi, d ) <= 0 )
    DOR( r, duf_levinfo_openat_dh_d( pdi, d ) );
  DUF_TRACE( levinfo, 5, "%d", duf_levinfo_dfd_d( pdi, d ) );
  assert( duf_levinfo_dfd_d( pdi, d ) > 0 );
  DEBUG_ENDR( r );
}
/* *INDENT-OFF*  */
DUF_LEVINFO_F( int, if_openat_dh )
DUF_LEVINFO_F_UP( int, if_openat_dh )
/* *INDENT-ON*  */

/* 20150904.120545 */
int
duf_levinfo_openat_dh_d( duf_depthinfo_t * pdi, int d )
{
  DEBUG_STARTR( r );
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
      DUF_TRACE( levinfo, 0, "(%d)? levinfo openated %s; dfd:%d", r, duf_levinfo_itemshowname_d( pdi, d ), pdhlev->dfd );
    }
    else                        /* d > 0 ! */
    {
      DOR_NOE( r, duf_levinfo_if_openat_dh_d( pdi, d - 1 ), DUF_ERROR_FS_DISABLED );
      assert( r <= 0 || pdhuplev->dfd );

      DOR_NOE( r, duf_openat_dh( pdhlev, pdhuplev, duf_levinfo_itemshowname_d( pdi, d ), duf_levinfo_is_leaf_d( pdi, d ) ), DUF_ERROR_OPENAT_ENOENT );
      DUF_TRACE( levinfo, r < 0 ? 0 : 2, "(%d)? levinfo openated %s : %s; dfd:%d", r, duf_levinfo_path_d( pdi, d ),
                 duf_levinfo_itemshowname_d( pdi, d ), pdhlev->dfd );
    }
    assert( r <= 0 || pdhlev->dfd );
    if ( DUF_IS_ERROR( r, DUF_ERROR_OPEN_ENOENT ) || DUF_IS_ERROR( r, DUF_ERROR_OPENAT_ENOENT ) )
    {
      pdi->pathinfo.levinfo[d].deleted = 1;
      DUF_TRACE( levinfo, r < 0 ? 0 : 2, "@(%d)? levinfo [deleted] %s : %s; opendir:%d", r, duf_levinfo_path_d( pdi, d ),
                 duf_levinfo_itemshowname_d( pdi, d ), pdi->opendir );
      r = 0;
    }
  }
  else
  {
    DUF_TRACE( temp, 0, "@@@@@pdi->opendir not set" );
    DUF_TRACE( levinfo, 0, "pdi->opendir not set" );
  }
  DEBUG_ENDR( r );
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
  DEBUG_STARTR( r );
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
  DEBUG_ENDR( r );
}

/* *INDENT-OFF*  */
DUF_LEVINFO_F( int, opened_dh )
DUF_LEVINFO_F_UP( int, opened_dh )
/* *INDENT-ON*  */

int
duf_levinfo_opened_here_dh_d( duf_depthinfo_t * pdi, int d )
{
  DEBUG_STARTR( r );
  if ( !duf_levinfo_opened_copy_d( pdi, d ) )
    DOR( r, duf_levinfo_opened_dh_d( pdi, d ) );
  DEBUG_ENDR( r );
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
  DEBUG_STARTR( r );
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

  DEBUG_ENDR( r );
}

/* *INDENT-OFF*  */
DUF_LEVINFO_F( int, closeat_dh )
DUF_LEVINFO_F_UP( int, closeat_dh )
/* *INDENT-ON*  */
