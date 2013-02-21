#ifndef MAS_LCONTROL_OBJECT_H
#  define MAS_LCONTROL_OBJECT_H

#  include <mastar/types/mas_listener_control_types.h>



mas_lcontrol_t *mas_lcontrol_create( void );
/* naming : create + init + register = make */
mas_lcontrol_t *mas_lcontrol_make( const char *host, unsigned port );
int mas_lcontrol_init( mas_lcontrol_t * plcontrol, const char *host, unsigned port );
int mas_lcontrol_register( mas_lcontrol_t * plcontrol );
int mas_lcontrol_delete( mas_lcontrol_t * plcontrol );


#endif

