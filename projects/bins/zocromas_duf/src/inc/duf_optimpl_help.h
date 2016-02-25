#ifndef MAS_DUF_OPTIMPL_HELP_H
#  define MAS_DUF_OPTIMPL_HELP_H


# include <mastar/error/mas_error_types.h>                           /* mas_error_code_t ♣ */


/* duf_option_class_t duf_help_option2class( duf_option_code_t code ); */

mas_error_code_t duf_option_O_help(  /* int argc, char *const *argv */ void );
mas_error_code_t duf_option_O_examples(  /* int argc, char *const *argv */ void );
mas_error_code_t duf_option_O_showflags(  /* int argc, char *const *argv */ void );
mas_error_code_t duf_option_O_list_xtended( const char *s );

mas_error_code_t duf_option_O_list_options( long n );


#endif
