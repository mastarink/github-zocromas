#include "mas_server_def.h"
#include "mas_basic_def.h"

/* server tools */

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>

#include <mastar/thtools/mas_thread_tools.h>

#include "listener/inc/mas_listeners.h"

#include "mas_server_tools.h"


/*
this:
  mas_server_tools.c
more:
  mas_cmdmod_server.c
  mas_init_server.c
  mas_init_server.h
 
  mas_server_cmd_client.h
  mas_server_cmd_get.h
  mas_server_cmd_mem.h
  mas_server_cmd_set.h
  mas_server_cmd_top.h
  mas_server_main.c
  mas_server_sig.c
  mas_server_sig.h
  mas_server_tools.h
  
  mas_transaction.c

*/



//  unsigned
//  mas_clients_count( mas_rcontrol_t * prcontrol )
//  {
//    unsigned cnt = 0;
//
//    if ( prcontrol )
//    {
//      mas_lcontrol_t *plcontrol = NULL;
//
//      plcontrol = prcontrol->plcontrol;
//  /* mas_lcontrol_cleaning_transactions returns not-joined-count */
//      if ( plcontrol )
//        cnt = mas_lcontrol_cleaning_transactions( plcontrol, 0 /* removeit */ , 0 /* don't wait */  );
//    }
//    return cnt;
//  }


