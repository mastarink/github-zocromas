#include <string.h>

#include "duf_maintenance.h"

#include "duf_levinfo_updown.h"
#include "duf_levinfo_ref.h"

#include "duf_li.h"


#include "duf_context.h"

#include "duf_pdi.h"
#include "duf_path2dirid.h"

/* ###################################################################### */
#include "duf_li_credel.h"
/* ###################################################################### */



duf_levinfo_t *
duf_li_create( int maxcount )
{
  duf_levinfo_t *pli = NULL;

  if ( maxcount )
  {
    size_t lsz;

    lsz = sizeof( duf_levinfo_t ) * ( maxcount );
    pli = mas_malloc( lsz );
    memset( pli, 0, lsz );
  }
  return pli;
}

void
duf_li_clear( duf_levinfo_t * pli )
{
  assert( pli );
  assert( pli->lev_dh.opened_copy || pli->lev_dh.dfd == 0 );
  if ( pli->itemname )
    mas_free( pli->itemname );
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
duf_li_clear_n( duf_levinfo_t * pli, unsigned maxcount )
{
  for ( unsigned i = 0; i < maxcount; i++ )
  {
    duf_li_clear( pli + i );
  }
}

void
duf_li_delete( duf_levinfo_t * pli, unsigned maxcount )
{
  duf_li_clear_n( pli, maxcount );
  mas_free( pli );
}

static void
duf_li_copy( duf_levinfo_t * plidst, const duf_levinfo_t * plisrc, unsigned maxcount )
{
  assert( plidst );
  assert( plisrc );
  memcpy( plidst, plisrc, maxcount * sizeof( duf_levinfo_t ) );
  for ( unsigned i = 0; i < maxcount; i++ )
  {
    plidst[i].fullpath = mas_strdup( plisrc[i].fullpath );
    plidst[i].itemname = mas_strdup( plisrc[i].itemname );
    plidst[i].lev_dh.opened_copy = 1;
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
duf_li_clone( const duf_levinfo_t * plisrc, unsigned maxcount )
{
  duf_levinfo_t *pli = NULL;

  pli = duf_li_create( maxcount );
  assert( ( pli && maxcount > 0 ) || ( !pli && maxcount == 0 ) );
  if ( pli )
  {
    duf_li_copy( pli, plisrc, maxcount );
  }
  /* TODO .......... */
  return pli;
}

duf_levinfo_t *
duf_dirid2li( unsigned long long dirid, const char *leaf_itemtruename DUF_UNUSED, unsigned maxcount, int *pr )
{
  duf_levinfo_t *pli = NULL;
  int rpr = 0;

  pli = duf_li_create( maxcount );


  {
    int rpr = 0;
    int count = 0;
    char *name = NULL;
    duf_levinfo_t *plirev = NULL;

    plirev = duf_li_create( maxcount );

    duf_depthinfo_t di = { 0 };
    DOR( rpr, duf_pdi_init_min( &di, NULL /* real_path */  ) );
    do
    {
      name = duf_dirid2name_existed( &di, dirid, &dirid, &rpr );
      if ( name )
      {
        plirev[count].itemname = name;
        plirev[count].dirid = dirid;
        count++;
      }
    }
    while ( name );
    for ( int i = 0; i < count; i++ )
    {
      pli[i] = plirev[count - i - 1];
      /* reverse */
    }
    if ( leaf_itemtruename )
    {
      pli[count].itemname = mas_strdup( leaf_itemtruename );
      pli[count].dirid = dirid;
      count++;
    }
#if 0
    {
      char *t;

      t = duf_li_path( pli, count );
      T( "%s", t );
      mas_free( t );
    }
#endif
    duf_pdi_shut( &di );
    mas_free( plirev );
  }
  if ( pr )
    *pr = rpr;

  return pli;
}
