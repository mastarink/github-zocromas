#ifndef MAS_CLIENT_SESSION_H
#  define MAS_CLIENT_SESSION_H

#include <mastar/types/mas_channel_types.h>
int mas_client_exchange( mas_channel_t * pchannel, const char *question, const char *answer_format );

#endif
