#include "mas_basic_def.h"

#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib.h>

/* #include <mastar/channel/mas_channel_object.h> */
#include <mastar/channel/mas_channel_open.h>
#include <mastar/channel/mas_channel.h>

#include "mas_common.h"
#include "log/inc/mas_log.h"

#include "transaction/inc/mas_transaction_control.h"

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
*/


int
mas_rcontrol_set_channel( mas_rcontrol_t * prcontrol, mas_channel_t * pchannel )
{
  int r = -1;

  if ( pchannel && !prcontrol->pchannel )
  {
    prcontrol->pchannel = mas_channel_clone( pchannel );
    pchannel->cloned = 1;
    /* prcontrol->serv = prcontrol->pchannel->serv; */
    /* prcontrol->port = prcontrol->pchannel->port; */
    tMSG( "to close2" );
    {
      /* preserve earlier error */
      r = mas_channel_close2( pchannel );
      MAS_LOG( "(%d) close2", r );
    }
  }
  return r;
}

const char *
mas_rcontrol_protocol_name( mas_rcontrol_t * prcontrol )
{
  const char *p = NULL;

  if ( prcontrol )
    switch ( prcontrol->proto )
    {
    case MAS_TRANSACTION_PROTOCOL_NONE:
      p = "NONE";
      break;
    case MAS_TRANSACTION_PROTOCOL_XCROMAS:
      p = "XCRO_MAS";
      break;
    case MAS_TRANSACTION_PROTOCOL_HTTP:
      p = "HTTP_MAS";
      break;
    default:
      p = "UNKNOWN";
      break;
    }
  return p;
}
