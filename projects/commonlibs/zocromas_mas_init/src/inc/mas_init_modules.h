#ifndef MAS_INIT_MODULES_H
#  define MAS_INIT_MODULES_H

int mas_set_msg_file( mas_options_t * popts, const char **message );
int mas_message_init( mas_options_t * popts, const char **message );
int mas_uuid_init( mas_options_t * popts, const char **message );
int mas_opt_files_init( mas_options_t * popts, const char **message );
int mas_post_init( mas_options_t * popts, const char **message );

/* Moved to mas_control.c constructor */
/* int mas_proc_init( mas_options_t * popts, const char **message ); */


#endif
