#ifndef MAS_LISTENERS_H
#  define MAS_LISTENERS_H

int mas_listeners_start( MAS_PASS_OPTS_DECLARE1 );
int mas_listeners_stop( void );
int mas_listeners_cancel( void );
int mas_listeners_wait( void );


#  undef MAS_WEAK
#endif
