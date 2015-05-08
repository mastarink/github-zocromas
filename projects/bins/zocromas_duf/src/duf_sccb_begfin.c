#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_service.h"
#include "duf_config_ref.h"
#include "duf_status_ref.h"

#include "duf_sccb.h"

#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_dir_scan2_stages.h"

#include "duf_begfin.h"


/* ###################################################################### */
#include "duf_sccb_begfin.h"
/* ###################################################################### */



int
duf_scan_qbeginning_sql( const duf_scan_callbacks_t * sccb )
{
  DEBUG_STARTR( r );
  if ( !global_status.selection_done )
  {
    DOR( r, duf_scan_beginning_sql( sccb ) );
    /* if ( r >= 0 ) */
    global_status.selection_done = 1;
  }
  DEBUG_ENDR( r );
}

int
duf_scan_beginning_sql( const duf_scan_callbacks_t * sccb )
{
  DEBUG_STARTR( r );
  const char **psql = sccb->beginning_sql_argv;

  duf_scan_beginning_psql( psql, 1 /* bind */ , /* sccb->node.selector2, */ duf_uni_scan_action_title( sccb ) );
  DEBUG_ENDR( r );
}

int
duf_scan_final_sql( const duf_scan_callbacks_t * sccb )
{
  DEBUG_STARTR( r );

  /* if ( changes ) */
  {
    const char **psql = sccb->final_sql_argv;

    while ( r >= 0 && psql && *psql )
    {
      int changes = 0;

      DUF_TRACE( action, 2, "final psql : %s", *psql );
      /* r = duf_sql( *p, &changes ); */

      {
        DUF_SQL_START_STMT_NOPDI( *psql, r, pstmt );
        /* r = duf_sql_exec( *psql, &changes ); */
        DUF_SQL_STEP( r, pstmt );
        DUF_SQL_CHANGES_NOPDI( changes, r, pstmt );
        DUF_SQL_END_STMT_NOPDI( r, pstmt );
      }
      DUF_TEST_R( r );
      /* DUF_TRACE( action, 2, "(%d) final psql %s; changes:%d", r, *psql, changes ); */
      DUF_TRACE( action, 2, "%" DUF_ACTION_TITLE_FMT ": final SQL %lu; [%s] changes:%d; %s", duf_uni_scan_action_title( sccb ),
                 psql - sccb->final_sql_argv, *psql, changes, r < 0 ? "FAIL" : "OK" );
      psql++;
    }
  }
  DEBUG_ENDR( r );
}
