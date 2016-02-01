#ifndef MAS_DUF_EXPANDABLE_H
#  define MAS_DUF_EXPANDABLE_H

#  include "duf_expand_types.h"

void duf_cfg_string_delete( duf_expandable_string_t * cs );

char *duf_string_options_expand( const char *s, char protected_prefix );
char *duf_config_string_expanded( duf_expandable_string_t * cs );

#endif
