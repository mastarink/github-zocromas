#ifndef  MAS_TOOLS_EXPANDABLE_TYPES_H
# define  MAS_TOOLS_EXPANDABLE_TYPES_H

# include "mas_arg_tools.h"                                          /* mas_arg_get_cb_arg_t */
typedef struct
{
  int expandable:1;
  int is_expanded:1;
  char *expanded;
  char *value;
  char protected_prefix;
/* static const char * mas_string_options_at_string_xsdb_getvar( const char *name, const char *arg DUF_UNUSED ) */
  mas_arg_get_cb_arg_t varfunc;
} mas_expandable_string_t;

#endif
