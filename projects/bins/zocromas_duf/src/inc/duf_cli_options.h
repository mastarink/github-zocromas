#ifndef MAS_DUF_CLI_OPTIONS_H
#  define MAS_DUF_CLI_OPTIONS_H

int duf_cli_options( int argc, char *argv[] );

int duf_show_options( const char *a0 );

char *duf_restore_options( const char *a0 );

int duf_cli_option_by_string( const char *string );

int duf_infile_options( int argc, char *argv[] );

#endif
