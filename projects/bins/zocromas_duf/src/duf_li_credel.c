#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_levinfo_updown.h"

/* ###################################################################### */
#include "duf_li_credel.h"
/* ###################################################################### */



duf_levinfo_t *
duf_li_create( int count )
{
  duf_levinfo_t *pli = NULL;

  if ( count )
  {
    size_t lsz;

    lsz = sizeof( duf_levinfo_t ) * ( count );
    pli = mas_malloc( lsz );
    memset( pli, 0, lsz );
  }
  return pli;
}

void
duf_li_copy( duf_levinfo_t * plidst, const duf_levinfo_t * plisrc, size_t count )
{
  assert( plidst );
  assert( plisrc );
  memcpy( plidst, plisrc, count * sizeof( duf_levinfo_t ) );
  for ( int i = 0; i < count; i++ )
  {
    plidst[i].fullpath = mas_strdup( plisrc[i].fullpath );
    plidst[i].itemname = mas_strdup( plisrc[i].itemname );
    memset( &plidst[i].context, 0, sizeof( plidst[i].context ) );
    assert( !plisrc[i].context.ptr );
    assert( !plisrc[i].context.destructor );
#if 0
    duf_items_copy( plidst[i].items, plisrc[i].items );
#else
    /* it's OK: no allocations at duf_items_t */
#endif
  }
}

duf_levinfo_t *
duf_li_clone( const duf_levinfo_t * plisrc, size_t count )
{
  duf_levinfo_t *pli = NULL;

  pli = duf_li_create( count );
  assert( ( pli && count > 0 ) || ( !pli && count == 0 ) );
  if ( pli )
  {
    duf_li_copy( pli, plisrc, count );
  }
  /* TODO .......... */
  return pli;
}
