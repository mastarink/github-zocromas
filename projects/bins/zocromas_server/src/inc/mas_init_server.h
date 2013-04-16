#ifndef MAS_INIT_SERVER_H
#  define MAS_INIT_SERVER_H

int mas_init_daemon( void );
int mas_init_pids( void );
int mas_init_load_protos( void );

void mas_destroy_server( void );

#endif
