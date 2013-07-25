#ifndef MAS_OPTS_STORAGE_H
#  define MAS_OPTS_STORAGE_H

int mas_opts_set_configdir( mas_options_t * popts,  const char *dirname );
int mas_opts_set_configfilename( mas_options_t * popts,  const char *filename );
int mas_opts_check_dir( mas_options_t * popts );

#endif
