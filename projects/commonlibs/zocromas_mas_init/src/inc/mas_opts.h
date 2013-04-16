#ifndef MAS_OPTS_H
#  define MAS_OPTS_H

int mas_opts_destroy( void );
int mas_pre_init_default_opts( void );

int mas_opts_set_configdir( const char *dirname );
int mas_opts_set_configfilename( const char *filename );
int mas_opts_check_dir( void );

#endif
