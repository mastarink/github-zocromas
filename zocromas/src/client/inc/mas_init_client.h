#ifndef MAS_INIT_CLIENT_H
#  define MAS_INIT_CLIENT_H

#  include "init/inc/mas_init.h"

int mas_init_client( void ( *atexit_fun ) ( void ), int initsig, int argc, char **argv, char **env );
void mas_destroy_client( void );

#endif
