#ifndef MAS_OPTS_H
#  define MAS_OPTS_H

int mas_opts_destroy( void );
int mas_opts_save( const char *dirname, const char *filename );
int mas_opts_save_plus( const char *dirname, const char *filename, ... );
int mas_opts_restore( const char *dirname, const char *filename );
int mas_opts_restore_plus( const char *dirname, const char *filename, ... );

void mas_opts_set_argv( int *pargc, char ***pargv, const char *s );
void mas_opts_add_command( const char *s );

#endif
