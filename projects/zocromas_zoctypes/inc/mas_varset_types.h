#ifndef MAS_VARSET_TYPES_H
#  define MAS_VARSET_TYPES_H

#  include <sys/uio.h>
#  include <stdarg.h>

typedef struct iovec mas_var_t;
typedef struct mas_varset_class_s
{
  char *name;
  unsigned long id;
  mas_var_t *vec;
  int veccnt;
} mas_varset_class_t;

typedef struct mas_varset_s
{
  void *classes;
  unsigned long nclasses;
} mas_varset_t;

typedef size_t( *mas_xvsnprintf_t ) ( char *str, size_t size, const char *format, va_list args );



#endif
