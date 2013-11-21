#ifndef MAS_INIT_MODULES_H
#  define MAS_INIT_MODULES_H

/* 8888888888888888888888888888888888888888 */

/* int mas_set_msg_file( mas_options_t * popts, const char **message, unsigned flag ); */
INIT_HANDLER( mas_msg_file_init );
/* 8888888888888888888888888888888888888888 */

/* int mas_message_init( mas_options_t * popts, const char **message, unsigned flag ); */
INIT_HANDLER( mas_message_init );
/* 8888888888888888888888888888888888888888 */

/* int mas_uuid_init( mas_options_t * popts, const char **message, unsigned flag ); */
INIT_HANDLER( mas_uuid_init );
/* 8888888888888888888888888888888888888888 */

/* int mas_opt_files_init( mas_options_t * popts, const char **message, unsigned flag ); */
INIT_HANDLER( mas_opt_files_init );

/* 8888888888888888888888888888888888888888 */
/* int mas_post_init( mas_options_t * popts, const char **message, unsigned flag ); */
INIT_HANDLER( mas_post_init );

/* 8888888888888888888888888888888888888888 */

/* Moved to mas_control.c constructor */
/* int mas_proc_init( mas_options_t * popts, const char **message, unsigned flag ); */


/* #include <mastar/types/mas_common_cdefs.h> */
#endif
