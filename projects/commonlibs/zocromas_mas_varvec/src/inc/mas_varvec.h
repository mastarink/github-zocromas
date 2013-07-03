#ifndef MAS_VARVEC_H
#  define MAS_VARVEC_H

#  include <mastar/types/mas_varvec_types.h>

unsigned long mas_varvec_id( mas_varvec_t * vclass );

void mas_varvec_write( int fd, mas_varvec_t * vclass );

#endif
