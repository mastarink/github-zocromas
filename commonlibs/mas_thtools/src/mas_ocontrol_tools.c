#include <mastar/wrap/mas_std_def.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

/* #include <mastar/log/mas_log.h> */

#include "mas_thread_tools.h"
#include "mas_ocontrol_tools.h"



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
