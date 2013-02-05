#ifndef MAS_CHANNEL_LISTEN_H
#  define MAS_CHANNEL_LISTEN_H

#  include "mas_channel_types.h"

int mas_channel_unlink( void );

int mas_channel_listen( mas_channel_t * pchannel );

/* int mas_channel_listen_tcp( mas_channel_t * pchannel );  */

int mas_channel_deaf( mas_channel_t * pchannel );


#endif
