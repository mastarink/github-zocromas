#include <string.h>
#include <assert.h>
#include <limits.h>
#include <stdlib.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"
#include "duf_dh.h"

#include "duf_sql.h"
#include "duf_sql_field.h"

#include "duf_dbg.h"

#include "duf_pdi.h"
/* ###################################################################### */
#include "duf_levinfo.h"
/* ###################################################################### */

int
duf_levinfo_is_leaf_d( duf_depthinfo_t * pdi, int d )
{

  assert( pdi );
  assert( pdi->levinfo );
  assert( d >= 0 );
  return pdi->levinfo[d].is_leaf;
}

int
duf_levinfo_is_leaf( duf_depthinfo_t * pdi )
{
  assert( pdi );
  assert( pdi->levinfo );
  return duf_levinfo_is_leaf_d( pdi, pdi->depth );
}

int
duf_levinfo_dirid( duf_depthinfo_t * pdi )
{

  assert( pdi );
  assert( pdi->levinfo );
  {
    int d;

    d = pdi->depth;
    assert( d >= 0 );
    return pdi->levinfo[d].dirid;
  }
}

int
duf_levinfo_numdir_d( const duf_depthinfo_t * pdi, int d )
{
  assert( pdi );
  assert( pdi->levinfo );
  return d >= 0 ? pdi->levinfo[d].numdir : -1;
}

void
duf_levinfo_set_eod_d( const duf_depthinfo_t * pdi, int d )
{
  assert( pdi );
  assert( pdi->levinfo );
  assert( d >= 0 );
  pdi->levinfo[d].eod = 1;
}

void
duf_levinfo_set_eod( const duf_depthinfo_t * pdi )
{
  assert( pdi );
  duf_levinfo_set_eod_d( pdi, pdi->depth );
}

int
duf_levinfo_eod_d( const duf_depthinfo_t * pdi, int d )
{
  assert( pdi );
  assert( pdi->levinfo );
  assert( d >= 0 );
  return /* duf_pdi_deltadepth( pdi, d ) <= 0 || */ pdi->levinfo[d].eod /*|| pdi->levinfo[d].is_leaf */ ;
}

const char *
duf_levinfo_path( const duf_depthinfo_t * pdi )
{
  char *path = NULL;

  assert( pdi );
  assert( pdi->levinfo );
  {
    int d;

    d = pdi->depth;
    assert( d >= 0 );
    if ( pdi->levinfo[d].is_leaf )
      d--;
    {
      assert( d >= 0 );
      if ( pdi->levinfo[d].fullpath )
      {
        path = pdi->levinfo[d].fullpath;
      }
      else
        /* if ( pdi->path ) */
      {
        /* size_t lenp = strlen( pdi->path ); */
        size_t len = 2;
        char *p;

        for ( int i = 0; i <= d; i++ )
        {
          assert( pdi->levinfo[i].itemname );
          len += strlen( pdi->levinfo[i].itemname ) + 1;
        }
        path = mas_malloc( len );
        /* path = strcpy( path, pdi->path ); */
        p = path;
        if ( *( p - 1 ) != '/' )
          *p++ = '/';
        *p = 0;
        for ( int i = 0; i <= d; i++ )
        {
          strcpy( p, pdi->levinfo[i].itemname );
          p += strlen( pdi->levinfo[i].itemname );
          *p++ = '/';
          *p = 0;
        }
        assert( d >= 0 );
        pdi->levinfo[d].fullpath = path;
      }
      /* else                                */
      /* {                                   */
      /*   DUF_ERROR( "pdi->path not set" ); */
      /* }                                   */
    }
  }
  if ( !path )
    DUF_ERROR( "no path returned" );
  return path;
}

void
duf_levinfo_set_context( duf_depthinfo_t * pdi, void *ctx )
{
  assert( pdi );

  {
    int d;

    d = pdi->depth;
    assert( d >= 0 );
    pdi->levinfo[d].context = ctx;
  }
}

void *
duf_levinfo_context( duf_depthinfo_t * pdi )
{
  void *ctx = NULL;

  assert( pdi );

  {
    int d;

    d = pdi->depth;
    assert( d >= 0 );
    ctx = pdi->levinfo[d].context;
  }
  return ctx;
}

void *
duf_levinfo_context_up( duf_depthinfo_t * pdi )
{
  void *ctx = NULL;

  assert( pdi );

  {
    int d;

    d = pdi->depth - 1;
    if ( d >= 0 )
      ctx = pdi->levinfo[d].context;
  }
  return ctx;
}

int
duf_levinfo_down( duf_depthinfo_t * pdi, unsigned long long dirid, const char *itemname, unsigned long long ndirs,
                  unsigned long long nfiles, int is_leaf )
{
  int r = 0;

  assert( pdi );
  {
    int d;

    d = ++pdi->depth;
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
        pdi->levinfo[d].itemname = mas_strdup( itemname );
      pdi->levinfo[d].is_leaf = is_leaf ? 1 : 0;
      DUF_OINV_NOT_OPENED( pdi-> );
      DUF_TEST_R( r );
      assert( pdi->depth >= 0 );
    }
    else
    {
      pdi->depth--;
      r = DUF_ERROR_MAX_DEPTH;
    }
  }
  return r;
}

void
duf_levinfo_clear_li( duf_levinfo_t * pli )
{
  assert( pli->lev_dh.dfd == 0 );
  if ( pli->itemname )
    mas_free( pli->itemname );
  pli->itemname = NULL;

  if ( pli->fullpath )
    mas_free( pli->fullpath );
  pli->fullpath = NULL;
  if ( pli->context )
    mas_free( pli->context );
  pli->context = NULL;
  memset( pli, 0, sizeof( *pli ) );
}

void
duf_levinfo_clear_level_d( duf_depthinfo_t * pdi, int d )
{
  assert( pdi );
  assert( d >= 0 );
  duf_levinfo_clear_li( &pdi->levinfo[d] );
}

void
duf_levinfo_up( duf_depthinfo_t * pdi )
{
  assert( pdi );
  DUF_OINV_NOT_OPENED( pdi-> );
  {
    int d = pdi->depth--;

    assert( pdi->levinfo );
    duf_levinfo_clear_level_d( pdi, d );
    d = pdi->depth;
  }
}

void
duf_levinfo_countdown_dirs( duf_depthinfo_t * pdi )
{
  assert( pdi );
  assert( pdi->levinfo );
  {
    int d = pdi->depth - 1;

    assert( d >= 0 );

    /* if ( pdi->levinfo[d].numdir > 0 ) */
    pdi->levinfo[d].numdir--;
  }
}

duf_dirhandle_t *
duf_levinfo_pdh( duf_depthinfo_t * pdi )
{
  duf_dirhandle_t *pdhlev = NULL;

  assert( pdi );
  if ( !duf_config->cli.noopenat && pdi->opendir )
  {
    int d = pdi->depth;

    assert( pdi->levinfo );
    DUF_OINV( pdi-> );
    /* DUF_ERROR( "IS_LEAF:%d", pdi->levinfo[d].is_leaf ); */

    assert( d >= 0 );
    pdhlev = &pdi->levinfo[d].lev_dh;
  }
  return pdhlev;
}

int
duf_levinfo_udfd( duf_depthinfo_t * pdi )
{
  int r = 0;

  assert( pdi );
  if ( !duf_config->cli.noopenat && pdi->opendir )
  {
    int d = pdi->depth - 1;

    assert( pdi->levinfo );
    assert( d >= 0 );
    {
      DUF_OINV( pdi-> );
      DUF_TEST_R( r );
      r = pdi->levinfo[d].lev_dh.dfd;
    }
  }
  return r;
}

int
duf_levinfo_dfd( duf_depthinfo_t * pdi )
{
  int r = 0;

  assert( pdi );
  if ( !duf_config->cli.noopenat && pdi->opendir )
  {
    int d = pdi->depth;

    assert( pdi->levinfo );
    assert( d >= 0 );
    DUF_OINV( pdi-> );
    /* duf_dirhandle_t *pdhlev = &pdi->levinfo[d].lev_dh; */

    /* r = pdhlev->dfd; */
    r = pdi->levinfo[d].lev_dh.dfd;
  }
  return r;
}

struct stat *
duf_levinfo_stat( duf_depthinfo_t * pdi )
{
  struct stat *pst = NULL;

  assert( pdi );
  if ( !duf_config->cli.noopenat && pdi->opendir )
  {
    int d = pdi->depth;

    assert( pdi->levinfo );
    assert( d >= 0 );
    DUF_OINV( pdi-> );
    if ( pdi->levinfo[d].lev_dh.rs >= 0 )
      pst = &pdi->levinfo[d].lev_dh.st;
  }
  return pst;
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
    DUF_OINVC( pdi-> );

    duf_levinfo_t *pli, *pliu;

    pli = &pdi->levinfo[d];
    pliu = d > 0 ? &pdi->levinfo[d - 1] : NULL;
    /* ????? What was this ???
       if(d>0)
       pdi->levinfo[d - 1].lev_dh.dfd = 0;
     */
    if ( !duf_config->cli.noopenat && pdi->opendir )
    {
      duf_dirhandle_t *pdhlev = &pli->lev_dh;
      duf_dirhandle_t *pdhuplev = pliu ? &pliu->lev_dh : NULL;

      /* if ( S_ISBLK( stX.st_mode ) ) */
      /* {                             */
      /* }                             */
      DUF_OINVC( pdi-> );
      /* DUF_ERROR( "%s", pdi->levinfo[d].is_leaf ? "LEAF" : "NODE" ); */
      /* DUF_PRINTF( 0, "d:%d [%s]", d, pdi->levinfo[d].itemname ); */
      if ( d == 0 )
      {
        char *sp;

        sp = mas_strdup( "/" );
        sp = mas_strcat_x( sp, pdi->levinfo[d].itemname );
        r = duf_open_dh( pdhlev, sp );
        mas_free( sp );
      }
      else
        r = duf_openat_dh( pdhlev, pdhuplev, pdi->levinfo[d].itemname, pdi->levinfo[d].is_leaf );
      DUF_OINV( pdi-> );
      if ( r >= 0 )
        assert( pdhlev->dfd );
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
    DUF_OINV( pdi-> );

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

    if ( !duf_config->cli.noopenat && pdi->opendir )
    {
      assert( d >= 0 );
      duf_dirhandle_t *pdhlev = &pdi->levinfo[d].lev_dh;

      /* if ( S_ISBLK( stX.st_mode ) ) */
      /* {                             */
      /* }                             */
      DUF_OINV( pdi-> );

      {
        if ( r >= 0 && real_path )
          r = duf_open_dh( pdhlev, real_path );
      }
      DUF_OINV( pdi-> );
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

int
duf_levinfo_closeat_dh_d( duf_depthinfo_t * pdi, int d )
{
  assert( pdi );
  {
    int r = 0;

    DUF_OINV( pdi-> );
    assert( d >= 0 );
    if ( !duf_config->cli.noopenat && pdi->opendir )
      r = duf_close_dh( &pdi->levinfo[d].lev_dh );
    DUF_OINVC( pdi-> );
    DUF_TEST_R( r );
    return r;
  }
}

int
duf_levinfo_closeat_dh( duf_depthinfo_t * pdi )
{
  assert( pdi );
  {
    int r = 0;
    int d = pdi->depth;

    assert( d >= 0 );
    r = duf_levinfo_closeat_dh_d( pdi, pdi->depth );
    DUF_TEST_R( r );
    return r;
  }
}

/* create level-control array, open 0 level */
int
duf_levinfo_create( duf_depthinfo_t * pdi, const char *path )
{
  int r = 0;

  assert( pdi );
  if ( pdi->u.maxreldepth && !pdi->levinfo )
  {
    size_t lsz;

    pdi->maxdepth = pdi->u.maxreldepth + duf_pathdepth( path );
    lsz = sizeof( pdi->levinfo[0] ) * ( pdi->maxdepth + 3 );
    /* DUF_DIE( 0, "@@@@@@@ %lu : %u : %lu : %lu", lsz,pdi->maxdepth, sizeof( pdi->levinfo[0] ), sizeof( duf_levinfo_t ) ); */
    pdi->levinfo = mas_malloc( lsz );
    assert( pdi->levinfo );
    memset( pdi->levinfo, 0, lsz );
    assert( pdi->depth == -1 );

    /* r = duf_levinfo_open_dh( pdi, path ); */
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
      int d = pdi->depth--;

      r = duf_levinfo_closeat_dh_d( pdi, d );
      if ( r < 0 )
        DUF_ERROR( "close error" );
      duf_levinfo_clear_level_d( pdi, d );
    }
    /* pdi->depth++; */
    assert( pdi->depth == -1 );
    /* mas_free( pdi->path ); */
    /* pdi->path = NULL;      */

    mas_free( pdi->levinfo );
    pdi->levinfo = NULL;
  }
  DUF_TEST_R( r );
  return r;
}
