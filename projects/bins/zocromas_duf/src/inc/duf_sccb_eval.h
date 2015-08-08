#ifndef DUF_SCCB_EVAL_H
#  define DUF_SCCB_EVAL_H

int duf_evaluate_sccb( duf_scan_callbacks_t * sccb );
int duf_evaluate_sccb_array( duf_scan_callbacks_t ** sccb_sequence, int sccb_num, int *pcnt );

int duf_evaluate_sccb_named_list( const char *names, const duf_action_table_t * table );

int duf_sccbh_eval_pdi_and_summary( duf_sccb_handle_t * sccbh );

#endif
