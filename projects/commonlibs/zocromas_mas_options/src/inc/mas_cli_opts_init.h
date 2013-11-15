#ifndef MAS_CLI_OPTS_INIT_H
#  define MAS_CLI_OPTS_INIT_H

char *mas_cli_enabled_options( mas_options_t * popts );

/* int _mas_cli_options_init( mas_options_t * popts, const char **message, unsigned flags ); */
INIT_HANDLER( _mas_cli_options_init );
/* int mas_cli_options_init( mas_options_t * _popts, const char **message, unsigned flags ); */
INIT_HANDLER( mas_cli_options_init );
int mas_cli_options_argv_init( mas_options_t * popts, int argc, char **argv, char **env );

/* INIT_HANDLER( mas_cli_options_argv_init ); */

void mas_cli_options_destroy( mas_options_t * popts );
void mas_cli_options_argv_destroy( mas_options_t * popts );

#endif
