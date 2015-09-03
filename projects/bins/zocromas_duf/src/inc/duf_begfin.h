#ifndef   MAS_DUF_BEGFIN_H
#  define MAS_DUF_BEGFIN_H


int duf_eval_sql_sequence( duf_sql_sequence_t * ssql, int bind, const char *title, const char *selected_db );
int duf_eval_sql_sequence_cb( duf_sql_sequence_t * ssql, const char *title, duf_bind_cb_t callback, const duf_argvc_t *ttarg, const char *selected_db );

#endif

/*
vi: ft=c
*/
