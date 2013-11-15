#ifndef MAS_LISTENER_CONTROL_LIST_H
#  define MAS_LISTENER_CONTROL_LIST_H

#  include <mastar/types/mas_listener_control_types.h>

/* int mas_lcontrols_init( mas_options_t * popts, const char * *message ); */
INIT_HANDLER( mas_lcontrols_init );

void mas_lcontrols_list_create( void );
int mas_lcontrols_clean_list( int force );
void mas_lcontrols_delete_list( void );

/* find by host + port */
mas_lcontrol_t *mas_lcontrol_find( const char *host, int port );

#endif
