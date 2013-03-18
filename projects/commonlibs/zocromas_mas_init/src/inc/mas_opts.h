#ifndef MAS_OPTS_H
#  define MAS_OPTS_H

int mas_opts_destroy( void );
int mas_opts_save_user( const char *dirname, const char *filename );
int mas_opts_save_user_plus( const char *dirname, const char *filename, ... );

int mas_opts_restore_zero(  const char *filename );
int mas_opts_restore_user( const char *dirname, const char *filename );
int mas_opts_restore_user_plus( const char *dirname, const char *filename, ... );

int mas_pre_init_default_opts( void );

#endif
