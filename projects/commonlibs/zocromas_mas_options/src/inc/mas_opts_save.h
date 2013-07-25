#ifndef MAS_OPTS_SAVE_H
#  define MAS_OPTS_SAVE_H

int mas_opts_save_user( mas_options_t * popts,  const char *dirname, const char *filename );
int mas_opts_save_user_plus( mas_options_t * popts,  const char *dirname, const char *filename, ... );

#endif
