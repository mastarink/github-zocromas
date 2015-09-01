#include <assert.h>
#include <libgen.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_status_ref.h"

/* #include "duf_path2db.h" */
#include "duf_maindb.h"

#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_ufilter.h"
#include "duf_ufilter_bind.h"

/* ###################################################################### */
#include "duf_begfin.h"
/* ###################################################################### */

#if 0
static int
duf_bind_ufilter( duf_sqlite_stmt_t * pstmt, const duf_argvc_t * ttarg )
{
  DEBUG_STARTR( r );
  DOR( r, duf_bind_ufilter_uni( pstmt, ttarg ) );
#if 0
  duf_ufilter_delete( global_status.selection_bound_ufilter );
#if 0
  global_status.selection_bound_ufilter = duf_ufilter_create_from( duf_config->pu );
#else
  global_status.selection_bound_ufilter = duf_ufilter_clone( duf_config->pu );
#endif
#endif
  DEBUG_ENDR( r );
}
#endif

int
duf_eval_sql_sequence_cb( duf_sql_sequence_t * ssql, const char *title, duf_bind_cb_t callback, const duf_argvc_t * ttarg )
{
  DEBUG_STARTR( r );

  if ( ssql /* && !ssql->done */  )
  {
    const char **psql = ssql->sql;

#ifdef MAS_TRACING
    const char **psql0 = psql;
#endif
    int nn = 0;

    while ( r >= 0 && psql && *psql )
    {
      nn++;
#ifdef MAS_TRACING
      int changes = 0;
#else
      int DUF_UNUSED changes = 0;
#endif
      DUF_TRACE( sql, 0, "beginning psql #%d: %s", nn, *psql );
      DUF_TRACE( select, 0, "beginning psql #%d: %s", nn, *psql );
      /* r = duf_sql( *p, &changes ); */
      DORF( r, duf_main_db_open );

      {
        DUF_SQL_START_STMT_NOPDI( *psql, r, pstmt );
        if ( callback )
          DOR( r, ( callback ) ( pstmt, ttarg ) );
        if ( r >= 0 )
        {
          DUF_SQL_STEP( r, pstmt );
/* DUF_SHOW_ERROR( "@@@ %s", duf_error_name(r) ); */
          DUF_SQL_CHANGES_NOPDI( changes, r, pstmt );
/* DUF_SHOW_ERROR( "@@@ %s: %s", duf_error_name(r),  *psql); */

        }
        DUF_SQL_END_STMT_NOPDI( r, pstmt );
      }
      DUF_TEST_R( r );
      /* DUF_TRACE( action, 2, "(%d) beginning psql %s; changes:%d", r, *psql, changes ); */
      DUF_TRACE( action, 2, "%" DUF_ACTION_TITLE_FMT ": beginning SQL %lu; [%s] changes:%d; %s", title ? title : "no-title",
                 psql - psql0, *psql, changes, r < 0 ? "FAIL" : "OK" );
      psql++;
    }
    ssql->done++;

  }
  DEBUG_ENDR( r );
}

int
duf_eval_sql_sequence( duf_sql_sequence_t * ssql, int bind, const char *title )
{
  DEBUG_STARTR( r );

#if 0
  DOR( r, duf_eval_sql_sequence_cb( ssql, title, bind ? duf_bind_ufilter : NULL, NULL ) );
#else
  DOR( r, duf_eval_sql_sequence_cb( ssql, title, bind ? duf_bind_ufilter_uni : NULL, NULL ) );
#endif

  DEBUG_ENDR( r );
}
