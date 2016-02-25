#ifndef MAS_DUF_OPTIMPL_MISC_H
#  define MAS_DUF_OPTIMPL_MISC_H

# include <mastar/error/mas_error_types.h>                           /* mas_error_code_t ♣ */

mas_error_code_t duf_option_O_list_targ1( mas_argvc_t * targ, long n );
mas_error_code_t duf_option_O_list_targ2( int *ptargc, char ***ptargv, long n );

mas_error_code_t duf_option_O_clear_targ1( mas_argvc_t * targ, long n );
mas_error_code_t duf_option_O_clear_targ2( int *ptargc, char ***ptargv, long n );

mas_error_code_t duf_option_O_add_targ1( mas_argvc_t * targ, const char *s );
mas_error_code_t duf_option_O_add_targ2( int *ptargc, char ***ptargv, const char *s );

mas_error_code_t duf_option_O_echo( const char *arg );

mas_error_code_t duf_option_O_history( void );

mas_error_code_t duf_option_O_error_level( long lev );


#endif
