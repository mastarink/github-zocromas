#ifndef MAS_DUF_OPTION_CONFIG_H
#  define MAS_DUF_OPTION_CONFIG_H

#  include "duf_option_types.h"

void duf_cli_options_init( duf_config_cli_t * cli, int argc, char **argv, const duf_longval_extended_vtable_t * const *xvtables );
void duf_cli_options_shut( duf_config_cli_t * cli );
duf_config_cli_t *duf_cli_options_config( void );

char *duf_cli_options_shorts_create( const duf_longval_extended_vtable_t * const *xvtables );
const char *duf_cli_options_get_shorts( void );
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
#endif
