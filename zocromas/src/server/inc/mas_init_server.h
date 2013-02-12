#ifndef MAS_INIT_SERVER_H
#  define MAS_INIT_SERVER_H

#  include "init/inc/mas_init.h"

int mas_init_server( void ( *fun ) ( void ), int initsig, int argc, char **argv, char **env );
void mas_destroy_server( void );

#endif
