#ifndef MAS_DUF_PREPARE_ACTIONS_H
#  define MAS_DUF_PREPARE_ACTIONS_H

int duf_config2sccb_vector_sample( duf_scan_callbacks_t ** ppscan_callbacks, int max_asteps ) __attribute__ ( ( warn_unused_result ) );
int duf_config2sccb_vector( duf_scan_callbacks_t ** ppscan_callbacks, int max_asteps ) __attribute__ ( ( warn_unused_result ) );

#endif
