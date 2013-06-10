#ifndef MAS_INIT_H
#  define MAS_INIT_H

/* int mas_pre_init( int argc, char **argv, char **env ); */
/* int mas_post_init( void ); */
int mas_init( MAS_PASS_OPTS_DECLARE int argc, char **argv, char **env );
int mas_init_plus( MAS_PASS_OPTS_DECLARE int argc, char **argv, char **env, ... );


void mas_destroy( MAS_PASS_OPTS_DECLARE1 );

/* defined separately for client, server etc. */
void mas_atexit( void );

#endif
