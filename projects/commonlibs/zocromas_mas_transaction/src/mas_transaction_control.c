#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib.h>

/* #include <mastar/channel/mas_channel_object.h> */
#include <mastar/channel/mas_channel_open.h>
#include <mastar/channel/mas_channel.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;

#include <mastar/log/mas_log.h>

#include "mas_transaction_control.h"

/*
this:
  mas_transaction_control.c
related:
  mas_transaction.c
  mas_transaction_control.h
  mas_transaction_control_types.h
  mas_server_master.c
  mas_transaction_xcromas.c
  mas_transaction_http.c

  mas_listener.c
  
  mas_lib_thread.c
  mas_cmdmod_server.c
  mas_server_tools.c
*/


int
mas_rcontrol_set_channel( mas_rcontrol_t * prcontrol, mas_channel_t * pchannel )
{
  int r = -1;

  if ( pchannel && !prcontrol->h.pchannel )
  {
    prcontrol->h.pchannel = mas_channel_clone( pchannel );
    pchannel->cloned = 1;
    /* prcontrol->serv = prcontrol->h.pchannel->serv; */
    /* prcontrol->port = prcontrol->h.pchannel->port; */
    /* tMSG( "to close2" ); */
    {
      /* preserve earlier error */
      r = mas_channel_close2( pchannel );
      MAS_LOG( "(%d) close2", r );
    }
  }
  return r;
}
