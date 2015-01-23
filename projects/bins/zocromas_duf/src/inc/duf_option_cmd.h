#ifndef MAS_DUF_OPTION_CMD_H
#  define MAS_DUF_OPTION_CMD_H

#  include "duf_option_types.h"

int duf_execute_cmd_long_xtables( const char *string, const duf_longval_extended_t * xtables[], char vseparator, int stage );
int duf_execute_cmd_long_xtables_std( const char *string, char vseparator, int stage );



#endif
