/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>

#include "duf_tracen_defs.h"                                         /* DUF_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

/* #include "duf_li.h" */
/* #include "duf_levinfo.h" */

#include "duf_pathinfo_credel.h"

/* ###################################################################### */
#include "duf_levinfo_dbinit.h"
/* ###################################################################### */

/* 20151029.114756 */
void
duf_levinfo_dbinit_level_d( duf_depthinfo_t * pdi, duf_stmnt_t * pstmt, duf_node_type_t node_type, int d )
{
  assert( pdi );
/* TODO: via duf_pi_dbinit_level_d; done 20160120.191056 */
#if 0
  assert( d >= 0 );
  assert( pdi->pathinfo.levinfo );

  duf_levinfo_clear_level_d( pdi, d );

  if ( d >= 0 /* pdi->opendir */  )
  {
    duf_levinfo_t *pli;

    assert( !pdi->pathinfo.levinfo[d].itemname );
    pli = &pdi->pathinfo.levinfo[d];

    duf_li_dbinit( pli, pstmt, node_type, d );
    DUF_TRACE( levinfo, 10, "levinfo openated %s; dfd:%d", pdi->pathinfo.levinfo[d].itemname, pli->lev_dh.dfd );
  }
#else
  duf_pi_dbinit_level_d( &pdi->pathinfo, pstmt, node_type, d );
#endif
}
