#ifndef MAS_DUF_EXPAND_TYPES_H
#  define MAS_DUF_EXPAND_TYPES_H

typedef struct
{
  int expandable:1;
  char *expanded;
  char *value;
} duf_expandable_string_t;


#endif

/*
vi: ft=c
*/
