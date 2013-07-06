#ifndef MAS_AUTOSET_TYPES_H
#  define MAS_AUTOSET_TYPES_H

#  include <sys/uio.h>
#  include <stdarg.h>

#  include <mastar/types/mas_common_types.h>

typedef struct mas_autoset_s
{
  void *instances;
  unsigned long ninstances;
} mas_autoset_t;



#endif
