#ifndef MAS_INIT_PID_H
#  define MAS_INIT_PID_H

/* int mas_pids_init( mas_options_t * popts, const char * *message ); */
INIT_HANDLER( mas_pids_init );
void mas_pids_destroy( mas_options_t * popts );

#endif
