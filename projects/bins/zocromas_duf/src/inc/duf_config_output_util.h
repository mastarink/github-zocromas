#ifndef MAS_DUF_CONFIG_OUTPUT_UTIL_H
# define MAS_DUF_CONFIG_OUTPUT_UTIL_H

int duf_verbose( void );
int duf_dry_run( void );

int duf_output_level( void );
FILE *duf_output_file( void );

int duf_output_force_color( void );
int duf_output_nocolor( void );
int duf_output_progress( void );

/* int duf_output_use_binformat( void ); */

#endif
