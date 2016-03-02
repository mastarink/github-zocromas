/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/trace/mas_trace.h>

#include "duf_tracen_defs.h"                                         /* MAST_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */

/* #include "duf_levinfo_updown.h" */
/* #include "duf_levinfo_ref.h" */

#include "duf_li.h"

#include "duf_context.h"

#include "duf_pdi.h"
/* #include "duf_path2dirid.h" */

/* ###################################################################### */
#include "duf_li_credel.h"
/* ###################################################################### */

duf_levinfo_t *
duf_li_create( int maxdepth )
{
  duf_levinfo_t *pli = NULL;

  if ( maxdepth )
  {
    size_t lsz;

    lsz = sizeof( duf_levinfo_t ) * ( maxdepth + 3 );
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
duf_li_clear_alln( duf_levinfo_t * pli, unsigned maxdepth )
{
  for ( unsigned i = 0; i < maxdepth; i++ )
  {
    duf_li_clear( pli + i );
  }
}

void
duf_li_init( duf_levinfo_t * pli, const char *itemname, unsigned long long dirid, duf_node_type_t node_type )
{
  assert( pli );

  duf_li_clear( pli );
  assert( !pli->itemname );

  pli->node_type = node_type;
  pli->db.dirid = dirid;

  if ( itemname )
  {
    assert( !pli->itemname );
    pli->itemname = mas_strdup( itemname );
  }
}

void
duf_li_delete( duf_levinfo_t * pli, unsigned maxdepth )
{
  duf_li_clear_alln( pli, maxdepth );
  mas_free( pli );
}

static void
duf_li_copy( duf_levinfo_t * plidst, const duf_levinfo_t * plisrc, unsigned maxdepth )
{
  assert( plidst );
  assert( plisrc );
  memcpy( plidst, plisrc, maxdepth * sizeof( duf_levinfo_t ) );
  for ( unsigned i = 0; i < maxdepth; i++ )
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
duf_li_clone( const duf_levinfo_t * plisrc, unsigned maxdepth )
{
  duf_levinfo_t *pli = NULL;

  pli = duf_li_create( maxdepth );
  assert( ( pli && maxdepth > 0 ) || ( !pli && maxdepth == 0 ) );
  if ( pli )
  {
    duf_li_copy( pli, plisrc, maxdepth );
  }
/* TODO .......... */
  return pli;
}

duf_levinfo_t *
duf_dirid2li( unsigned long long dirid, const char *leaf_itemtruename MAS_UNUSED, unsigned maxdepth, int *pr )
{
  duf_levinfo_t *pli = NULL;
  int rpr = 0;

  pli = duf_li_create( maxdepth );

  {
    int rpr = 0;
    unsigned count = 0;

  /* char *name = NULL; */
    duf_levinfo_t *plirev = NULL;

  /* unsigned long long cdirid, pdirid; */

    plirev = duf_li_create( maxdepth );

    duf_depthinfo_t di = {.pdi_name = "dirid2li" };
    DOR( rpr, duf_pdi_init_min_r( &di, NULL /* real_path */  ) );
  /* cdirid = dirid; */
    while ( DUF_NOERROR( rpr ) )
    {
      DOR( rpr, duf_dirid2li_existed( &di, dirid, &plirev[count], &dirid ) );
      if ( DUF_IS_ERROR( rpr ) || !plirev[count].itemname )
        break;
    /* plirev[count].itemname = name; */
    /* plirev[count].db.dirid = pdirid; */
    /* pdirid = cdirid; */
    /* plirev[count].node_type = DUF_NODE_NODE; */
    /* plirev[count].source = DUF_DH_SOURCE_DB; */
    /* T( "@@@count:%d [%llu:%llu] %s", count, dirid, plirev[count].db.dirid, plirev[count].itemname ); */
      assert( count < maxdepth );
      count++;
    }
    if ( DUF_NOERROR( rpr ) )
    {
      for ( unsigned i = 0; i < count; i++ )
      {
        pli[i] = plirev[count - i - 1];
      /* reverse */
      }
      if ( leaf_itemtruename && count > 0 )
      {
        pli[count].itemname = mas_strdup( leaf_itemtruename );
        pli[count].db.dirid = pli[count - 1].db.dirid;
        pli[count].node_type = DUF_NODE_LEAF;
      /* pli[count].source = DUF_DH_SOURCE_DB; */
      }
    /* for ( unsigned i = 0; i <= count; i++ )                             */
    /* {                                                                   */
    /*   T( "@@%d. dirid:%llu; %s", i, pli[i].db.dirid, pli[i].itemname ); */
    /* }                                                                   */
#if 0
      {
        char *t;

        t = duf_li_path( pli, count );
        T( "%s", t );
        mas_free( t );
      }
#endif
    }
    duf_pdi_close( &di );
    mas_free( plirev );
  }
  if ( pr )
    *pr = rpr;

  return pli;
}

duf_levinfo_t *
duf_nameid2li( unsigned long long nameid, unsigned maxdepth, int *pr )
{
  duf_levinfo_t *pli = NULL;
  int rpr = 0;

  pli = duf_li_create( maxdepth );

  {
    unsigned count = 0;
    unsigned long long dirid = 0;

  /* char *name = NULL; */
    duf_levinfo_t *plirev = NULL;

  /* unsigned long long cdirid, pdirid; */

    plirev = duf_li_create( maxdepth );

    duf_depthinfo_t di = {.pdi_name = "nameid2li" };
    DOR( rpr, duf_pdi_init_min_r( &di, NULL /* real_path */  ) );
  /* cdirid = dirid; */
    DOR( rpr, duf_nameid2li_existed( &di, nameid, &plirev[count], &dirid ) );
    if ( DUF_NOERROR( rpr ) )
      count++;
    assert( count < maxdepth );
    while ( DUF_NOERROR( rpr ) )
    {
      DOR( rpr, duf_dirid2li_existed( &di, dirid, &plirev[count], &dirid ) );
      if ( DUF_IS_ERROR( rpr ) || !plirev[count].itemname )
        break;
    /* plirev[count].itemname = name; */
    /* plirev[count].db.dirid = pdirid; */
    /* pdirid = cdirid; */
    /* plirev[count].node_type = DUF_NODE_NODE; */
    /* plirev[count].source = DUF_DH_SOURCE_DB; */
    /* T( "@@@count:%d [%llu:%llu] %s", count, dirid, plirev[count].db.dirid, plirev[count].itemname ); */
      assert( count < maxdepth );
      count++;
    }
    if ( DUF_NOERROR( rpr ) )
    {
      for ( unsigned i = 0; i < count; i++ )
      {
        pli[i] = plirev[count - i - 1];
        pli[i].d = i;
      /* reverse */
      }
    }
    duf_pdi_close( &di );
    mas_free( plirev );
  }
  if ( pr )
    *pr = rpr;

  return pli;
}
