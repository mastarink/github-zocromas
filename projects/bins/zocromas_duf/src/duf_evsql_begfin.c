/* #undef MAS_TRACING */
#include <assert.h>                                                  /* assert */

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

/* #include "duf_tracen_defs.h"                                         (* MAST_TRACE ♠ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ *) */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ♠ *) */
/* #include "duf_dodefs.h"                                              (* DOR ♠ *) */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_fmt_defs.h"

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_sql_se_stmt_defs.h"                                       /* DUF_SQL_BIND_S_OPT etc. ✗ */
/* #include "duf_sql_stmt_defs.h"                                       (* DUF_SQL_BIND_S_OPT etc. ✗ *) */
#include "duf_sql_prepared.h"                                        /* duf_sql_(prepare|step|finalize) ✗ */
#include "duf_sql_bind.h"                                            /* duf_sql_... for DUF_SQL_BIND_... etc. ✗ */

#include "duf_ufilter.h"
#include "duf_ufilter_bind.h"

#include "duf_pdi_global.h"                                          /* duf_pdi_global() ✗ */
#include "duf_pdi_stmt.h"                                            /* duf_pdi_find_statement_by_id; etc. ✗ */

/* #include "duf_evsql_selector.h" */
#include "duf_evsql_selector_new.h"

/* ###################################################################### */
#include "duf_evsql_begfin.h"                                        /* duf_eval_sqlsq() ✗ */
/* ###################################################################### */

#if 0
static int
duf_bind_ufilter( duf_stmnt_t * pstmt, const mas_argvc_t * ttarg )
{
  DUF_STARTR( r );
  DOR( r, duf_bind_ufilter_uni( pstmt, ttarg ) );
# if 0
  duf_ufilter_delete( global_status.selection_bound_ufilter );
#  if 0
  global_status.selection_bound_ufilter = duf_ufilter_create_from( DUF_CONFIGG( pu ) );
#  else
  global_status.selection_bound_ufilter = duf_ufilter_clone( DUF_CONFIGG( pu ) );
#  endif
# endif
  DUF_ENDR( r );
}
#endif

/* 20150913.101022
 *  evaluate one sql statement with callback
 * 1. expand sql statement with duf variables ${...}
 * 2. prepare statement
 * 3. make callback for ...?
 * 4. sql ONE step, store changes, no any selects
 * 5. end statement
 * */
SR( SQL, eval_sql_one_cb, const char *sql, const duf_ufilter_t * pu, const duf_yfilter_t * py, duf_bind_cb_t callback, const mas_argvc_t * ttarg,
    const char *selected_db, int *pchanges, const void *ptr )
{
/* DUF_STARTR( r ); */
  int changes = 0;
  const char *worksql = NULL;
  char *tmpsql = NULL;

/* r = duf_sql( *p, &changes ); */
#if 0
  DORF( r, duf_main_db_open, pdi );
#endif
  if ( selected_db )
    worksql = tmpsql = duf_expand_sql( sql, selected_db );
  else
    worksql = sql;
  {
    MAST_TRACE( sql, 3, worksql );
#if 0
    DUF_SQL_START_STMT_NOPDI( worksql, r, pstmt );
#else
    DUF_SQL_SE_START_STMT_LOCAL( duf_pdi_global(  ), worksql, pstmt );
#endif
    if ( callback )
      CRV( ( callback ), pstmt, pu, py, ttarg, ptr );
    if ( QNOERR )
    {
      DUF_SQL_SE_STEPC( pstmt );
      DUF_SQL_SE_CHANGES_NOPDI( changes, pstmt );
    }
#if 0
    DUF_SQL_END_STMT_NOPDI( r, pstmt );
#else
    DUF_SQL_SE_END_STMT_LOCAL( duf_pdi_global(  ), pstmt );
#endif
  }
/* MAST_TRACE( action, 2, "(%d) beginning psql %s; changes:%d", r, worksql, changes ); */
  if ( tmpsql )
    mas_free( tmpsql );
  if ( pchanges )
    *pchanges = changes;
/* DUF_ENDR( r ); */
  ER( SQL, eval_sql_one_cb, const char *sql, const duf_ufilter_t * pu, const duf_yfilter_t * py, duf_bind_cb_t callback, const mas_argvc_t * ttarg,
      const char *selected_db, int *pchanges, const void *ptr );
}

/* 20150913.101952
 *  evaluate one sql statement without callback 
 * */
SR( SQL, eval_sql_one, const char *sql, const duf_ufilter_t * pu, const duf_yfilter_t * py, const char *selected_db, int *pchanges )
{
/* DUF_STARTR( r ); */
  CR( eval_sql_one_cb, sql, pu, py, NULL /* cb */ , NULL /* ttarg */ , selected_db, pchanges, NULL /* ptr */  );
/* DUF_ENDR( r ); */
  ER( SQL, eval_sql_one, const char *sql, const duf_ufilter_t * pu, const duf_yfilter_t * py, const char *selected_db, int *pchanges );
}

/* 20150913.101143
 *  evaluate each sql statement from the sequence, possibly wrapped with BEGIN/END
 * */
SR( SQL, eval_sqlsq_cb, duf_sql_sequence_t * ssql, const char *title MAS_UNUSED, const duf_ufilter_t * pu, const duf_yfilter_t * py,
    duf_bind_cb_t callback, const mas_argvc_t * ttarg, const char *selected_db, const void *ptr )
{
/* DUF_STARTR( r ); */

  if ( ssql /* && !ssql->done */  )
  {
    const char **psql = ssql->sql;

    const char **psql0 = psql;

#ifdef MAS_TRACING
    int changes = 0;
#else
    int MAS_UNUSED changes = 0;
#endif
    assert( ssql );
    assert( ssql->name );
    MAST_TRACE( db, 0, "@@@@@@ssql:%s", ssql->name );
    if ( QNOERR && psql0 && *psql0 && ssql->beginend )
    {
      CR( eval_sql_one_cb, "BEGIN", pu, py, callback, ttarg, NULL, &changes, ptr );
    }

    while ( QNOERR && psql && *psql )
    {
      int nntr = 0;

      nntr++;
      MAST_TRACE( sql, 3, "beginning psql #%d: %s", nntr, *psql );
      MAST_TRACE( select, 0, "beginning psql #%d: %s", nntr, *psql );

      assert( ( ssql->set_selected_db && selected_db ) || ( !ssql->set_selected_db && !selected_db ) );
      CR( eval_sql_one_cb, *psql, pu, py, callback, ttarg, ssql->set_selected_db ? selected_db : NULL, &changes, ptr );

      MAST_TRACE( action, 2, "%" DUF_ACTION_TITLE_FMT ": beginning SQL %lu; [%s] changes:%d; %s", title ? title : "no-title",
                  psql - psql0, *psql, changes, QISERR ? "FAIL" : "OK" );
      psql++;
    }
    if ( QNOERR && psql0 && *psql0 && ssql->beginend )
    {
      CR( eval_sql_one_cb, "END", pu, py, callback, ttarg, NULL, &changes, ptr );
    }
    ssql->done++;

  }
/* DUF_ENDR( r ); */
  ER( SQL, eval_sqlsq_cb, duf_sql_sequence_t * ssql, const char *title MAS_UNUSED, const duf_ufilter_t * pu, const duf_yfilter_t * py,
      duf_bind_cb_t callback, const mas_argvc_t * ttarg, const char *selected_db, const void *ptr );
}

/* 20150913.101143
 *  evaluate each sql statement from the sequence, possibly wrapped with BEGIN/END, without callback
 * */
SR( SQL, eval_sqlsq, duf_sql_sequence_t * ssql, int bind, const char *title, const duf_ufilter_t * pu, const duf_yfilter_t * py,
    const char *selected_db )
{
/* DUF_STARTR( r ); */

#if 0
  DOR( r, duf_eval_sqlsq_cb( ssql, title, pu, bind ? duf_bind_ufilter : NULL, NULL /* ttarg */ , selected_db ) );
#else
  CR( eval_sqlsq_cb, ssql, title, pu, py, bind ? duf_bind_ufilter_uni : NULL, NULL /* ttarg */ , selected_db, NULL /* ptr */  );
#endif

/* DUF_ENDR( r ); */
  ER( SQL, eval_sqlsq, duf_sql_sequence_t * ssql, int bind, const char *title, const duf_ufilter_t * pu, const duf_yfilter_t * py,
      const char *selected_db );
}
