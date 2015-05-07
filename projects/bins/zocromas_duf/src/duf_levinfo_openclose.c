#include <string.h>
#include <assert.h>
#include <limits.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"


/* #include "duf_config_ref.h" */
#include "duf_dh.h"


/* #include "duf_pdi.h" */

#include "duf_levinfo_ref_def.h"
#include "duf_levinfo_ref.h"
/* ###################################################################### */
#include "duf_levinfo_openclose.h"
/* ###################################################################### */



int
duf_levinfo_openat_dh( duf_depthinfo_t * pdi )
{
  assert( pdi );
  {
    int r = 0;
    int d = pdi->depth;

    assert( pdi->levinfo );
    assert( d >= 0 );

    duf_levinfo_t *pli, *pliu;

    pli = &pdi->levinfo[d];
    pliu = d > 0 ? &pdi->levinfo[d - 1] : NULL;
    /* ????? What was this ???
       if(d>0)
       pdi->levinfo[d - 1].lev_dh.dfd = 0;
     */
    if ( pdi->opendir )
    {
      duf_dirhandle_t *pdhlev = &pli->lev_dh;
      duf_dirhandle_t *pdhuplev = pliu ? &pliu->lev_dh : NULL;

      /* if ( S_ISBLK( stX.st_mode ) ) */
      /* {                             */
      /* }                             */
      /* DUF_SHOW_ERROR( "%s", pdi->levinfo[d].is_leaf ? "LEAF" : "NODE" ); */
      /* DUF_PRINTF( 0, "d:%d [%s]", d, pdi->levinfo[d].itemname ); */
      assert( pdi->levinfo[d].itemname );
      if ( d == 0 )
      {
        /* char *sp; */

        assert( *pdi->levinfo[d].itemname == 0 );
        /* sp = mas_strdup( "/" ); */
        /* sp = mas_strcat_x( sp, pdi->levinfo[d].itemname ); */
        r = duf_open_dh( pdhlev, "/" );
        /* mas_free( sp ); */
      }
      else
      {
        r = duf_openat_dh( pdhlev, pdhuplev, pdi->levinfo[d].itemname, pdi->levinfo[d].is_leaf );
      }
      if ( r >= 0 )
      {
        assert( pdhlev->dfd );
      }
      /* if ( r == DUF_ERROR_OPEN_ENOENT || r == DUF_ERROR_OPENAT_ENOENT ) */
      /* {                                                                 */
      /*   pdi->levinfo[d].deleted = 1;                                    */
      /*   r = 0;                                                          */
      /* }                                                                 */
    }
    DUF_TEST_R( r );
    return r;
  }
}

int
duf_levinfo_open_dh( duf_depthinfo_t * pdi, const char *path )
{
  assert( pdi );
  {
    int r = 0;
    int d = pdi->depth;
    char *real_path = NULL;

    assert( pdi->levinfo );

    if ( path )
    {
      real_path = mas_malloc( PATH_MAX );
      if ( real_path )
      {
        *real_path = 0;
        if ( !realpath( path, real_path ) )
          DUF_MAKE_ERROR( r, DUF_ERROR_PATH );
      }
      else
        DUF_MAKE_ERROR( r, DUF_ERROR_MEMORY );
    }

    if ( pdi->opendir )
    {
      assert( d >= 0 );
      duf_dirhandle_t *pdhlev = &pdi->levinfo[d].lev_dh;

      /* if ( S_ISBLK( stX.st_mode ) ) */
      /* {                             */
      /* }                             */

      if ( r >= 0 && real_path )
        r = duf_open_dh( pdhlev, real_path );
      if ( r == DUF_ERROR_OPEN_ENOENT || r == DUF_ERROR_OPENAT_ENOENT )
      {
        pdi->levinfo[d].deleted = 1;
        r = 0;
      }
      if ( r >= 0 )
        assert( pdhlev->dfd );
    }
/*     {                                        */
/* #ifdef DUF_NO_ECONOMY                        */
/*       if ( r >= 0 )                          */
/*         pdi->path = real_path;               */
/*       else                                   */
/*         mas_free( real_path );               */
/* #else                                        */
/*       if ( r >= 0 )                          */
/*         pdi->path = mas_strdup( real_path ); */
/*       mas_free( real_path );                 */
/* #endif                                       */
/*     }                                        */
    DUF_TEST_R( r );
    return r;
  }
}

/************************************************************************/


/* returns handle >0 */
int
duf_levinfo_opened_dh_d( duf_depthinfo_t * pdi, int d )
{
  DEBUG_STARTR( r );
  if ( pdi->opendir || duf_levinfo_dfd_d( pdi, d ) )
    r = duf_opened_dh( &duf_levinfo_ptr_d( pdi, d )->lev_dh );
  DEBUG_ENDR( r );
}

/* *INDENT-OFF*  */
DUF_LEVINFO_F( int, opened_dh )
DUF_LEVINFO_F_UP( int, opened_dh )
/* *INDENT-ON*  */

/************************************************************************/

int
duf_levinfo_closeat_dh_d( duf_depthinfo_t * pdi, int d )
{
  DEBUG_STARTR( r );

  if ( duf_levinfo_dfd_d( pdi, d ) )
    r = duf_close_dh( &duf_levinfo_ptr_d( pdi, d )->lev_dh );
  assert( duf_levinfo_dfd_d( pdi, d ) == 0 );

  DEBUG_ENDR( r );
}

/* *INDENT-OFF*  */
DUF_LEVINFO_F( int, closeat_dh )
DUF_LEVINFO_F_UP( int, closeat_dh )
/* *INDENT-ON*  */
