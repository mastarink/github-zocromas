#ifndef MAS_TRANSACTION_CONTROL_H
#  define MAS_TRANSACTION_CONTROL_H

#  include <mastar/types/mas_transaction_control_types.h>

/*
this:
  mas_transaction_control.h
related:
  mas_transaction.c
  mas_transaction.h
  mas_transaction_control.c
  mas_transaction_control_types.h
  mas_server_master.c
  mas_transaction_xcromas.c
  mas_transaction_http.c
  
  mas_lib_thread.c

*/


int mas_rcontrol_set_channel( mas_rcontrol_t * prcontrol, mas_channel_t * pchannel );

#endif
