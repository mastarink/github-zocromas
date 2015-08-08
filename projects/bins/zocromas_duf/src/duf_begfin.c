#include <assert.h>
#include <libgen.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_status_ref.h"

/* #include "duf_path2db.h" */

#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_ufilter.h"
#include "duf_ufilter_bind.h"

/* ###################################################################### */
#include "duf_begfin.h"
/* ###################################################################### */


static int
duf_bind_ufilter( duf_sqlite_stmt_t * pstmt )
{
  DEBUG_STARTR( r );
  DOR( r, duf_bind_ufilter_uni( pstmt ) );
  duf_ufilter_delete( global_status.selection_bound_ufilter );
  global_status.selection_bound_ufilter = duf_ufilter_create_from( duf_config->pu );

  DEBUG_ENDR( r );
}

int
duf_eval_sql_sequence( duf_sql_sequence_t * ssql, int bind, const char *title )
{
  DEBUG_STARTR( r );

  if ( ssql /* && !ssql->done */ )
  {
    const char **psql = ssql->sql;
#ifdef MAS_TRACING
    const char **psql0 = psql;
#endif

    while ( r >= 0 && psql && *psql )
    {
#ifdef MAS_TRACING
      int changes = 0;
#else
      int DUF_UNUSED changes = 0;
#endif
      DUF_TRACE( sql, 0, "beginning psql : %s", *psql );
      /* r = duf_sql( *p, &changes ); */

      {
        DUF_SQL_START_STMT_NOPDI( *psql, r, pstmt );
        if ( bind )
          DOR( r, duf_bind_ufilter( pstmt ) );
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
