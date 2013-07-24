#ifndef MAS_INIT_MODULES_H
#  define MAS_INIT_MODULES_H

int mas_init_set_msg_file( mas_options_t * popts, const char **message );
int mas_init_message( mas_options_t * popts, const char **message );
int mas_init_uuid( mas_options_t * popts, const char **message );
int mas_init_opt_files( mas_options_t * popts, const char **message );
int mas_init_proc( mas_options_t * popts, const char **message );
int mas_post_init( mas_options_t * popts, const char **message );


#endif
