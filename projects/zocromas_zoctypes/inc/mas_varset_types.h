#ifndef MAS_VARSET_TYPES_H
#  define MAS_VARSET_TYPES_H

#  include <sys/uio.h>
#  include <stdarg.h>

typedef struct iovec mas_vclass_element_t;
typedef struct mas_varset_class_s
{
  char *name;
  unsigned long id;
  unsigned has_head:1;
  unsigned has_tail:1;
  unsigned el_cnt;
  mas_vclass_element_t *vec;
  mas_vclass_element_t *use_var;
} mas_varset_class_t;

typedef struct mas_varset_s
{
  void *classes;
  unsigned long nclasses;
} mas_varset_t;



#endif
