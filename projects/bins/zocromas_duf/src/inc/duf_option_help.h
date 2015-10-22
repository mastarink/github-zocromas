#ifndef MAS_DUF_OPTION_HELP_H
#  define MAS_DUF_OPTION_HELP_H




/* duf_option_class_t duf_help_option2class( duf_option_code_t code ); */

mas_error_code_t duf_option_$_smart_help_all( duf_option_class_t oclass );
mas_error_code_t duf_option_$_smart_help( duf_option_class_t oclass );
mas_error_code_t duf_option_$_help(  /* int argc, char *const *argv */ void );
mas_error_code_t duf_option_$_examples(  /* int argc, char *const *argv */ void );
mas_error_code_t duf_option_$_version(  /* int argc, char *const *argv */ void );
mas_error_code_t duf_option_$_showflags(  /* int argc, char *const *argv */ void );
mas_error_code_t duf_option_$_list_xtended( const char *s );

mas_error_code_t duf_option_$_list_options( long n );


#endif
