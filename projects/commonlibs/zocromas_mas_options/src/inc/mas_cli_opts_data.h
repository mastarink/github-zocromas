#ifndef MAS_CLI_OPTS_DATA_H
#  define MAS_CLI_OPTS_DATA_H

/* int mas_cli_options_data_init( mas_options_t * popts, const char **message, unsigned flags ); */
INIT_HANDLER( mas_cli_options_data_init );
void mas_cli_options_data_destroy( mas_options_t * popts );

/* mas_optionx_t *mas_cli_find_optx( int opt ); */

void mas_cli_print_optx_table( mas_options_t * popts );

#endif
