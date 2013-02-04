#ifndef MAS_ARG_TOOLS_H
#  define MAS_ARG_TOOLS_H

size_t mas_wordlen( const char *s );
char *mas_chomp( char *s );
int mas_strcmp2( const char *s1, const char *s2 );
int mas_strcmpv( const char *s1, const char *s2, char **vval );
const char *mas_find_eq_value( const char *s );

const char *mas_skip_space( const char *args );

/* int __mas_make_argv( const char *args, int argc, char ***pargv, int from_a ); */

int mas_add_argv_args( int targc, char ***ptargv, const char *args, int ia_offset );
int mas_add_argv_argv( int targc, char ***ptargv, int argc, char *argv[], int ia_offset );
int mas_add_argv_arg( int targc, char ***ptargv, const char *arg );

int mas_del_argv( int argc, char **argv, int from_a );

char *mas_argv_string( int targc, char *const *targv, int from_a );

const char *mas_find_next_arg( const char *args, const char **pthis, size_t * plen, int *quot );

char *mas_strcat_xt( char *s1, const char *s2 );
char *mas_strncat_xt( char *s1, const char *s2, size_t maxs2 );

#endif
