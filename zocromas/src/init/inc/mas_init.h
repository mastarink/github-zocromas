#ifndef MAS_INIT_H
#  define MAS_INIT_H

void mas_pre_init( int argc, char **argv, char **env );
void mas_init( void ( *atexit_fun ) ( void ), int initsig, int argc, char **argv, char **env );
void mas_destroy( void );

void mas_init_sig( void );

/* defined separately for client, server etc. */
void mas_atexit( void );

#endif
