#ifndef MAS_OPTS_RESTORE_H
#  define MAS_OPTS_RESTORE_H

int mas_opts_restore_zero( MAS_PASS_OPTS_DECLARE const char *filename );
int mas_opts_restore_user( MAS_PASS_OPTS_DECLARE const char *dirname, const char *filename );
int mas_opts_restore_user_plus( MAS_PASS_OPTS_DECLARE const char *dirname, const char *filename, ... );

int mas_opts_restore_flags( MAS_PASS_OPTS_DECLARE const char *s );

int mas_opts_restore_new_section( MAS_PASS_OPTS_DECLARE const char *section );
int mas_opts_restore_at_section( MAS_PASS_OPTS_DECLARE const char *section, const char *s );

int mas_opts_restore_relative( MAS_PASS_OPTS_DECLARE const char *filename );
int mas_opts_restore_path( MAS_PASS_OPTS_DECLARE const char *fpath );

#endif
