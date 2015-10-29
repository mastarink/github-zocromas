#include <string.h>

#include "duf_maintenance.h"

#include "duf_context.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"

#include "duf_li.h"
#include "duf_levinfo_ref.h"
#include "duf_levinfo_ref_def.h"
#include "duf_levinfo_context.h"

#include "duf_pstmt_levinfo.h"

/* ###################################################################### */
#include "duf_levinfo.h"
/* ###################################################################### */

#ifndef DUF_NO_NUMS
void
duf_levinfo_set_nums_d( duf_depthinfo_t * pdi, unsigned long long ndirs, unsigned long long nfiles, int d )
{
  duf_levinfo_t *pli;

  assert( pdi );
  assert( d >= 0 );
  pli = &pdi->pathinfo.levinfo[d];
  duf_li_set_nums( pli, ndirs, nfiles );
}
#endif

static void
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
duf_levinfo_clear_level_d( duf_depthinfo_t * pdi, int d )
{
  assert( pdi );
  assert( pdi->pathinfo.levinfo );
  assert( d >= 0 );
  duf_li_clear( &pdi->pathinfo.levinfo[d] );
}
