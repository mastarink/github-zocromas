#ifndef MAS_DUF_OPTION_MISC_H
#  define MAS_DUF_OPTION_MISC_H

void duf_option_$_list_targ( int *ptargc, char ***ptargv, long n );
void duf_option_$_clear_targ( int *ptargc, char ***ptargv, long n );
void duf_option_$_add_targ( int *ptargc, char ***ptargv, const char *s );

void duf_option_$_echo( const char *arg );

void duf_option_$_history( void );

#endif
