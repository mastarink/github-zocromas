#define DUF_SQL_PDI_STMT

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
/* #include <mastar/wrap/mas_memory.h> */

#include "duf_maintenance.h"

#include "duf_levinfo_ref.h"
#include "duf_levinfo_updown.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"

/* ###################################################################### */
#include "duf_pstmt_levinfo.h"
/* ###################################################################### */


/* 20150831.202009 */
int
duf_pstmt_levinfo_godown_dbopenat_dh( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, int is_leaf )
{
  DEBUG_STARTR( r );

  /* Not here : assert( DUF_GET_UFIELD2( dirid) == duf_levinfo_dirid( pdi ) ); */
  DUF_TRACE( scan, 10, "before duf_levinfo_godown() : dirID:%llu", DUF_GET_UFIELD2( dirid ) );
  DUF_TRACE( explain, 2, "@ sel cb2 node" );

#if 0
  DOR( r, duf_levinfo_godown_dbopenat_dnn_dh( pdi, 0 /* dirid?!? */ , DUF_GET_SFIELD2( dfname ), DUF_GET_UFIELD2( ndirs ), DUF_GET_UFIELD2( nfiles ),
                                              is_leaf, pstmt ) );
#else
  DOR( r, duf_levinfo_godown_dbopenat_dh( pdi, is_leaf, pstmt ) );
  /* pdi->levinfo[pdi->depth].numdir = DUF_GET_UFIELD2( ndirs );   */
  /* pdi->levinfo[pdi->depth].numfile = DUF_GET_UFIELD2( nfiles ); */
#endif
  assert( r < 0 || DUF_GET_UFIELD2( dirid ) == duf_levinfo_dirid( pdi ) ); /* was set by duf_levinfo_godown */

  DEBUG_ENDR( r );
}
