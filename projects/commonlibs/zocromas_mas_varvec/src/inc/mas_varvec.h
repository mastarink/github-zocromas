#ifndef MAS_VARVEC_H
#  define MAS_VARVEC_H

#  include <mastar/types/mas_varvec_types.h>

unsigned long mas_varvec_id( mas_varvec_t * vclass );

ssize_t mas_varvec_write( int fd, mas_varvec_t * vclass );


/* #  include <mastar/types/mas_common_cdefs.h> */
#endif
