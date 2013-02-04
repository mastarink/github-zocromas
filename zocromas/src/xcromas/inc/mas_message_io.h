#ifndef MAS_MESSAGE_IO_H
#  define MAS_MESSAGE_IO_H

#  include "mas_message_types.h"

/* int mas_read_message( int fd, char **pbuf, pid_t * psender_pid, unsigned long *psender_vdate, unsigned long *psender_vtime ) ; */
#  ifndef MAS_CHANNEL_STREAM_READ
int mas_read_message( int fd, char **pbuf, mas_header_t * pheader );
#  else
int mas_fread_message( FILE * stream, char **pbuf, mas_header_t * pheader );
#  endif

#  ifndef MAS_CHANNEL_STREAM_WRITE
int mas_write_message( int fd, const char *cbuf, mas_header_t * header );
#  else
int mas_fwrite_message( FILE * stream, const char *cbuf, mas_header_t * header );
#  endif

int mas_channel_write_message( const mas_channel_t * pchannel, const char *cbuf, mas_header_t * header );
int mas_channel_read_message( const mas_channel_t * pchannel, char **pcbuf, mas_header_t * pheader );

#endif
