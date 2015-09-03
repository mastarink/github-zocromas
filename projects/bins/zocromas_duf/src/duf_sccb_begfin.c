#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_sccb.h"
#include "duf_begfin.h"

/* ###################################################################### */
#include "duf_sccb_begfin.h"
/* ###################################################################### */


int
duf_scan_beginning_sql( const duf_scan_callbacks_t * sccb, char *selected_db )
{
  DEBUG_STARTR( r );

  DUF_TRACE( sql, 0, "beginning_sql '%s'",
             ( sccb && sccb->beginning_sql_seq && sccb->beginning_sql_seq->sql ) ? *sccb->beginning_sql_seq->sql : "?" );
  duf_eval_sql_sequence( sccb->beginning_sql_seq, 1 /* bind */ , duf_uni_scan_action_title( sccb ) /* title */ , selected_db );
  DEBUG_ENDR( r );
}

int
duf_scan_final_sql( const duf_scan_callbacks_t * sccb )
{
  DEBUG_STARTR( r );

  duf_eval_sql_sequence( sccb->final_sql_seq, 1 /* bind */ , duf_uni_scan_action_title( sccb ) /* title */, NULL /* selected.db */ );
  DEBUG_ENDR( r );
}


#if 0
int
duf_scan_final_sql( const duf_scan_callbacks_t * sccb )
{
  DEBUG_STARTR( r );

  /* if ( changes ) */
  {
    const char **psql = sccb->final_sql_seq;

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
                 psql - sccb->final_sql_seq, *psql, changes, r < 0 ? "FAIL" : "OK" );
      psql++;
    }
  }
  DEBUG_ENDR( r );
}
#endif
