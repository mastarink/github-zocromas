#ifndef MAS_DUF_OPTION_HELP_H
#  define MAS_DUF_OPTION_HELP_H




/* duf_option_class_t duf_help_option2class( duf_option_code_t code ); */

void duf_option_$_smart_help_all( duf_option_class_t oclass );
void duf_option_$_smart_help( duf_option_class_t oclass );
void duf_option_$_help(  /* int argc, char *const *argv */ void );
void duf_option_$_examples(  /* int argc, char *const *argv */ void );
void duf_option_$_version(  /* int argc, char *const *argv */ void );
void duf_option_$_showflags(  /* int argc, char *const *argv */ void );

void duf_option_$_list_targ( int *ptargc, char ***ptargv, long n );
void duf_option_$_clear_targ( int *ptargc, char ***ptargv, long n );
void duf_option_$_add_targ( int *ptargc, char ***ptargv, const char *s );

void duf_option_$_list_options( long n );

void duf_option_$_echo( const char *arg );

#endif
