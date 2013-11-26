#ifndef MAS_INIT_H
#  define MAS_INIT_H

/* int mas_post_init( void ); */
int mas_init( mas_options_t * popts, int argc, char **argv, char **env );
int mas_init_plus( mas_options_t * popts, int argc, char **argv, char **env, ... );

int mas_init_set_n( mas_options_t * popts, /* int argc, char **argv, char **env, */ int funcnt, mas_init_fun_t * init_funcs );
int mas_init_set_z( mas_options_t * popts, /* int argc, char **argv, char **env, */ mas_init_fun_t * init_funcs );

void mas_destroy( mas_options_t * popts );

/* defined separately for client, server etc. */
void mas_atexit( void );

/* #include <mastar/types/mas_common_cdefs.h> */
#endif
