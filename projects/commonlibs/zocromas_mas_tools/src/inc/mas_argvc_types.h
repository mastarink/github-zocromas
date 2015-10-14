#ifndef MAS_ARGVC_TYPES_H
#  define MAS_ARGVC_TYPES_H

typedef struct
{
  int argc;
  char **argv;
} mas_argvc_t;
typedef struct
{
  int argc;
  char *const *argv;
} mas_cargvc_t;

#endif
