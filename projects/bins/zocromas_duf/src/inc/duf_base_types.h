#ifndef MAS_DUF_BASR_TYPES_H
#define MAS_DUF_BASR_TYPES_H

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

#endif

/*
vi: ft=c
*/
