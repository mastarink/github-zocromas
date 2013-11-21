#ifndef MAS_CHANNEL_SOCKET_H
#  define MAS_CHANNEL_SOCKET_H

#  include <mastar/types/mas_channel_types.h>

int mas_channel_bind( mas_channel_t * pchannel );
int mas_channel_accept( mas_channel_t * pchannel );
int mas_channel_connect( mas_channel_t * pchannel );


/* #include <mastar/types/mas_common_cdefs.h> */
#endif
