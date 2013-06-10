#ifndef MAS_LISTENER_H
#  define MAS_LISTENER_H

int mas_listener( MAS_PASS_OPTS_DECLARE mas_lcontrol_t * plcontrol );
void *mas_listener_th( void *arg );

int mas_listener_start( MAS_PASS_OPTS_DECLARE char *host, unsigned port );
int mas_listener_cancel( mas_lcontrol_t * plcontrol );
int mas_listener_wait( mas_lcontrol_t * plcontrol );

#  undef MAS_WEAK
#endif
