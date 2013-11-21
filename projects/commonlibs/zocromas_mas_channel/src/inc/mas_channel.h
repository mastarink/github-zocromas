#ifndef MAS_CHANNEL_H
#  define MAS_CHANNEL_H

#  include <mastar/types/mas_channel_types.h>

int mas_channel_fd( const mas_channel_t * pchannel );

#ifdef MAS_CHANNEL_STREAM_WRITE
FILE *mas_channel_stream( const mas_channel_t * pchannel );
#endif

mas_channel_t *mas_channel_clone( mas_channel_t * pchannel_src );


int mas_channel_writef( const mas_channel_t * pchannel, const char *fmt, ... );
int mas_channel_vwritef( const mas_channel_t * pchannel, const char *fmt, va_list args );

int mas_channel_write_string( const mas_channel_t * pchannel, const char *cbuf, int z );

int mas_channel_write( const mas_channel_t * pchannel, const char *cbuf, size_t sz );

char *mas_channel_ip_string( mas_channel_t * pchannel );

int mas_channel_read_some( mas_channel_t * pchannel );
int mas_channel_read_some_new( mas_channel_t * pchannel );
int mas_channel_read_remainder( mas_channel_t * pchannel );


/* #include <mastar/types/mas_common_cdefs.h> */
#endif
