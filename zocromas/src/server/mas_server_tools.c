#include "mas_basic_def.h"

/* server tools */

#include "mas_common.h"
#include "log/inc/mas_log.h"

#include "zoctools/inc/mas_thread_tools.h"

#include "listener/inc/mas_listener_control.h"
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



unsigned
mas_clients_count( mas_rcontrol_t * prcontrol )
{
  unsigned cnt = 0;

  if ( prcontrol )
  {
    mas_lcontrol_t *plcontrol = NULL;

    plcontrol = prcontrol->plcontrol;
/* mas_lcontrol_cleaning_transactions returns not-joined-count */
    if ( plcontrol )
      cnt = mas_lcontrol_cleaning_transactions( plcontrol, 0 /* removeit */ , 0 /* don't wait */  );
  }
  return cnt;
}

void
do_quit_server( mas_rcontrol_t * prcontrol, bin_type_t * pbinary )
{
  ctrl.keep_listening = 0;
  prcontrol->keep_alive = 0;
  MAS_LOG( "KA => %u", prcontrol->keep_alive );
  if ( pbinary )
  {
    if ( ctrl.restart )
      *pbinary = MSG_BIN_RESTART;
    else if ( ctrl.quit )
      *pbinary = MSG_BIN_QUIT;
    else
      *pbinary = MSG_BIN_DISCONNECT;
  }
  mas_listeners_cancel(  );
}

const char *
mas_sstatus( mas_status_t status )
{
  switch ( status )
  {
  case MAS_STATUS_NONE:
    return "NONE";
  case MAS_STATUS_START:
    return "START";
  case MAS_STATUS_INIT:
    return "INIT";
  case MAS_STATUS_OPEN:
    return "OPEN";
  case MAS_STATUS_WAIT:
    return "WAIT";
  case MAS_STATUS_WORK:
    return "WORK";
  case MAS_STATUS_CLOSE:
    return "CLOSE";
  case MAS_STATUS_STOP:
    return "STOP";
  case MAS_STATUS_END:
    return "END";
  }
  return NULL;
}
