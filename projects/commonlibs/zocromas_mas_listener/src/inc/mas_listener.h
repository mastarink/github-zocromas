#ifndef MAS_LISTENER_H
#  define MAS_LISTENER_H

int mas_listener( mas_lcontrol_t * plcontrol );
void *mas_listener_th( void *arg );

int mas_listener_start( mas_options_t * popts, char *host, unsigned port );
int mas_listener_cancel( mas_lcontrol_t * plcontrol );
int mas_listener_wait( mas_lcontrol_t * plcontrol );

#  undef MAS_WEAK
#endif
