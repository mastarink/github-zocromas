#ifndef MAS_INIT_SERVER_H
#  define MAS_INIT_SERVER_H

#  ifdef MAS_INIT_SEPARATE
int mas_init_server( void ( *fun ) ( void ), int initsig, int argc, char **argv, char **env );
#  endif

int mas_init_daemon( void );
int mas_init_pids( void );
int mas_init_load_protos( void );

void mas_destroy_server( void );

#endif
