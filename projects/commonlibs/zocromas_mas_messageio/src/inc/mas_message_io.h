#ifndef MAS_MESSAGE_IO_H
#  define MAS_MESSAGE_IO_H

#  include <mastar/types/mas_message_types.h>

int mas_channel_write_message( mas_channel_t * pchannel, const char *cbuf, mas_header_t * header );
int mas_channel_read_message( mas_channel_t * pchannel, char **pcbuf, mas_header_t * pheader );

#endif
