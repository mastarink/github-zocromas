#include <assert.h>
#include <libgen.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_status_ref.h"

/* #include "duf_path2db.h" */
#include "duf_maindb.h"

#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_ufilter.h"
#include "duf_ufilter_bind.h"

#include "duf_selector.h"
/* ###################################################################### */
#include "duf_begfin.h"
/* ###################################################################### */

#if 0
static int
duf_bind_ufilter( duf_sqlite_stmt_t * pstmt, const duf_argvc_t * ttarg )
{
  DEBUG_STARTR( r );
  DOR( r, duf_bind_ufilter_uni( pstmt, ttarg ) );
#  if 0
  duf_ufilter_delete( global_status.selection_bound_ufilter );
#    if 0
  global_status.selection_bound_ufilter = duf_ufilter_create_from( duf_config->pu );
#    else
  global_status.selection_bound_ufilter = duf_ufilter_clone( duf_config->pu );
#    endif
#  endif
  DEBUG_ENDR( r );
}
#endif

static int
duf_eval_sql_one_of_seq_cb( const char *sql, const char *title, duf_bind_cb_t callback, const duf_argvc_t * ttarg, const char *selected_db,
                            int *pchanges )
{
  DEBUG_STARTR( r );
  int changes = 0;
  const char *worksql = NULL;
  char *tmpsql = NULL;

  /* r = duf_sql( *p, &changes ); */
  DORF( r, duf_main_db_open );
  if ( selected_db )
    worksql = tmpsql = duf_expand_selected_db( sql, selected_db );
  else
    worksql = sql;
  {
    DUF_SQL_START_STMT_NOPDI( worksql, r, pstmt );
    if ( callback )
      DOR( r, ( callback ) ( pstmt, ttarg ) );
    if ( r >= 0 )
    {
      DUF_SQL_STEP( r, pstmt );
/* DUF_SHOW_ERROR( "@@@ %s", duf_error_name(r) ); */
      DUF_SQL_CHANGES_NOPDI( changes, r, pstmt );
/* DUF_SHOW_ERROR( "@@@ %s: %s", duf_error_name(r),  worksql); */

    }
    DUF_SQL_END_STMT_NOPDI( r, pstmt );
  }
  /* DUF_TRACE( action, 2, "(%d) beginning psql %s; changes:%d", r, worksql, changes ); */
  if ( tmpsql )
    mas_free( tmpsql );
  if ( pchanges )
    *pchanges = changes;
  DEBUG_ENDR( r );
}

int
duf_eval_sql_sequence_cb( duf_sql_sequence_t * ssql, const char *title, duf_bind_cb_t callback, const duf_argvc_t * ttarg, const char *selected_db )
{
  DEBUG_STARTR( r );

  if ( ssql /* && !ssql->done */  )
  {
    const char **psql = ssql->sql;

#ifdef MAS_TRACING
    const char **psql0 = psql;
#endif
    int nn = 0;

#ifdef MAS_TRACING
    int changes = 0;
#else
    int DUF_UNUSED changes = 0;
#endif

    if ( DUF_NOERROR( r ) && psql0 && *psql0 && ssql->beginend )
    {
      DOR( r, duf_eval_sql_one_of_seq_cb( "BEGIN", title, callback, ttarg, NULL, &changes ) );
    }

    while ( DUF_NOERROR( r ) && psql && *psql )
    {
      nn++;
      DUF_TRACE( sql, 0, "beginning psql #%d: %s", nn, *psql );
      DUF_TRACE( select, 0, "beginning psql #%d: %s", nn, *psql );

      assert( ( ssql->set_selected_db && selected_db ) || ( !ssql->set_selected_db && !selected_db ) );
      DOR( r, duf_eval_sql_one_of_seq_cb( *psql, title, callback, ttarg, ssql->set_selected_db ? selected_db : NULL, &changes ) );

      DUF_TRACE( action, 2, "%" DUF_ACTION_TITLE_FMT ": beginning SQL %lu; [%s] changes:%d; %s", title ? title : "no-title",
                 psql - psql0, *psql, changes, r < 0 ? "FAIL" : "OK" );
      psql++;
    }
    if ( DUF_NOERROR( r ) && psql0 && *psql0 && ssql->beginend )
    {
      DOR( r, duf_eval_sql_one_of_seq_cb( "END", title, callback, ttarg, NULL, &changes ) );
    }
    ssql->done++;

  }
  DEBUG_ENDR( r );
}

int
duf_eval_sql_sequence( duf_sql_sequence_t * ssql, int bind, const char *title, const char *selected_db )
{
  DEBUG_STARTR( r );


#if 0
  DOR( r, duf_eval_sql_sequence_cb( ssql, title, bind ? duf_bind_ufilter : NULL, NULL /* ttarg */ , selected_db ) );
#else
  DOR( r, duf_eval_sql_sequence_cb( ssql, title, bind ? duf_bind_ufilter_uni : NULL, NULL /* ttarg */ , selected_db ) );
#endif

  DEBUG_ENDR( r );
}
