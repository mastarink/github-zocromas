#ifndef DUF_SCCB_EVAL_H
#  define DUF_SCCB_EVAL_H

int duf_evaluate_sccb( duf_scan_callbacks_t * sccb );
int duf_evaluate_sccb_named_list( const char *names, const duf_action_table_t * table );
int duf_evaluate_sccb_namen( const char *name, size_t len, const duf_action_table_t * table );
int duf_evaluate_sccb_name( const char *name, const duf_action_table_t * table );

int duf_evaluate_sccb_array( duf_scan_callbacks_t ** sccb_sequence, int sccb_num, int *pcnt );

#endif
