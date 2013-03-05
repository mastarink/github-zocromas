#ifndef MAS_CHANNEL_BUFFER_H
#  define MAS_CHANNEL_BUFFER_H

#  include <mastar/types/mas_channel_types.h>

char *mas_channel_buffer( mas_channel_t * pchannel, size_t * psz );
char *mas_channel_buffer_start( mas_channel_t * pchannel );

void mas_channel_set_buffer( mas_channel_t * pchannel, char *buffer, size_t size );
void mas_channel_delete_buffer( mas_channel_t * pchannel );

void mas_channel_set_buffer_copy( mas_channel_t * pchannel, const char *path );

ssize_t mas_channel_buffer_iptr( mas_channel_t * pchannel );
const char *mas_channel_buffer_ptr( mas_channel_t * pchannel );
void mas_channel_set_buffer_ptr( mas_channel_t * pchannel, const char *ptr );

void mas_channel_buffer_strip( mas_channel_t * pchannel, size_t sz );

int mas_channel_buffer_endfile( mas_channel_t * pchannel );
int mas_channel_buffer_enddata( mas_channel_t * pchannel );
int mas_channel_buffer_eof( mas_channel_t * pchannel );

void mas_channel_buffer_set_maxread( mas_channel_t * pchannel, size_t maxr );

const char *mas_channel_buffer_nl( mas_channel_t * pchannel, size_t * psz );
char *mas_channel_buffer_nl_dup( mas_channel_t * pchannel );
const char *mas_channel_buffer_find_eol( mas_channel_t * pchannel );


#endif
