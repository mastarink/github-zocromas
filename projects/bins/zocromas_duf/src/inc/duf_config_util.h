#ifndef MAS_DUF_CONFIG_UTIL_H
# define MAS_DUF_CONFIG_UTIL_H

# include "duf_config_cli_types.h"
# include "duf_config_opt_types.h"

/* void *duf_get_config_offset( unsigned long off ); */
void *duf_get_config_puz_offset( unsigned long off );

duf_config_cli_t *duf_get_config_cli( void );
void *duf_get_pointer_config_cli( void );
void *duf_get_pointer_config_trace(void);

duf_config_opt_t *duf_get_config_opt( void );

/* char duf_option_delimiter( void ); */

char *duf_config_db_path_add_subdir( const char *path, int *pr );
void *duf_get_pointer_ufilter( void );

const char *duf_string_options_at_string_xsdb_getvar( const char *name, const char *arg );

#endif
