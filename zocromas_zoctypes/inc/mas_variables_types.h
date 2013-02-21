#ifndef MAS_VARIABLES_TYPES_H
#  define MAS_VARIABLES_TYPES_H

#  include <stdarg.h>
#  include <string.h>

#  include <mastar/types/mas_list_def.h>

#  include <mastar/types/mas_th_types.h>

typedef size_t( *mas_xvsnprintf_t ) ( char *str, size_t size, const char *format, va_list args );

// *INDENT-OFF*
typedef MAS_LIST_HEAD( mas_variables_list_s, mas_variable_s ) mas_variables_list_head_t;
typedef MAS_LIST_ENTRY( mas_variable_s ) mas_variables_list_entry_t;
// *INDENT-ON*

typedef enum mas_vartype_e
{
  MAS_VARTYPE_NONE,
  MAS_VARTYPE_TEXT,
  MAS_VARTYPE_BINARY,
  MAS_VARTYPE_FILEPATH,
} mas_vartype_t;

struct mas_variable_s
{
  th_type_t thtype;
  mas_vartype_t vtype;
  char *vclass;
  char *name;
  char *value;
  size_t valsize;
  mas_variables_list_entry_t next;
};
typedef struct mas_variable_s mas_variable_t;

#endif
