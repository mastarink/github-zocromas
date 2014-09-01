#include <string.h>
#include <assert.h>
#include <limits.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"


#include "duf_config_ref.h"
#include "duf_dh.h"


#include "duf_pdi.h"

/* ###################################################################### */
#include "duf_levinfo.h"
/* ###################################################################### */



static void
duf_levinfo_clear_context( duf_levinfo_t * pli )
{
  assert( pli );
  duf_clear_context( &pli->context );
}

void
duf_levinfo_set_context_up_destructor( duf_depthinfo_t * pdi, duf_void_voidp_t destr )
{
  assert( pdi );

  {
    int d;

    d = pdi->depth - 1;
    if ( d >= 0 )
      duf_set_context_destructor( &pdi->levinfo[d].context, destr );
  }
}


void
duf_levinfo_clear_li( duf_levinfo_t * pli )
{
  assert( pli );
  assert( pli->lev_dh.dfd == 0 );
  if ( pli->itemname )
  {
    /* DUF_ERROR( "CLEAR %s %p", pli->itemname, pli->itemname ); */
    mas_free( pli->itemname );
  }
  pli->itemname = NULL;

  if ( pli->fullpath )
    mas_free( pli->fullpath );
  pli->fullpath = NULL;
  duf_levinfo_clear_context( pli );
  memset( pli, 0, sizeof( *pli ) );
}

void
duf_levinfo_clear_level_d( duf_depthinfo_t * pdi, int d )
{
  assert( pdi );
  assert( d >= 0 );
  duf_levinfo_clear_li( &pdi->levinfo[d] );
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
    if ( d <= pdi->maxdepth )
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
        /* DUF_ERROR( "BEFORE NEW LEVEL %d %s %p", d, pdi->levinfo[d].itemname, pdi->levinfo[d].itemname ); */
        pdi->levinfo[d].itemname = mas_strdup( itemname );
        /* DUF_ERROR( "NEW LEVEL %d %s %p", d, pdi->levinfo[d].itemname, pdi->levinfo[d].itemname ); */
      }
      pdi->levinfo[d].is_leaf = is_leaf ? 1 : 0;
      DUF_OINV_NOT_OPENED( pdi-> );
      DUF_TRACE( explain, 1, "level down: %d; ≪%s≫", d, is_leaf ? "leaf" : "node" );
      assert( pdi->depth >= 0 );
    }
    else
    {
      pdi->depth--;
      r = DUF_ERROR_MAX_DEPTH;
      DUF_TEST_R( r );
    }
    /* assert( duf_pdi_depth( pdi ) == 0 || ( duf_pdi_depth( pdi ) > 0 && duf_levinfo_dirid( pdi ) ) ); */
    if ( is_leaf )
      DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan leaf    =>           - %s", duf_pdi_depth( pdi ), duf_levinfo_itemname( pdi ) );
    else
    {
      duf_levinfo_countdown_dirs( pdi );
      DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": scan node:   =>  by %5llu - %s", duf_pdi_depth( pdi ), duf_levinfo_dirid( pdi ),
                 duf_levinfo_itemname( pdi ) );
    }
  }
  return r;
}

int
duf_levinfo_goup( duf_depthinfo_t * pdi )
{
  int r = 0;

  assert( pdi );

  if ( duf_levinfo_is_leaf( pdi ) )
    DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan leaf  <=             - %s", duf_pdi_depth( pdi ), duf_levinfo_itemname( pdi ) );
  else
    DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": scan node: <=    by %5llu - %s", duf_pdi_depth( pdi ), duf_levinfo_dirid( pdi ),
               duf_levinfo_itemname( pdi ) );
  {
    int d = pdi->depth--;

    if ( duf_levinfo_opened_dh_d( pdi, d ) > 0 )
      r = duf_levinfo_closeat_dh_d( pdi, d );

    assert( pdi->levinfo[d].lev_dh.dfd == 0 );

    DUF_TEST_R( r );
    if ( r < 0 )
      DUF_ERROR( "(%d) close error; L%d", r, pdi->depth );
    DUF_TRACE( explain, 1, "level up:   %d", d );
    assert( pdi->levinfo );
    duf_levinfo_clear_level_d( pdi, d );
    d = pdi->depth;
  }
  return r;
}

void
duf_levinfo_countdown_dirs( duf_depthinfo_t * pdi )
{
  assert( pdi );
  assert( pdi->levinfo );
  {
    int d = pdi->depth - 1;

    if ( d >= 0 )
      pdi->levinfo[d].numdir--;
  }
}

int
duf_levinfo_openat_dh( duf_depthinfo_t * pdi )
{
  assert( pdi );
  {
    int r = 0;
    int d = pdi->depth;

    assert( pdi->levinfo );
    assert( d >= 0 );
    /* assert( duf_config->nopen - duf_config->nclose == d ); */

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
      /* DUF_ERROR( "%s", pdi->levinfo[d].is_leaf ? "LEAF" : "NODE" ); */
      /* DUF_PRINTF( 0, "d:%d [%s]", d, pdi->levinfo[d].itemname ); */
      assert( pdi->levinfo[d].itemname );
      if ( d == 0 )
      {
        /* char *sp; */

        assert( *pdi->levinfo[d].itemname == 0 );
        /* sp = mas_strdup( "/" ); */
        /* sp = mas_strcat_x( sp, pdi->levinfo[d].itemname ); */
        r = duf_open_dh( pdhlev, "/" );
        DUF_TRACE( temp, 0, "@@@@@@@@@@@ r: %d", r );
        /* mas_free( sp ); */
      }
      else
      {
        r = duf_openat_dh( pdhlev, pdhuplev, pdi->levinfo[d].itemname, pdi->levinfo[d].is_leaf );
        DUF_TRACE( temp, 0, "@@@@@@@@@@@ r: %d (%s)", r, pdi->levinfo[d].itemname );
      }
      if ( r >= 0 )
      {
        assert( pdhlev->dfd );
      }
      if ( r == DUF_ERROR_OPEN_ENOENT || r == DUF_ERROR_OPENAT_ENOENT )
      {
        pdi->levinfo[d].deleted = 1;
        r = 0;
      }
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
          r = DUF_ERROR_PATH;
      }
      else
        r = DUF_ERROR_MEMORY;
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

/* returns handle >0 */
int
duf_levinfo_opened_dh_d( duf_depthinfo_t * pdi, int d )
{
  assert( pdi );
  {
    int r = 0;

    assert( d >= 0 );
    if ( pdi->opendir || duf_levinfo_dfd_d( pdi, d ) )
      r = duf_opened_dh( &pdi->levinfo[d].lev_dh );
    return r;
  }
}

/* returns handle >0 */
int
duf_levinfo_opened_dh( duf_depthinfo_t * pdi )
{
  assert( pdi );
  {
    int r = 0;
    int d = pdi->depth;

    assert( d >= 0 );
    r = duf_levinfo_opened_dh_d( pdi, pdi->depth );
    DUF_TEST_R( r );
    return r;
  }
}


/* create level-control array, open 0 level */
int
duf_levinfo_create( duf_depthinfo_t * pdi, int pathdepth )
{
  int r = 0;

  assert( pdi );

  if ( pdi->u.max_rel_depth /* FIXME */  && !pdi->levinfo )
  {
    size_t lsz;

    pdi->maxdepth = pdi->u.max_rel_depth + pathdepth;
    lsz = sizeof( pdi->levinfo[0] ) * ( pdi->maxdepth + 3 );
    /* DUF_DIE( 0, "@@@@@@@ %lu : %u : %lu : %lu", lsz,pdi->maxdepth, sizeof( pdi->levinfo[0] ), sizeof( duf_levinfo_t ) ); */
    pdi->levinfo = mas_malloc( lsz );
    assert( pdi->levinfo );
    memset( pdi->levinfo, 0, lsz );
    assert( pdi->depth == -1 );

    /* r = duf_levinfo_open_dh( pdi, path ); */
  }
  else
  {

  }
  DUF_TEST_R( r );
  return r;
}

/* delete level-control array, close 0 level */
int
duf_levinfo_delete( duf_depthinfo_t * pdi )
{
  int r = 0;

  assert( pdi );
  assert( pdi->levinfo );

  if ( pdi->levinfo )
  {
    while ( r >= 0 && pdi->depth >= 0 )
    {
      duf_levinfo_goup( pdi );
    }
    assert( pdi->depth == -1 );

    mas_free( pdi->levinfo );
    pdi->levinfo = NULL;
  }
  DUF_TEST_R( r );
  return r;
}
