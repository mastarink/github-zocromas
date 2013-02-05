#ifndef MAS_CHANNEL_OBJECT_H
#  define MAS_CHANNEL_OBJECT_H

#  include "mas_channel_types.h"

mas_channel_t *mas_channel_create( void );
int mas_channel_delete( mas_channel_t * pchannel, int toclose, int d );

int mas_channel_init( mas_channel_t * pchannel, int is_server, chn_type_t type, const char *host, size_t hostlen, int port );
int mas_channel_test( mas_channel_t * pchannel );

#endif
