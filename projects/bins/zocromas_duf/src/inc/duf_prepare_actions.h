#ifndef MAS_DUF_PREPARE_ACTIONS_H
#  define MAS_DUF_PREPARE_ACTIONS_H

int duf_config2sccb_vector_sample( duf_scan_callbacks_t ** ppscan_callbacks, int max_asteps );
int duf_config2sccb_vector( duf_scan_callbacks_t ** ppscan_callbacks, int max_asteps );
duf_action_table_t *duf_action_table( void );

#endif
