#ifndef MAS_CONTROL_H
#  define MAS_CONTROL_H

/* #include <mastar/types/mas_control_types.h> */
#include <mastar/types/mas_opts_types.h>

int mas_ctrl_init( mas_options_t * popts );
void mas_ctrl_destroy( void );
void mas_ctrl_add_command( const char *s );

#endif
