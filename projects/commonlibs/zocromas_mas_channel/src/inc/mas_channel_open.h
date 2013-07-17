#ifndef MAS_CHANNEL_OPEN_H
#  define MAS_CHANNEL_OPEN_H

#  include <mastar/types/mas_channel_types.h>

int mas_channel_open( mas_channel_t * pchannel );

/* int mas_channel_open_tcp( mas_channel_t * pchannel, const char *name, int port ); */

int mas_channel_close( mas_channel_t * pchannel );
int mas_channel_close2( mas_channel_t * pchannel );

/* int mas_channel_close_tcp( mas_channel_t * pchannel );  */
int mas_channel_opened( mas_channel_t * pchannel );

int mas_channel_cork( mas_channel_t * pchannel, int val );

#endif
