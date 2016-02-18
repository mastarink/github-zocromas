#ifndef   MAS_EVSQL_BEGFIN_H
#  define MAS_EVSQL_BEGFIN_H

#include "sql_beginning_types.h" /* duf_bind_cb_t */

/* 20150913.101022
 * 1. expand sql statement with duf variables ${...}
 * 2. prepare statement
 * 3. make callback for ...?
 * 4. sql ONE step, store changes, no any selects
 * 5. end statement * */
int duf_eval_sql_one_cb( const char *sql, const duf_ufilter_t * pu, const duf_yfilter_t * py, duf_bind_cb_t callback, const mas_argvc_t * ttarg,
                         const char *selected_db, int *pchanges ) __attribute__ ( ( warn_unused_result ) );

/* 20150913.101952
 *  evaluate one sql statement without callback 
 * */
int duf_eval_sql_one( const char *sql, const duf_ufilter_t * pu, const duf_yfilter_t * py, const char *selected_db, int *pchanges );

/* 20150913.101143
 *  evaluate each sql statement from the sequence, possibly wrapped with BEGIN/END, with callback */
int duf_eval_sqlsq_cb( duf_sql_sequence_t * ssql, const char *title, const duf_ufilter_t * pu, const duf_yfilter_t * py, duf_bind_cb_t callback,
                       const mas_argvc_t * ttarg, const char *selected_db ) __attribute__ ( ( warn_unused_result ) );

/* 20150913.101143
 *  evaluate each sql statement from the sequence, possibly wrapped with BEGIN/END, without callback */
int duf_eval_sqlsq( duf_sql_sequence_t * ssql, int bind, const char *title, const duf_ufilter_t * pu, const duf_yfilter_t * py, const char *selected_db )
      __attribute__ ( ( warn_unused_result ) );

#endif

/*
vi: ft=c
*/
