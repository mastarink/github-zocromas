#ifndef MAS_ERROR_TYPES_H
#  define MAS_ERROR_TYPES_H

#  include <mastar/sqlite/mas_sqlite_const.h>

typedef enum
{
#  define ENUM_WRAP(_n)       _n,
#  define ENUM_WRAP_V(_n, _v) _n = _v,
#  include "mas_error_enum.def"
#  undef ENUM_WRAP
#  undef ENUM_WRAP_V
} mas_error_code_t;

typedef struct
{
  const char *name;
  mas_error_code_t code;
} mas_errdesc_t;

typedef int mas_error_index_t;

typedef struct
{
  mas_error_index_t index;
  mas_error_code_t code;
  const char *funcid;
  int linid;
  int shown;
  char *message;
  int count_reported;
} mas_error_event_t;


#  define DUF_ERROR_COUNT ( DUF_ERROR_ERROR_MAX - DUF_ERROR_ERROR_BASE )

#endif

/*
vi: ft=c
*/
