#ifndef MAS_DUF_EXPAND_TYPES_H
# define MAS_DUF_EXPAND_TYPES_H

# include <mastar/tools/mas_arg_tools.h>                             /* duf_expandable_string_t */

typedef struct
{
  int expandable:1;
  int is_expanded:1;
  char *expanded;
  char *value;
  char protected_prefix;
/* static const char * duf_string_options_at_string_xsdb_getvar( const char *name, const char *arg DUF_UNUSED ) */
  mas_arg_get_cb_arg_t varfunc;
} duf_expandable_string_t;

#endif

/*
vi: ft=c
*/
