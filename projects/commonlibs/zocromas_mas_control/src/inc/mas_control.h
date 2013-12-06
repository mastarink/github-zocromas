#ifndef MAS_CONTROL_H
#  define MAS_CONTROL_H

/* #include <mastar/types/mas_control_types.h> */
#  include <mastar/types/mas_opts_types.h>

int mas_ctrl_setup( int targc, char **targv, char **tenv );

/* int mas_ctrl_init( mas_options_t * popts, const char **message, unsigned force_messages ); */
INIT_HANDLER( mas_ctrl_init );
int mas_ctrl_destroy( void );
void mas_ctrl_add_command( const char *s );



#endif
