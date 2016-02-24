/* #undef MAS_TRACING */
#include <string.h>

#include "duf_maintenance.h"

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
