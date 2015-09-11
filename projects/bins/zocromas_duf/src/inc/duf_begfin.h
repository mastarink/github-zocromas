#ifndef   MAS_DUF_BEGFIN_H
#  define MAS_DUF_BEGFIN_H


int duf_eval_sql_one_cb( const char *sql, duf_bind_cb_t callback, const duf_argvc_t * ttarg, const char *selected_db, int *pchanges )
      __attribute__ ( ( warn_unused_result ) );
int duf_eval_sql_sequence( duf_sql_sequence_t * ssql, int bind, const char *title, const char *selected_db ) __attribute__ ( ( warn_unused_result ) );
int duf_eval_sql_sequence_cb( duf_sql_sequence_t * ssql, const char *title, duf_bind_cb_t callback, const duf_argvc_t * ttarg,
                              const char *selected_db ) __attribute__ ( ( warn_unused_result ) );

#endif

/*
vi: ft=c
*/
