#ifndef MAS_DUF_OPTION_MISC_H
#  define MAS_DUF_OPTION_MISC_H

duf_error_code_t duf_option_$_list_targ( int *ptargc, char ***ptargv, long n );
duf_error_code_t duf_option_$_clear_targ( int *ptargc, char ***ptargv, long n );
duf_error_code_t duf_option_$_add_targ( int *ptargc, char ***ptargv, const char *s );

duf_error_code_t duf_option_$_echo( const char *arg );

duf_error_code_t duf_option_$_history( void );

#endif
