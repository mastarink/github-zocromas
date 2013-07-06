#ifndef MAS_VARVEC_TYPES_H
#  define MAS_VARVEC_TYPES_H

#  include <sys/uio.h>
#  include <stdarg.h>

#  include <mastar/types/mas_common_types.h>


typedef struct iovec mas_varvec_element_t;
typedef struct mas_varvec_s
{
  char *name;
  unsigned long id;
  unsigned has_head:1;
  unsigned has_tail:1;
  unsigned el_cnt;
  mas_varvec_element_t *vec;
  mas_varvec_element_t *use_var;
} mas_varvec_t;




#endif
