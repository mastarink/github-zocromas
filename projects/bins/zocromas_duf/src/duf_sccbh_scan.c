#define DUF_SQL_PDI_STMT

#include <assert.h>
#include <string.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"


#include "duf_utils_path.h"     /* duf_realpath */

#include "duf_levinfo_ref.h"

#include "duf_pdi.h"

#include "duf_sccb.h"
#include "duf_sccbh.h"

#include "duf_sccbh_shortcuts.h"
/* ###################################################################### */
#include "duf_sccbh_scan.h"
/* ###################################################################### */


static int
duf_sccbh_eval_real_path( duf_sccb_handle_t * sccbh, const char *real_path )
{
  DEBUG_STARTR( r );

  assert( sccbh );
  DEBUG_STEP(  );

  DOR( r, duf_pdi_reinit( PDI, real_path, PU, duf_get_node_sql_set( SCCB )->selector2, PU->v.flag.recursive, duf_pdi_opendir( PDI ) ) );
  DUF_TRACE( scan, 0, "[%llu] #%llu start scan from pdi path: ≪%s≫;", duf_levinfo_dirid( PDI ), PDI->seq_leaf, duf_levinfo_path( PDI ) );
  DOR( r, duf_sccbh_eval_pdi_and_summary( sccbh ) );
  /* duf_pdi_close( PDI ); */

  DEBUG_ENDR( r );
}

static int
duf_sccbh_eval_path( duf_sccb_handle_t * sccbh, const char *path )
{
  DEBUG_STARTR( r );
  char *real_path = NULL;
  const duf_scan_callbacks_t *sccb = SCCB;

  assert( sccb );
  {
    real_path = duf_realpath( path, &r );
    {
      DUF_TRACE( explain, 0, "start scan from path: ≪%s≫; real: ≪%s≫", path, real_path );
      DUF_TRACE( path, 4, "start scan from path: ≪%s≫; real: ≪%s≫", path, real_path );

      DUF_E_NO( DUF_ERROR_MAX_REACHED, DUF_ERROR_MAX_SEQ_REACHED, DUF_ERROR_TOO_DEEP );
//  duf_scan_qbeginning_sql( sccb ); ==[20140506]==> duf_open_sccb_handle
      DOR( r, duf_sccbh_eval_real_path( sccbh, real_path ) );
    }
    mas_free( real_path );
  }
  DEBUG_ENDR_YES_CLEAR( r, DUF_ERROR_MAX_REACHED, DUF_ERROR_MAX_SEQ_REACHED, DUF_ERROR_TOO_DEEP );
}

int
duf_sccbh_eval_each_path( duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );

  int count = 0;
  char *const *sargv = NULL;

  assert( sccbh );
  /* assert( PARGV ); */

  DUF_TRACE( action, 1, "%" DUF_ACTION_TITLE_FMT ": targc:%u", duf_uni_scan_action_title( SCCB ), PARGC );
  for ( int ia = 0; r >= 0 && ia < PARGC; ia++ )
    DUF_TRACE( action, 1, "%" DUF_ACTION_TITLE_FMT ": targv[%d]='%s'", duf_uni_scan_action_title( SCCB ), ia, PARGV[ia] );

  count = 1;
  sargv = NULL;
  HCHANGES = 0;
  if ( PARGC > 0 )
  {
    count = PARGC;
    sargv = PARGV;
  }
  {
    /* - evaluate sccb for each string from PARG[CV] as path */
    if ( duf_levinfo_path( PDI ) )
      DOR( r, duf_sccbh_eval_pdi_and_summary( sccbh ) );
    else
      for ( int ia = 0; r >= 0 && ia < count; ia++ )
      {
        const char *cargv = NULL;

        if ( sargv )
          cargv = *sargv++;
        DUF_TRACE( path, 0, "@@PARGV[%d]=\"%s\"; cargv=\"%s\"", ia, PARGV ? PARGV[ia] : NULL, cargv );
        DOR( r, duf_sccbh_eval_path( sccbh, cargv ) );
      }
  }
  DUF_TRACE( action, 1, "after scan" );

  DEBUG_ENDR( r );
}
