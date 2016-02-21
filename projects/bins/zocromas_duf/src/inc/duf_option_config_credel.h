#ifndef MAS_DUF_OPTION_CONFIG_CREDEL_H
# define MAS_DUF_OPTION_CONFIG_CREDEL_H

# include <mastar/tools/mas_arg_tools.h>                             /* mas_arg_get_cb_arg_t */
# include "duf_option_types.h"                                       /* duf_longval_extended_vtable_t */
# include "duf_config_cli_types.h"                                   /* duf_config_cli_t */

void duf_cli_options_init( duf_config_cli_t * cli, int argc, char **argv, const duf_longval_extended_table_t * const *xtable_list,
                           const char *config_dir, const char *commands_dir, mas_arg_get_cb_arg_t varfunc );


void duf_cli_options_shut( duf_config_cli_t * cli );

char *duf_cli_options_shorts_create( const duf_longval_extended_vtable_t * const *xvtables );

#endif

