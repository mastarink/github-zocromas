#ifndef MAS_INIT_SERVER_H
#  define MAS_INIT_SERVER_H

#  include "init/inc/mas_init.h"

#ifdef MAS_INIT_SEPARATE
int mas_init_server( void ( *fun ) ( void ), int initsig, int argc, char **argv, char **env );
#endif

int mas_init_daemon( void );
int mas_init_load_protos( void );

void mas_destroy_server( void );

#endif
