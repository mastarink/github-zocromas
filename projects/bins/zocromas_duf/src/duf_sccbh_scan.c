#define DUF_SQL_PDI_STMT

#include <assert.h>
#include <string.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

/* #include "duf_config_ref.h" */

#include "duf_utils_path.h"     /* duf_realpath */

#include "duf_levinfo_ref.h"

#include "duf_pdi.h"

#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_sccb.h"
#include "duf_sccb_handle.h"

static int
duf_sccbh_real_path( duf_sccb_handle_t * sccbh, const char *real_path )
{
  DEBUG_STARTR( r );


  /* duf_depthinfo_t di = {.depth = -1,         */
  /*   .seq = 0,                                */
  /*   .levinfo = NULL,                         */
  /*   .u = *pu,                                */
  /*   .opendir = sccb ? sccb->def_opendir : 0, */
  /*   (* .name = real_path, *)                 */
  /* };                                         */
  /*                                            */
  /* pdi=&di;                                   */

  DEBUG_STEP(  );

  /* assert( di.depth == -1 ); */
  DOR( r, duf_pdi_reinit( sccbh->pdi, real_path, sccbh->pu, sccbh->pu->v.flag.recursive ) );
  DUF_TRACE( scan, 0, "[%llu] #%llu start scan from pdi path: ≪%s≫;", duf_levinfo_dirid( sccbh->pdi ), sccbh->pdi->seq_leaf,
             duf_levinfo_path( sccbh->pdi ) );
  DOR( r, duf_sccb_pdi( SCCBX ) );
  /* (* xchanges = di.changes; --- needless!? *) */
  /* duf_pdi_close( &di ); */

  DEBUG_ENDR( r );
}

static int
duf_sccbh_path( duf_sccb_handle_t * sccbh, const char *path, const duf_ufilter_t * pu,
                const duf_scan_callbacks_t * sccb /*, unsigned long long *pchanges */  )
{
  DEBUG_STARTR( r );
  char *real_path = NULL;

  real_path = duf_realpath( path, &r );

  DUF_TRACE( explain, 0, "start scan from path: ≪%s≫; real: ≪%s≫", path, real_path );

  DUF_E_NO( DUF_ERROR_MAX_REACHED, DUF_ERROR_MAX_SEQ_REACHED, DUF_ERROR_TOO_DEEP );
  if ( sccb )
    DOR( r, duf_sccbh_real_path( sccbh, real_path ) );

  mas_free( real_path );
  DEBUG_ENDR_YES_CLEAR( r, DUF_ERROR_MAX_REACHED, DUF_ERROR_MAX_SEQ_REACHED, DUF_ERROR_TOO_DEEP );
}

int
duf_sccbh_each_path( duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
  sccbh->changes = 0;
  DUF_TRACE( action, 1, "%" DUF_ACTION_TITLE_FMT ": targc:%u", duf_uni_scan_action_title( sccbh->sccb ), sccbh->targc );
  for ( int ia = 0; r >= 0 && ia < sccbh->targc; ia++ )
    DUF_TRACE( action, 1, "%" DUF_ACTION_TITLE_FMT ": targv[%d]='%s'", duf_uni_scan_action_title( sccbh->sccb ), ia, sccbh->targv[ia] );

  if ( sccbh->targc <= 0 )
  {
    /* - evaluate sccb for NULL path */
    DOR( r, duf_sccbh_path( sccbh, NULL, sccbh->pu, sccbh->sccb ) );
  }
  else
  {
    /* - evaluate sccb for each string from sccbh->targ[cv] as path */
    for ( int ia = 0; r >= 0 && ia < sccbh->targc; ia++ )
      DOR( r, duf_sccbh_path( sccbh, sccbh->targv[ia], sccbh->pu, sccbh->sccb ) );
  }

  DUF_TRACE( action, 1, "after scan" );

  DEBUG_ENDR( r );
}
