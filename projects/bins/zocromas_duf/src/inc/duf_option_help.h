#ifndef MAS_DUF_OPTION_HELP_H
#  define MAS_DUF_OPTION_HELP_H

#  include "duf_option_types.h"
#  include "duf_options_enum.h"



/* duf_option_class_t duf_help_option2class( duf_option_code_t code ); */

void duf_option_smart_help_all( duf_option_class_t oclass );
void duf_option_smart_help( duf_option_class_t oclass );
void duf_option_help( int argc, char *const *argv );
void duf_option_examples( int argc, char *const *argv );
void duf_option_version( int argc, char *const *argv );
void duf_option_showflags( int argc, char *const *argv );

void duf_option_list_sccb( int x );
void duf_option_list_targ( int *ptargc, char ***ptargv, long n );
void duf_option_clear_targ( int *ptargc, char ***ptargv, long n );
void duf_option_add_targ( int *ptargc, char ***ptargv, const char *s );

#endif
