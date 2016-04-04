/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs.h>                             /* MASE_TEST_R; MASE_TEST_R_LOWERE; ... */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_li.h"

#include "duf_context.h"

#include "duf_pdi.h"                                                 /* duf_pdi_init; duf_pdi_shut; duf_pdi_close ✗ */

#include "duf_pdi_structs.h"
#include "duf_levinfo_structs.h"

/* ###################################################################### */
#include "duf_li_credel.h"
/* ###################################################################### */
static size_t
li_size_array( unsigned maxdepth, unsigned r )
{
  return sizeof( duf_levinfo_t ) * ( maxdepth + 1 + r );
}

/* duf_levinfo_t *                          */
/* CRX(li_create_array, unsigned maxdepth ) */
SRX( LI, duf_levinfo_t *, pli, NULL, li_create_array, unsigned maxdepth )
{
/* duf_levinfo_t *pli = NULL; */

  if ( maxdepth )
  {
    size_t lsz;

    lsz = li_size_array( maxdepth, 1 ) /* +1 more for duf_levinfo_calc_depth */ ;
    pli = mas_malloc( lsz );
    memset( pli, 0, lsz );
    assert( pli[maxdepth + 1].d == 0 );
  }
/* return pli; */
  ERX( LI, duf_levinfo_t *, pli, NULL, li_create_array, unsigned maxdepth );
}

/* void                                */
/* CRX(li_clear, duf_levinfo_t * pli ) */
SRN( LI, void, li_clear, duf_levinfo_t * pli )
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
  CRX(levinfo_clear_context, pli );
#else
/* CRX(clear_context, &pli->context ); */
#endif
  memset( pli, 0, sizeof( *pli ) );
  ERN( LI, void, li_clear, duf_levinfo_t * pli );
}

/* int                                                                 */
/* CRX(li_closed_array, const duf_levinfo_t * pli, unsigned maxdepth ) */
SRX( LI, int, closed, 1, li_closed_array, const duf_levinfo_t * pli, unsigned maxdepth )
{
/* int closed = 1; */

  for ( unsigned i = 0; i <= maxdepth; i++ )
  {
    if ( pli && !( pli[i].lev_dh.opened_copy || pli[i].lev_dh.dfd == 0 ) )
      closed = 0;
  }
/* return closed; */
  ERX( LI, int, closed, 1, li_closed_array, const duf_levinfo_t * pli, unsigned maxdepth );
}

/* void                                                         */
/* CRX(li_clear_array, duf_levinfo_t * pli, unsigned maxdepth ) */
SRN( LI, void, li_clear_array, duf_levinfo_t * pli, unsigned maxdepth )
{
  for ( unsigned i = 0; i <= maxdepth; i++ )
  {
    assert( pli[i].lev_dh.opened_copy || pli[i].lev_dh.dfd == 0 );
    CRX( li_clear, pli + i );
  }
  ERN( LI, void, li_clear_array, duf_levinfo_t * pli, unsigned maxdepth );
}

/* void                                                                                                          */
/* CRX(li_init, duf_levinfo_t * pli, const char *itemname, unsigned long long dirid, duf_node_type_t node_type ) */
SRN( LI, void, li_init, duf_levinfo_t * pli, const char *itemname, unsigned long long dirid, duf_node_type_t node_type )
{
  assert( pli );

  CRX( li_clear, pli );
  assert( !pli->itemname );

  pli->node_type = node_type;
  pli->db.dirid = dirid;

  if ( itemname )
  {
    assert( !pli->itemname );
    pli->itemname = mas_strdup( itemname );
  }
  ERN( LI, void, li_init, duf_levinfo_t * pli, const char *itemname, unsigned long long dirid, duf_node_type_t node_type );
}

/* void                                                          */
/* CRX(li_delete_array, duf_levinfo_t * pli, unsigned maxdepth ) */
SRN( LI, void, li_delete_array, duf_levinfo_t * pli, unsigned maxdepth )
{
  CRX(li_clear_array, pli, maxdepth );
  mas_free( pli );
  ERN( LI, void, li_delete_array, duf_levinfo_t * pli, unsigned maxdepth );
}

static
/*   void                                                                                       */
/* CRX(li_copy_array, duf_levinfo_t * plidst, const duf_levinfo_t * plisrc, unsigned maxdepth ) */
SRN( LI, void, li_copy_array, duf_levinfo_t * plidst, const duf_levinfo_t * plisrc, unsigned maxdepth )
{
  assert( plidst );
  assert( plisrc );
  memcpy( plidst, plisrc, li_size_array( maxdepth, 1 ) );
  for ( unsigned i = 0; i <= maxdepth; i++ )
  {
    plidst[i].fullpath = mas_strdup( plisrc[i].fullpath );
    plidst[i].itemname = mas_strdup( plisrc[i].itemname );
    plidst[i].lev_dh.opened_copy = 1;
  /* memset( &plidst[i].context, 0, sizeof( plidst[i].context ) ); */
#if 0
    CRX(items_copy, plidst[i].items, plisrc[i].items );
#else
  /* it's OK: no allocations at duf_items_t */
#endif
  }
  ERN( LI, void, li_copy_array, duf_levinfo_t * plidst, const duf_levinfo_t * plisrc, unsigned maxdepth );
}

/* duf_levinfo_t *                                                       */
/* CRX(li_clone_array, const duf_levinfo_t * plisrc, unsigned maxdepth ) */
SRX( LI, duf_levinfo_t *, pli, NULL, li_clone_array, const duf_levinfo_t * plisrc, unsigned maxdepth )
{
/* duf_levinfo_t *pli = NULL; */

  pli = CRX( li_create_array, maxdepth );
  assert( ( pli && maxdepth > 0 ) || ( !pli && maxdepth == 0 ) );
  if ( pli )
  {
    CRX( li_copy_array, pli, plisrc, maxdepth );
  }
/* TODO .......... */
/* return pli; */
  ERX( LI, duf_levinfo_t *, pli, NULL, li_clone_array, const duf_levinfo_t * plisrc, unsigned maxdepth );
}

SRP( LI, duf_levinfo_t *, pli, NULL, nameid2li, unsigned long long nameid, unsigned maxdepth )
{
  unsigned count = 0;
  unsigned long long dirid = 0;

  duf_levinfo_t *plirev = NULL;

  plirev = CRX(li_create_array, maxdepth );
  {
    duf_depthinfo_t di = {.pdi_name = "nameid2li" };
    CR( pdi_init_min_r, &di, NULL /* real_path */  );
/* cdirid = dirid; */
    CR( nameid2li_existed, &di, nameid, &plirev[count], &dirid );
    if ( QNOERR )
      count++;
    assert( count <= maxdepth );
    while ( QNOERR )
    {
      CR( dirid2li_existed, &di, dirid, &plirev[count], &dirid );
      if ( QISERR || !plirev[count].itemname )
        break;
    /* plirev[count].itemname = name; */
    /* plirev[count].db.dirid = pdirid; */
    /* pdirid = cdirid; */
    /* plirev[count].node_type = DUF_NODE_NODE; */
    /* plirev[count].source = DUF_DH_SOURCE_DB; */
    /* T( "@@@count:%d [%llu:%llu] %s", count, dirid, plirev[count].db.dirid, plirev[count].itemname ); */
      assert( count <= maxdepth );
      count++;
    }
    if ( QNOERR )
    {
      pli = CRX(li_create_array, maxdepth );
      for ( unsigned i = 0; i < count; i++ )
      {
        pli[i] = plirev[count - i - 1];
        pli[i].d = i;
      /* reverse */
      }
    }
    CRX(pdi_close, &di );
  }
  mas_free( plirev );

  ERP( LI, duf_levinfo_t *, pli, NULL, nameid2li, unsigned long long nameid, unsigned maxdepth );
}
