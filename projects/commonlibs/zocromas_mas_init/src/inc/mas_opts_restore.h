#ifndef MAS_OPTS_RESTORE_H
#  define MAS_OPTS_RESTORE_H


int mas_opts_restore_zero( const char *filename );
int mas_opts_restore_user( const char *dirname, const char *filename );
int mas_opts_restore_user_plus( const char *dirname, const char *filename, ... );

int mas_opts_restore_flags( const char *s );

int mas_opts_restore_new_section( const char *section );
int mas_opts_restore_at_section( const char *section, const char *s );

int mas_opts_restore_relative( const char *filename );
int mas_opts_restore_path( const char *fpath );

#endif
