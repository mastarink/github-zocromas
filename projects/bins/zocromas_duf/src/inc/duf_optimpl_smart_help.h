#ifndef MAS_DUF_OPTIMPL_SMART_HELP_H
# define MAS_DUF_OPTIMPL_SMART_HELP_H

# include <mastar/error/mas_error_types.h>                           /* mas_error_code_t ♣ */

/* muc_option_class_t duf_help_option2class( duf_option_code_t code ); */

mas_error_code_t duf_optimpl_O_smart_help_all( muc_option_class_t oclass );
mas_error_code_t duf_optimpl_O_smart_help( muc_option_class_t oclass );
mas_error_code_t duf_optimpl_O_help_set( const char *arg );

#endif
