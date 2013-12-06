#ifndef MAS_LISTENERS_H
#  define MAS_LISTENERS_H

int mas_listeners_start( const mas_options_t * popts );
int mas_listeners_stop( void );
int mas_listeners_cancel( void );
int mas_listeners_wait( void );
int mas_listeners_main( const mas_options_t * popts );


#  undef MAS_WEAK
#endif
