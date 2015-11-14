#ifndef MAS_DUF_OPTION_HELP_H
#  define MAS_DUF_OPTION_HELP_H




/* duf_option_class_t duf_help_option2class( duf_option_code_t code ); */

mas_error_code_t duf_option_O_smart_help_all( duf_option_class_t oclass );
mas_error_code_t duf_option_O_smart_help( duf_option_class_t oclass );
mas_error_code_t duf_option_O_help(  /* int argc, char *const *argv */ void );
mas_error_code_t duf_option_O_examples(  /* int argc, char *const *argv */ void );
mas_error_code_t duf_option_O_version(  /* int argc, char *const *argv */ void );
mas_error_code_t duf_option_O_showflags(  /* int argc, char *const *argv */ void );
mas_error_code_t duf_option_O_list_xtended( const char *s );

mas_error_code_t duf_option_O_list_options( long n );


#endif
