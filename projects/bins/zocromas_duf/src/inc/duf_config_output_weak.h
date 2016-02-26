#ifndef MAS_DUF_CONFIG_OUTPUT_WEAK_H
# define MAS_DUF_CONFIG_OUTPUT_WEAK_H

int duf_verbose( void ) __attribute__ ( ( weak ) );
int duf_dry_run( void ) __attribute__ ( ( weak ) );

int duf_output_level( void ) __attribute__ ( ( weak ) );
FILE *duf_output_file( void ) __attribute__ ( ( weak ) );
int duf_output_force_color( void ) __attribute__ ( ( weak ) );
int duf_output_nocolor( void ) __attribute__ ( ( weak ) );

#endif
