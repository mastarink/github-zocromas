#ifndef MAS_CLIENT_READLINE_H
#  define MAS_CLIENT_READLINE_H

int mas_client_init_readline( mas_options_t * popts );
void mas_client_destroy_readline( mas_options_t * popts );
int mas_exchange_with_readline( mas_channel_t * pchannel );

#endif
