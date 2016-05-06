#ifndef MAS_MUC_OPTION_CONFIG_H
# define MAS_MUC_OPTION_CONFIG_H

# include <mastar/tools/mas_arg_tools.h>                             /* mas_arg_get_cb_arg_t */
# include "muc_option_types.h"                                       /* muc_option_t; muc_longval_extended_vtable_t */
# include "muc_config_cli_types.h"                                   /* muc_config_cli_t */

void muc_cli_options_xtable_add_one( muc_config_cli_t * cli, const muc_longval_extended_table_t * tab );
void muc_cli_options_xtable_list_add( muc_config_cli_t * cli, const muc_longval_extended_table_t * const *xtable_multi, int numtabs );
muc_longval_extended_vtable_t **muc_cli_options_xtable_list2xvtable( muc_config_cli_t * cli, const muc_longval_extended_table_t * const *xtable_multi,
                                                                     unsigned numtabs, muc_longval_extended_vtable_t ** vtable_multi );

muc_option_gen_code_t muc_cli_options_get_maxcodeval( const muc_config_cli_t * cli );
const char *muc_cli_options_get_shorts( const muc_config_cli_t * cli );
const char *muc_cli_options_config_dir( const muc_config_cli_t * cli );
const char *muc_cli_options_commands_dir( const muc_config_cli_t * cli );

mas_argvc_t *muc_cli_options_get_targ( muc_config_cli_t * cli );

const char *muc_cli_options_get_history_filename( const muc_config_cli_t * cli );

int muc_cli_options_get_targc( muc_config_cli_t * cli );
int *muc_cli_options_get_ptargc( muc_config_cli_t * cli );
char **muc_cli_options_get_targv( muc_config_cli_t * cli );
char ***muc_cli_options_get_ptargv( muc_config_cli_t * cli );
char *muc_cli_options_get_targi( muc_config_cli_t * cli, int ia );
char **muc_cli_options_get_ptargi( muc_config_cli_t * cli, int ia );

mas_cargvc_t *muc_cli_options_get_carg( muc_config_cli_t * cli );
int muc_cli_options_get_cargc( const muc_config_cli_t * cli );
char *const *muc_cli_options_get_cargv( const muc_config_cli_t * cli );
const char *muc_cli_options_get_cargvn( const muc_config_cli_t * cli, int n );

int muc_cli_options_get_targ_offset( const muc_config_cli_t * cli );
void muc_cli_options_set_targ_offset( muc_config_cli_t * cli, int targ_offset );

void muc_cli_options_reset_targ_offset( muc_config_cli_t * cli );

muc_option_t *muc_cli_options_get_longopts_table( const muc_config_cli_t * cli );

muc_longval_extended_vtable_t **muc_cli_options_xvtable_multi( const muc_config_cli_t * cli );

char muc_cli_options_delimiter( const muc_config_cli_t * cli, muc_option_source_t source );
muc_option_adata_t *muc_cli_options_aod( muc_config_cli_t * cli );

mas_arg_get_cb_arg_t muc_cli_options_varfunc( const muc_config_cli_t * cli );

const char *muc_cli_options_bin_name( const muc_config_cli_t * cli );
const char *muc_cli_options_config_file_name( const muc_config_cli_t * cli );
const char *muc_cli_options_config_env_var_name( const muc_config_cli_t * cli );

const mas_config_trace_t *muc_get_cli_options_trace_config( const muc_config_cli_t * cli );

#endif
