#ifndef  MAS_TOOLS_EXPANDABLE_H
#define  MAS_TOOLS_EXPANDABLE_H

#include "mas_arg_tools.h"   /* mas_arg_get_cb_arg_t */
#include "mas_expandable_types.h" /* mas_expandable_string_t */

void mas_expandable_string_delete( mas_expandable_string_t * cs );

char *mas_string_options_expand( const char *s, char protected_prefix, mas_arg_get_cb_arg_t varfunc );
const char *mas_string_expanded( mas_expandable_string_t * cs );

#endif
