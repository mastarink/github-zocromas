#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"

#include "duf_sql.h"
#include "duf_sql_field.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_levinfo.h"
/* ###################################################################### */

/* create level-control array, open 0 level */
int
duf_levinfo_create( duf_depthinfo_t * pdi, const char *path )
{
  int r = 0;

  assert( pdi );
  if ( pdi->u.maxdepth && !pdi->levinfo )
  {
    size_t lsz = sizeof( pdi->levinfo[0] ) * ( pdi->u.maxdepth + 3 );

    pdi->levinfo = mas_malloc( lsz );
    assert( pdi->levinfo );
    memset( pdi->levinfo, 0, lsz );
    assert( pdi->depth == 0 );
    pdi->path = mas_strdup( path );

    /* assert( pdi->depth > 0 ); */
    /* assert( pdi->levinfo[pdi->depth - 1].lev_dh.dfd == 0 ); */
    r = duf_levinfo_open_dh( pdi, path );




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

  if ( pdi->levinfo )
  {
    mas_free( pdi->path );
    pdi->path = NULL;

    if ( pdi->levinfo[0].dirname )
      mas_free( pdi->levinfo[0].dirname );
    pdi->levinfo[0].dirname = NULL;

    if ( pdi->levinfo[0].context )
      mas_free( pdi->levinfo[0].context );
    pdi->levinfo[0].context = NULL;

    assert( pdi->depth == 0 );
    duf_levinfo_closeat_dh( pdi );

    mas_free( pdi->levinfo );
    pdi->levinfo = NULL;
  }
  DUF_TEST_R( r );
  return r;
}

const char *
duf_levinfo_path( const duf_depthinfo_t * pdi, const char *tail )
{
  char *path = NULL;

  assert( pdi );
  {
    int d;

    d = pdi->depth;
    if ( pdi->levinfo && pdi->path && !pdi->levinfo[d].fullpath )
    {
      size_t lenp = strlen( pdi->path );
      size_t len = lenp + 2;
      char *p;

      if ( tail )
        len += strlen( tail );

      for ( int i = 1; i < pdi->depth; i++ )
      {
        len += strlen( pdi->levinfo[i].dirname ) + 1;
      }
      path = mas_malloc( len );
      strcpy( path, pdi->path );
      p = path + lenp;
      if ( lenp && *( p - 1 ) != '/' )
        *p++ = '/';
      *p = 0;
      for ( int i = 1; i < pdi->depth; i++ )
      {
        strcpy( p, pdi->levinfo[i].dirname );
        p += strlen( pdi->levinfo[i].dirname );
        *p++ = '/';
        *p = 0;
      }
      if ( tail )
        strcpy( p, tail );
      pdi->levinfo[d].fullpath = path;
    }
  }
  return path;
}

void
duf_levinfo_set_context( duf_depthinfo_t * pdi, void *ctx )
{
  assert( pdi );

  {
    int d;

    d = pdi->depth;
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
duf_levinfo_down( duf_depthinfo_t * pdi, unsigned long long dirid, const char *dirname, unsigned long long ndirs,
                  unsigned long long nfiles, int is_leaf )
{
  int r = 0;

  assert( pdi );
  {
    int d;

    d = ++pdi->depth;
    if ( d <= pdi->u.maxdepth )
    {
      assert( pdi->levinfo );
      memset( &pdi->levinfo[d], 0, sizeof( pdi->levinfo[d] ) );
      pdi->levinfo[d].lev_dh.dirid = dirid;

      pdi->levinfo[d].dirid = dirid;
      pdi->levinfo[d].ndirs = ndirs;
      pdi->levinfo[d].nfiles = nfiles;
      if ( dirname )
        pdi->levinfo[d].dirname = mas_strdup( dirname );
      pdi->levinfo[d].is_leaf = is_leaf ? 1 : 0;
      DUF_OINV_NOT_OPENED( pdi-> );
      DUF_TEST_R( r );
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
duf_levinfo_up( duf_depthinfo_t * pdi )
{
  assert( pdi );
  DUF_OINV_NOT_OPENED( pdi-> );
  if ( pdi )
  {
    int d = pdi->depth--;

    assert( d >= 0 );
    assert( pdi->levinfo );
    /* if ( d >= 0 ) */
    {
      /* d>=0  */
      if ( pdi->levinfo[d].dirname )
        mas_free( pdi->levinfo[d].dirname );
      pdi->levinfo[d].dirname = NULL;

      if ( pdi->levinfo[d].fullpath )
        mas_free( pdi->levinfo[d].fullpath );
      pdi->levinfo[d].fullpath = NULL;
      if ( pdi->levinfo[d].context )
      {
        mas_free( pdi->levinfo[d].context );
      }
      pdi->levinfo[d].context = NULL;

      memset( &pdi->levinfo[d], 0, sizeof( pdi->levinfo[d] ) );
    }
  }
}

void
duf_levinfo_countdown_dirs( duf_depthinfo_t * pdi )
{
  assert( pdi );
  assert( pdi->levinfo );
  if ( pdi->depth > 0 )
    pdi->levinfo[pdi->depth - 1].ndirs--;
}

duf_dirhandle_t *
duf_levinfo_pdh( duf_depthinfo_t * pdi )
{
  duf_dirhandle_t *pdh = NULL;

  assert( pdi );
  if ( !duf_config->cli.noopenat && pdi->opendir )
  {
    int d = pdi->depth;

    assert( pdi->levinfo );
    DUF_OINV( pdi-> );
    /* DUF_ERROR( "IS_LEAF:%d", pdi->levinfo[d].is_leaf ); */

    pdh = &pdi->levinfo[d].lev_dh;
  }
  return pdh;
}

int
duf_levinfo_udfd( duf_depthinfo_t * pdi )
{
  int r = 0;

  assert( pdi );
  if ( !duf_config->cli.noopenat && pdi->opendir )
  {
    int d = pdi->depth;

    assert( pdi->levinfo );
    DUF_OINV( pdi-> );
    /* DUF_ERROR( "IS_LEAF:%d", pdi->levinfo[d].is_leaf ); */
    duf_dirhandle_t *pdhu = &pdi->levinfo[d - 1].lev_dh;

    DUF_TEST_R( r );
    r = pdhu->dfd;
  }
  return r;
}

int
duf_levinfo_openat_dh( duf_depthinfo_t * pdi )
{
  assert( pdi );
  {
    int r = 0;
    int d = pdi->depth;

    assert( pdi->levinfo );
    /* assert( duf_config->nopen - duf_config->nclose == d ); */
    DUF_OINV( pdi-> );

    duf_levinfo_t *pli, *pliu;

    pli = &pdi->levinfo[d];
    pliu = d > 0 ? &pdi->levinfo[d - 1] : NULL;
    /* ????? What was this ???
       if(d>0)
       pdi->levinfo[d - 1].lev_dh.dfd = 0;
     */
    if ( !duf_config->cli.noopenat && pdi->opendir )
    {
      duf_dirhandle_t *pdh = &pli->lev_dh;
      duf_dirhandle_t *pdhu = pliu ? &pliu->lev_dh : NULL;

      /* if ( S_ISBLK( stX.st_mode ) ) */
      /* {                             */
      /* }                             */
      DUF_OINV( pdi-> );
      r = duf_openat_dh( pdh, pdhu, pdi->levinfo[d].dirname );
      DUF_OINV( pdi-> );
      if ( r >= 0 )
        assert( pdh->dfd );
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

    assert( pdi->levinfo );
    DUF_OINV( pdi-> );

    /* duf_dirhandle_t *pdhu = &pdi->levinfo[d - 1].lev_dh; */

    /* assert( d > 0 ); */
    /* assert( pdhu->dfd == 0 ); */

    /* Why???? :FIXME: pdhu->dfd = 0; */

    if ( !duf_config->cli.noopenat && pdi->opendir )
    {
      duf_dirhandle_t *pdh = &pdi->levinfo[d].lev_dh;


      /* if ( S_ISBLK( stX.st_mode ) ) */
      /* {                             */
      /* }                             */
      DUF_OINV( pdi-> );


      r = duf_open_dh( pdh, path );


      DUF_OINV( pdi-> );
      if ( r >= 0 )
        assert( pdh->dfd );
    }
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

    DUF_OINV( pdi-> );
    if ( !duf_config->cli.noopenat && pdi->opendir )
      r = duf_close_dh( &pdi->levinfo[d].lev_dh );
    DUF_OINV( pdi-> );
    DUF_TEST_R( r );
    return r;
  }
}
