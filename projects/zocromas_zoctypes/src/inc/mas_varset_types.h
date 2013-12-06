#ifndef MAS_VARSET_TYPES_H
#  define MAS_VARSET_TYPES_H

#  include <sys/uio.h>
#  include <stdarg.h>

#include <mastar/types/mas_common_types.h>
#  include <mastar/types/mas_varvec_types.h>

typedef struct mas_varset_s
{
  void *classes;
  unsigned long nclasses;
} mas_varset_t;



#endif
