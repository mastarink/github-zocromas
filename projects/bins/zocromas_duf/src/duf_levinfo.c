#include <string.h>
#include <assert.h>
#include <limits.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"


#include "duf_levinfo_ref.h"
#include "duf_levinfo_context.h"
/* ###################################################################### */
#include "duf_levinfo.h"
/* ###################################################################### */



void
duf_levinfo_clear_li( duf_levinfo_t * pli )
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
