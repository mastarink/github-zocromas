#ifndef MAS_DUF_OPTION_CONFIG_H
# define MAS_DUF_OPTION_CONFIG_H

# include <mastar/tools/mas_arg_tools.h>                             /* mas_arg_get_cb_arg_t */
# include "duf_option_types.h"                                       /* duf_option_t; duf_longval_extended_vtable_t */
# include "duf_config_cli_types.h"                                   /* duf_config_cli_t */

void duf_cli_options_init( duf_config_cli_t * cli, int argc, char **argv, const duf_longval_extended_table_t * const *xtable_list,
                           const char *config_dir, const char *commands_dir, mas_arg_get_cb_arg_t varfunc );

duf_longval_extended_vtable_t **duf_cli_options_xtable_list2xvtable( const duf_longval_extended_table_t * const *xtable_multi );

void duf_cli_options_shut( duf_config_cli_t * cli );
duf_config_cli_t *duf_cli_options_config( void );

char *duf_cli_options_shorts_create( const duf_longval_extended_vtable_t * const *xvtables );
const char *duf_cli_options_get_shorts( void );
const char *duf_cli_options_config_dir( void );
const char *duf_cli_options_commands_dir( void );

mas_argvc_t *duf_cli_options_get_targ( void );

const char *duf_cli_options_get_history_filename( void );

int duf_cli_options_get_targc( void );
int *duf_cli_options_get_ptargc( void );
char **duf_cli_options_get_targv( void );
char ***duf_cli_options_get_ptargv( void );
char *duf_cli_options_get_targi( int ia );
char **duf_cli_options_get_ptargi( int ia );

mas_cargvc_t *duf_cli_options_get_carg( void );
int duf_cli_options_get_cargc( void );
char *const *duf_cli_options_get_cargv( void );
const char *duf_cli_options_get_cargvn( int n );

int duf_cli_options_get_targ_offset( void );
void duf_cli_options_set_targ_offset( int targ_offset );

void duf_cli_options_reset_targ_offset( void );

duf_option_t *duf_cli_options_get_longopts_table( void );

duf_longval_extended_vtable_t **duf_cli_options_xvtable_multi( void );

char duf_cli_options_delimiter( void );
duf_option_adata_t *duf_cli_options_aod( void );

mas_arg_get_cb_arg_t duf_cli_options_varfunc( void );

const char *duf_cli_options_bin_name( void );
const char *duf_cli_options_config_file_name( void );

#endif
