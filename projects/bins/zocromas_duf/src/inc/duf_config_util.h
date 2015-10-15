#ifndef MAS_DUF_CONFIG_UTIL_H
#  define MAS_DUF_CONFIG_UTIL_H

#  include "duf_config_types.h"

duf_config_t *duf_get_config( void );
void *duf_get_config_offset( unsigned long off );
void *duf_get_config_puz_offset( unsigned long off );
duf_config_cli_t *duf_get_config_cli( void );
duf_config_opt_t *duf_get_config_opt( void );

int duf_verbose( void );
int duf_output_level( void );
FILE *duf_output_file( void );
char duf_option_delimiter( void );

char *duf_string_options_expand( const char *s, char protected_prefix );
char *duf_config_string_expanded( duf_config_string_t * cs );
char *duf_config_db_path_add_subdir( const char *path, int *pr );


#endif
