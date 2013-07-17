#ifndef MAS_INIT_SERVER_H
#  define MAS_INIT_SERVER_H

int mas_init_daemon( mas_options_t * popts, const char * *message );
int mas_init_pids( mas_options_t * popts, const char * *message );
int mas_init_load_protos( mas_options_t * popts, const char * *message );

void mas_destroy_server( mas_options_t * popts );

#endif
