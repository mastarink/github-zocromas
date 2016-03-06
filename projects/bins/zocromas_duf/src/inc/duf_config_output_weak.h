#ifndef MAS_DUF_CONFIG_OUTPUT_WEAK_H
# define MAS_DUF_CONFIG_OUTPUT_WEAK_H

# include <stdio.h>                                                  /* FILE */

int mas_verbose( void ) __attribute__ ( ( weak ) );
int mas_dry_run( void ) __attribute__ ( ( weak ) );

int mas_output_level( void ) __attribute__ ( ( weak ) );
FILE *mas_output_file( void ) __attribute__ ( ( weak ) );
int mas_output_force_color( void ) __attribute__ ( ( weak ) );
int mas_output_nocolor( void ) __attribute__ ( ( weak ) );

#endif
