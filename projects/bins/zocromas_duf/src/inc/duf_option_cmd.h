#ifndef MAS_DUF_OPTION_CMD_H
#  define MAS_DUF_OPTION_CMD_H

#  include "duf_option_types.h"
#  include "duf_options_def.h"

/* int duf_find_cmd_long( const char *string, const duf_longval_extended_t * xtable, unsigned xtable_size, char vseparator, char **parg ); */
int duf_find_cmd_long_no( const char *string, const duf_longval_extended_t * xtable, unsigned xtable_size, char vseparator, char **parg, int *pno );
int duf_execute_cmd_long( const char *string, const duf_longval_extended_t * xtable, unsigned xtable_size, char vseparator,int stage );
int duf_execute_cmd_long_std( const char *string, char vseparator,int stage );


#endif
