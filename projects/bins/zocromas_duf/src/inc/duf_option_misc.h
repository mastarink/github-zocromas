#ifndef MAS_DUF_OPTION_MISC_H
#  define MAS_DUF_OPTION_MISC_H

duf_error_code_t duf_option_$_list_targ1( mas_argvc_t * targ, long n );
duf_error_code_t duf_option_$_list_targ2( int *ptargc, char ***ptargv, long n );

duf_error_code_t duf_option_$_clear_targ1( mas_argvc_t * targ, long n );
duf_error_code_t duf_option_$_clear_targ2( int *ptargc, char ***ptargv, long n );

duf_error_code_t duf_option_$_add_targ1( mas_argvc_t * targ, const char *s );
duf_error_code_t duf_option_$_add_targ2( int *ptargc, char ***ptargv, const char *s );

duf_error_code_t duf_option_$_echo( const char *arg );

duf_error_code_t duf_option_$_history( void );

duf_error_code_t duf_option_$_error_level( long lev );


#endif
