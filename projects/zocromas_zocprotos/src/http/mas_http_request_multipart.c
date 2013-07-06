#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>

#include <mastar/types/mas_control_types.h>
/* dir.post, uuid */
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;

#include <mastar/init/mas_opts_common.h>

/* #include "server/inc/mas_server_tools.h" */
#include <mastar/thtools/mas_ocontrol_tools.h>
#include <mastar/variables/mas_variables.h>
#include <mastar/fileinfo/mas_fileinfo.h>
#include <mastar/fileinfo/mas_fileinfo_object.h>

#include <mastar/channel/mas_channel_buffer.h>
#include <mastar/channel/mas_channel.h>

#include "mas_http_utils.h"

#include "mas_http_request_headers.h"
#include "mas_http_request.h"


/*
this:
  mas_http_request.c
more:
  mas_http_get.c
  mas_http_get.h
  mas_http_reply.c
  mas_http_reply.h
  mas_http_request.h
  mas_http_types.h
  mas_http_utils.c
  mas_http_utils.h
  mas_transaction.c
  mas_transaction_http.c
  mas_transaction_http.h
  
  mas_server_tools.c
  mas_log.c

*/


#ifdef MAS_HTTP_MULTIPART
mas_http_t *
mas_proto_http_parse_multipart( mas_rcontrol_t * prcontrol, mas_http_t * http )
{
  char *eol_boundary;
  size_t len;
  size_t offset;
  int np;

  eol_boundary = mas_strdup( "\r\n--" );
  eol_boundary = mas_strcat_x( eol_boundary, http->boundary );
  np = 0;
  offset = 2;
  len = strlen( eol_boundary );
  while ( 1 )
  {
    char cname[512];
    const char *f;
    char *f1;


    f1 = NULL;
    snprintf( cname, sizeof( cname ), "%s/%s.%lu-%u.part%u.post-%lu", opts.dir.post ? opts.dir.post : "/tmp",
              opts.uuid, prcontrol->h.serial, ctrl.pserver_thread->pid, ++np, time( NULL ) );

    mas_channel_set_buffer_copy( prcontrol->h.pchannel, cname );
    HMSG( "FFF %s", cname );
    f = mas_channel_search( prcontrol->h.pchannel, eol_boundary + offset, len - offset, cb, prcontrol );
    if ( f )
    {
      char *pstring;

      f1 = mas_strndup( f, len + 2 );
      /* HMSG( "NEEDLE:'%s'", eol_boundary ); */
      /* HMSG( "%d FOUND:%s ...[[ %02x %02x %02x ]]", np, f1, f[len - offset], f[len - offset + 1], f[len - offset + 2] ); */
      mas_free( f1 );
      mas_channel_set_buffer_copy( prcontrol->h.pchannel, NULL );
      if ( f[len - offset] == '-' && f[len - offset + 1] == '-' )
        break;
      /* HMSG( "IPTR:%lu",prcontrol-> pchannel->buffer.iptr ); */
      HMSG( "ZZZ" );
      pstring = mas_channel_buffer_nl_dup( prcontrol->h.pchannel );
      HMSG( "PS:'%s'", pstring );
      mas_free( pstring );
      {
        int b = 0;

        do
        {
          pstring = mas_channel_buffer_nl_dup( prcontrol->h.pchannel );
          HMSG( "PS PLUS:'%s'", pstring );
          b = ( pstring && *pstring != '\n' && *pstring != '\r' ? 1 : 0 );
          mas_free( pstring );
          pstring = NULL;
        }
        while ( b );
      }
    }
    offset = 0;
  }
  mas_channel_set_buffer_copy( prcontrol->h.pchannel, NULL );
  mas_free( eol_boundary );
  return http;
}
#endif
