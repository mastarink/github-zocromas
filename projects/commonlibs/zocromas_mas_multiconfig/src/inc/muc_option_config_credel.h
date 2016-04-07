#ifndef MAS_MUC_OPTION_CONFIG_CREDEL_H
# define MAS_MUC_OPTION_CONFIG_CREDEL_H

# include <mastar/tools/mas_arg_tools.h>                             /* mas_arg_get_cb_arg_t */
# include <mastar/trace/mas_trace_types.h>                           /* mas_config_trace_t */

# include "muc_option_types.h"                                       /* muc_longval_extended_vtable_t; muc_longval_extended_table_t */
# include "muc_config_cli_types.h"                                   /* muc_config_cli_t */

muc_config_cli_t *muc_cli_options_allocate( void );
muc_config_cli_t *muc_cli_options_create( int argc, char **argv, const muc_longval_extended_table_t * const *xtable_list, unsigned mandatory_config,
                                          const char *config_dir, const char *commands_dir, mas_arg_get_cb_arg_t varfunc,
                                          const mas_config_trace_t * ptracecfg );

void muc_cli_options_delete( muc_config_cli_t * cli );

void muc_cli_options_init( muc_config_cli_t * cli, int argc, char **argv, const muc_longval_extended_table_t * const *xtable_list,
                           unsigned mandatory_config, const char *config_dir, const char *commands_dir, mas_arg_get_cb_arg_t varfunc,
                           const mas_config_trace_t * ptracecfg );
void muc_cli_options_postinit( muc_config_cli_t * cli );
void muc_cli_options_postinit_reset( muc_config_cli_t * cli );

void muc_cli_options_shut( muc_config_cli_t * cli );

muc_option_gen_code_t muc_cli_option_count_maxcodeval( const muc_config_cli_t * cli, muc_longval_extended_vtable_t * *xvtables );

#endif
