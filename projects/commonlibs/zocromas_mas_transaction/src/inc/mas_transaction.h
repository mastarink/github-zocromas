#ifndef MAS_TRANSACTION_H
#  define MAS_TRANSACTION_H

#  include <mastar/types/mas_transaction_control_types.h>

/*
this:
  mas_transaction.h
related:
  mas_transaction.c
  mas_transaction_control.c
  mas_transaction_control.h
  mas_transaction_control_types.h
  mas_server_master.c
  mas_transaction_xcromas.c
  mas_transaction_http.c
  
  mas_lib_thread.c
  mas_server_tools.c

*/

#  ifdef MAS_TR_PERSIST
int mas_transaction_start( mas_lcontrol_t * plcontrol, unsigned persistent_transaction );
#  else
int mas_transaction_start( mas_lcontrol_t * plcontrol );
#  endif
int mas_transaction_cancel( mas_rcontrol_t * prcontrol );

/* void *mas_transaction( mas_rcontrol_t * prcontrol ); */

/* void *mas_transaction_th( void *trcontrol ); */

void mas_transaction_cleanup( void *arg );

int mas_transaction_write( const mas_rcontrol_t * prcontrol, char *cbuf, size_t sz );
int mas_transaction_vwritef( const mas_rcontrol_t * prcontrol, const char *fmt, va_list args );



#endif
