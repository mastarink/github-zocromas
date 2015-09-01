#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_context.h"

#include "duf_levinfo_ref.h"
#include "duf_levinfo_context.h"

/* ###################################################################### */
#include "duf_levinfo.h"
/* ###################################################################### */



static void
duf_li_clear( duf_levinfo_t * pli )
{
  assert( pli );
  assert( pli->lev_dh.dfd == 0 );
  if ( pli->itemname )
  {
    /* DUF_SHOW_ERROR( "CLEAR %s %p", pli->itemname, pli->itemname ); */
    mas_free( pli->itemname );
  }
  pli->itemname = NULL;

  if ( pli->fullpath )
    mas_free( pli->fullpath );
  pli->fullpath = NULL;
#if 0  
  duf_levinfo_clear_context( pli );
#else
  duf_clear_context( &pli->context );
#endif
  memset( pli, 0, sizeof( *pli ) );
}

void
duf_levinfo_clear_level_d( duf_depthinfo_t * pdi, int d )
{
  assert( pdi );
  assert( pdi->levinfo );
  assert( d >= 0 );
  duf_li_clear( &pdi->levinfo[d] );
}

/* 20150831.000000 */
void
duf_levinfo_init_level_d( duf_depthinfo_t * pdi, const char *itemname, unsigned long long dirid, unsigned long long ndirs, unsigned long long nfiles,
                          int is_leaf, int d )
{
  assert( pdi );
  assert( pdi->levinfo );
  assert( d >= 0 );
  duf_levinfo_clear_level_d( pdi, d );
  pdi->levinfo[d].lev_dh.dirid = dirid;

  pdi->levinfo[d].dirid = dirid;
  pdi->levinfo[d].numdir = ndirs;
  pdi->levinfo[d].numfile = nfiles;
  if ( itemname )
  {
    /* DUF_SHOW_ERROR( "BEFORE NEW LEVEL %d %s %p", d, pdi->levinfo[d].itemname, pdi->levinfo[d].itemname ); */
    pdi->levinfo[d].itemname = mas_strdup( itemname );
    /* DUF_SHOW_ERROR( "NEW LEVEL %d %s %p", d, pdi->levinfo[d].itemname, pdi->levinfo[d].itemname ); */
  }
  pdi->levinfo[d].is_leaf = is_leaf ? 1 : 0;

}
