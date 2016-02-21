#ifndef MAS_DUF_OPTION_CONFIG_H
# define MAS_DUF_OPTION_CONFIG_H

# include <mastar/tools/mas_arg_tools.h>                             /* mas_arg_get_cb_arg_t */
# include "duf_option_types.h"                                       /* duf_option_t; duf_longval_extended_vtable_t */
# include "duf_config_cli_types.h"                                   /* duf_config_cli_t */

duf_longval_extended_vtable_t **duf_cli_options_xtable_list2xvtable( const duf_longval_extended_table_t * const *xtable_multi );

/* duf_config_cli_t *duf_cli_options_config( const duf_config_cli_t * cli ); */

const char *duf_cli_options_get_shorts( const duf_config_cli_t * cli );
const char *duf_cli_options_config_dir( const duf_config_cli_t * cli );
const char *duf_cli_options_commands_dir( const duf_config_cli_t * cli );

mas_argvc_t *duf_cli_options_get_targ( duf_config_cli_t * cli );

const char *duf_cli_options_get_history_filename( const duf_config_cli_t * cli );

int duf_cli_options_get_targc( duf_config_cli_t * cli );
int *duf_cli_options_get_ptargc( duf_config_cli_t * cli );
char **duf_cli_options_get_targv( duf_config_cli_t * cli );
char ***duf_cli_options_get_ptargv( duf_config_cli_t * cli );
char *duf_cli_options_get_targi( duf_config_cli_t * cli, int ia );
char **duf_cli_options_get_ptargi( duf_config_cli_t * cli, int ia );

mas_cargvc_t *duf_cli_options_get_carg( duf_config_cli_t * cli );
int duf_cli_options_get_cargc( const duf_config_cli_t * cli );
char *const *duf_cli_options_get_cargv( const duf_config_cli_t * cli );
const char *duf_cli_options_get_cargvn( const duf_config_cli_t * cli, int n );

int duf_cli_options_get_targ_offset( const duf_config_cli_t * cli );
void duf_cli_options_set_targ_offset( duf_config_cli_t * cli, int targ_offset );

void duf_cli_options_reset_targ_offset( duf_config_cli_t * cli );

duf_option_t *duf_cli_options_get_longopts_table( const duf_config_cli_t * cli );

duf_longval_extended_vtable_t **duf_cli_options_xvtable_multi( const duf_config_cli_t * cli );

char duf_cli_options_delimiter( const duf_config_cli_t * cli );
duf_option_adata_t *duf_cli_options_aod( duf_config_cli_t * cli );

mas_arg_get_cb_arg_t duf_cli_options_varfunc( const duf_config_cli_t * cli );

const char *duf_cli_options_bin_name( const duf_config_cli_t * cli );
const char *duf_cli_options_config_file_name( const duf_config_cli_t * cli );
const char *duf_cli_options_config_env_var_name( const duf_config_cli_t * cli );

#endif
