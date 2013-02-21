#ifndef MAS_LISTENERS_H
#  define MAS_LISTENERS_H

int mas_listeners_start( void );
int mas_listeners_stop( void );
int mas_listeners_cancel( void );
int mas_listeners_wait( void );


#  undef MAS_WEAK
#endif
