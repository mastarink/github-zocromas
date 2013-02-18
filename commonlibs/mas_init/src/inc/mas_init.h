#ifndef MAS_INIT_H
#  define MAS_INIT_H

int mas_pre_init( int argc, char **argv, char **env );
int mas_post_init( void );
int mas_init( int argc, char **argv, char **env );
int mas_init_plus( int argc, char **argv, char **env, ... );


void mas_destroy( void );

/* defined separately for client, server etc. */
void mas_atexit( void );

#endif
