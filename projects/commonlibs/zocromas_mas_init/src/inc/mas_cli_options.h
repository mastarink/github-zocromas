#ifndef MAS_CLI_OPTIONS_H
#  define MAS_CLI_OPTIONS_H

#  include <getopt.h>

int mas_getopt_long( int argc, char *const argv[], const char *optstring, const struct option *longopts, int *longindex );
int mas_getopt_long_only( int argc, char *const argv[], const char *optstring, const struct option *longopts, int *longindex );
int mas_cli_make_option( mas_options_t * _popts, int opt, const char *m_optarg );
int mas_cli_options( mas_options_t * _popts, int argc, char *const argv[] );

#  undef MAS_WEAK
#endif
