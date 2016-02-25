#ifndef MAS_DUF_CONFIG_OUTPUT_UTIL_H
# define MAS_DUF_CONFIG_OUTPUT_UTIL_H

extern int duf_verbose( void ) __attribute__ ( ( weak ) );
extern int duf_dry_run( void ) __attribute__ ( ( weak ) );

extern int duf_output_level( void ) __attribute__ ( ( weak ) );
extern FILE *duf_output_file( void ) __attribute__ ( ( weak ) );

int duf_output_force_color( void );
int duf_output_nocolor( void );
int duf_output_progress( void );

/* int duf_output_use_binformat( void ); */

#endif
