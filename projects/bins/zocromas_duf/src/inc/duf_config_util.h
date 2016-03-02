#ifndef MAS_DUF_CONFIG_UTIL_H
# define MAS_DUF_CONFIG_UTIL_H

# include <mastar/multiconfig/muc_config_cli_types.h>                /* muc_config_cli_t */
# include "duf_config_opt_types.h"                                   /* duf_config_opt_t */
# include "duf_ufilter_types.h"                                      /* duf_ufilter_t; duf_yfilter_t; etc. ♠ */

/* void *duf_get_config_offset( unsigned long off ); */
/* void *duf_get_config_puz_offset( unsigned long off ); */

muc_config_cli_t *duf_get_config_cli( void );
duf_config_opt_t *duf_get_config_opt( void );
duf_ufilter_t *duf_get_config_ufilter( void );

# ifdef MAS_TRACING
mas_config_trace_t *duf_get_trace_config( void );
# endif
/* char duf_option_delimiter( void ); */

char *duf_config_db_path_add_subdir( const char *path, int *pr );

const char *duf_string_options_at_string_xsdb_getvar( const char *name, const char *arg );

#endif
