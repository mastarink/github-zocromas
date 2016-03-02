/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"

#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/trace/mas_trace.h>

#include "duf_tracen_defs.h"                                         /* MAST_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_config.h"                                              /* duf_get_config ♠ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */

#include "duf_levinfo_ref_def.h"
/* #include "duf_levinfo_context.h" */
#include "duf_levinfo_credel.h"

#include "duf_li_credel.h"
#include "duf_li.h"

#include "duf_dh.h"

#include "duf_pathinfo_ref.h"
#include "duf_pathinfo.h"

#include "duf_pathinfo_ref_def.h"
/* ###################################################################### */
#include "duf_pathinfo_credel.h"
/* ###################################################################### */

void
duf_pi_levinfo_delete( duf_pathinfo_t * pi )
{
  assert( pi );
  if ( pi->levinfo )
  {
#if 0
    duf_li_clear_n( pi->levinfo, pi->maxdepth );
#else
    duf_pi_clear_all( pi );
#endif
    mas_free( pi->levinfo );
    pi->levinfo = NULL;
  }
}

void
duf_pi_init_level_d( duf_pathinfo_t * pi, const char *itemname, unsigned long long dirid, duf_node_type_t node_type, int d )
{
  duf_levinfo_t *li;

  li = duf_pi_ptr_d( pi, d );
  duf_li_init( li, itemname, dirid, node_type );
}

/* 20160120.190820 */
void
duf_pi_dbinit_level_d( duf_pathinfo_t * pi, duf_stmnt_t * pstmt, duf_node_type_t node_type, int d )
{
  assert( pi );
  assert( d >= 0 );
  assert( pi->levinfo );

  duf_pi_clear_d( pi, d );

  if ( d >= 0 /* pdi->opendir */  )
  {
    duf_levinfo_t *pli;

    assert( !pi->levinfo[d].itemname );
    pli = &pi->levinfo[d];

    duf_li_dbinit( pli, pstmt, node_type, d );
    MAST_TRACE( levinfo, 10, "levinfo openated %s; dfd:%d", pi->levinfo[d].itemname, pli->lev_dh.dfd );
  }
}

int
duf_pi_levinfo_create( duf_pathinfo_t * pi, size_t maxdepth )
{
  DUF_STARTR( r );
  duf_levinfo_t *pli = NULL;

  pli = duf_li_create( maxdepth );
  DOR( r, duf_pi_levinfo_set( pi, pli, maxdepth ) );

  DUF_ENDR( r );
}

void
duf_pi_copy( duf_pathinfo_t * pidst, const duf_pathinfo_t * pisrc, int no_li )
{
  assert( pidst );
  assert( pisrc );
  memcpy( pidst, pisrc, sizeof( duf_pathinfo_t ) );
  pidst->levinfo = ( no_li ) ? NULL : duf_li_clone( pisrc->levinfo, pisrc->maxdepth );
}

int
duf_pi_shut( duf_pathinfo_t * pi )
{
  DUF_STARTR( r );

  pi->depth = pi->topdepth = 0;
  pi->maxdepth = 0;
  assert( !pi->maxdepth );
  assert( !pi->topdepth );
  assert( !pi->depth );
  assert( !pi->levinfo );

  DUF_ENDR( r );
}
