#ifndef MAS_DUF_BASE_TYPES_H
#  define MAS_DUF_BASE_TYPES_H

typedef unsigned char boolean;
typedef boolean bool;

typedef struct
{
  unsigned flag:1;
  unsigned min;
  unsigned max;
} duf_limits_t;
typedef struct
{
  unsigned flag:1;
  unsigned long long min;
  unsigned long long max;
} duf_limitsll_t;

#if 0
typedef struct
{
  int argc;
  char **argv;
} duf_argvc_t;
typedef struct
{
  int argc;
  char *const *argv;
} duf_cargvc_t;
#endif

typedef struct
{
  char *file;
  char *dir;
} duf_itemtag_t;




#endif

/*
vi: ft=c
*/
