#ifndef MAS_OPTS_RESTORE_H
#  define MAS_OPTS_RESTORE_H

int mas_opts_restore_zero( mas_options_t * _popts, const char *filename );
int mas_opts_restore_user( mas_options_t * _popts, const char *dirname, const char *filename );
int mas_opts_restore_user_plus( mas_options_t * _popts, const char *dirname, const char *filename, ... );

int mas_opts_restore_flags( mas_options_t * _popts, const char *s );

int mas_opts_restore_new_section( mas_options_t * _popts, const char *section );
int mas_opts_restore_at_section( mas_options_t * _popts, const char *section, const char *s );

int mas_opts_restore_relative( mas_options_t * _popts, const char *filename );
int mas_opts_restore_path( mas_options_t * _popts, const char *fpath );

/* #include <mastar/types/mas_common_cdefs.h> */
#endif
