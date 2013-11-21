#ifndef MAS_INIT_PROTOS_H
#  define MAS_INIT_PROTOS_H


/* int mas_protos_load( mas_options_t * popts, const char * *message ); */
INIT_HANDLER( mas_protos_init );

int mas_protos_destroy( void );


/* #include <mastar/types/mas_common_cdefs.h> */
#endif
