#ifndef MAS_DUF_SCCB_BEGFIN_H
#  define MAS_DUF_SCCB_BEGFIN_H

int duf_sccb_eval_sql_sequence( const duf_scan_callbacks_t * sccb, const duf_ufilter_t *pu, const char *selected_db ) __attribute__ ( ( warn_unused_result ) );
int duf_sccb_eval_final_sql_sequence( const duf_scan_callbacks_t * sccb, const duf_ufilter_t *pu ) __attribute__ ( ( warn_unused_result ) );

#endif

/*
vi: ft=c
*/
