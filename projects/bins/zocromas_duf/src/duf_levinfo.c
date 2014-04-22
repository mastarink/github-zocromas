#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_levinfo.h"
/* ###################################################################### */

int
duf_levinfo_create( duf_depthinfo_t * pdi )
{
  int r = 0;

  assert( pdi );
  if ( pdi->u.maxdepth && !pdi->levinfo )
  {
    size_t lsz = sizeof( pdi->levinfo[0] ) * ( pdi->u.maxdepth + 3 );

    pdi->levinfo = mas_malloc( lsz );
    assert( pdi->levinfo );
    memset( pdi->levinfo, 0, lsz );
  }
  return r;
}

int
duf_levinfo_delete( duf_depthinfo_t * pdi )
{
  int r = 0;

  assert( pdi );
  mas_free( pdi->levinfo );
  return r;
}

int
duf_levinfo_down( duf_depthinfo_t * pdi, unsigned long long dirid, const char *dirname, unsigned long long ndirs,
                  unsigned long long nfiles )
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
      pdi->levinfo[d].dh.dirid = dirid;

      pdi->levinfo[d].dirid = dirid;
      pdi->levinfo[d].ndirs = ndirs;
      pdi->levinfo[d].nfiles = nfiles;
      if ( dirname )
        pdi->levinfo[d].dirname = mas_strdup( dirname );
    }
  }
  return r;
}

void
duf_levinfo_up( duf_depthinfo_t * pdi )
{
  assert( pdi );
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

int
duf_levinfo_openat_dh( duf_depthinfo_t * pdi )
{
  int r = 0;

  assert( pdi );
  if ( !duf_config->cli.noopenat )
  {
    int d = pdi->depth;

    assert( pdi->levinfo );
    if ( d > 0 )
      r = duf_openat_dh( &pdi->levinfo[d].dh, &pdi->levinfo[d - 1].dh, pdi->levinfo[d].dirname );
    else
      r = duf_openat_dh( &pdi->levinfo[d].dh, NULL, pdi->levinfo[d].dirname );
  }
  return r;
}

int
duf_levinfo_closeat_dh( duf_depthinfo_t * pdi )
{
  int r = 0;

  assert( pdi );
  if ( !duf_config->cli.noopenat )
  {
    int d = pdi->depth;

    r = duf_close_dh( &pdi->levinfo[d].dh );
  }
  return r;
}
