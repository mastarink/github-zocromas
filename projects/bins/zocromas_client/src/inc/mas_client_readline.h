#ifndef MAS_CLIENT_READLINE_H
#  define MAS_CLIENT_READLINE_H

int mas_client_init_readline( MAS_PASS_OPTS_DECLARE1 );
void mas_client_destroy_readline( MAS_PASS_OPTS_DECLARE1 );
int mas_exchange_with_readline( mas_channel_t * pchannel );

#endif
