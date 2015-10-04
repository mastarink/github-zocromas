#ifndef MAS_DUF_CONFIG_UTIL_H
#  define MAS_DUF_CONFIG_UTIL_H


char *duf_string_options_expand( const char *s, char protected_prefix );
char *duf_config_string_expanded( duf_config_string_t * cs );
char *duf_config_db_path_add_subdir( const char *path, int *pr );


#endif
