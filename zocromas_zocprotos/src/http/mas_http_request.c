#include <mastar/wrap/mas_std_def.h>

#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>

#include <mastar/types/mas_control_types.h>
extern mas_control_t ctrl;

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>

/* #include "server/inc/mas_server_tools.h" */
#include <mastar/thtools/mas_ocontrol_tools.h>
#include <mastar/variables/mas_variables.h>
#include <mastar/fileinfo/mas_fileinfo.h>
#include <mastar/fileinfo/mas_fileinfo_object.h>

#include <mastar/channel/mas_channel_buffer.h>
#include <mastar/channel/mas_channel.h>

#include "mas_http_utils.h"
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

mas_http_t *
mas_proto_http_create_request( mas_rcontrol_t * prcontrol )
{
  mas_http_t *http = NULL;

  http = mas_malloc( sizeof( mas_http_t ) );
  memset( http, 0, sizeof( mas_http_t ) );
  http->prcontrol = prcontrol;
  http->content = mas_fileinfo_create(  );
  return http;
}

mas_http_t *
mas_proto_http_parse_request( mas_rcontrol_t * prcontrol, const mas_transaction_protodesc_t * proto_desc, mas_http_t * http )
{
  char *pstring;
  const char *cstring;

  {
    char bcpath[512];

    snprintf( bcpath, sizeof( bcpath ), "/tmp/%u-%lu-%lu-%u.tmp", ctrl.pserver_thread->pid, prcontrol->h.serial, time( NULL ), __LINE__ );
    /* mas_channel_buffer_strip( prcontrol->h.pchannel, 0 ); */
    mas_channel_set_buffer_copy( prcontrol->h.pchannel, bcpath );
    HMSG( "ANY BODY %s ?", bcpath );
  }
  HMSG( "HTTP REQUEST (parse)" );
  pstring = mas_channel_buffer_nl_dup( prcontrol->h.pchannel );
  if ( pstring )
  {
    cstring = ( const char * ) pstring;
    MAS_LOG( "http parse 1" );
    if ( http )
    {
      int good = 0;

      http->smethod = mas_proto_http_nonblank( cstring, &cstring );
      MAS_LOG( "http parse smethod: %s - %s", http->smethod, cstring );
      mas_proto_http_parse_method( http );
      HMSG( "HTTP METHOD: %d. %s", http->imethod, mas_proto_http_method_name( http ) );
      MAS_LOG( "http parse method: %s (%u)", mas_proto_http_method_name( http ), http->imethod );
      http->URI = mas_proto_http_nonblank( cstring, &cstring );
      MAS_LOG( "URI: %s", http->URI );
      MAS_LOG( "http parse URI: %s", http->URI );
      http->protocol_name = mas_proto_http_nonc( cstring, &cstring, " /" );
      MAS_LOG( "http to parse protocol: %s", http->protocol_name );
      if ( http->protocol_name && ( 0 == strcmp( http->protocol_name, "HTTP" ) || 0 == strcmp( http->protocol_name, "HTTPS" ) ) )
        good = 1;
      else if ( http->URI && *http->URI == '/' && http->imethod != MAS_HTTP_METHOD_NONE && http->imethod != MAS_HTTP_METHOD_BAD )
        good = 1;
      if ( good )
      {
        prcontrol->proto_desc = proto_desc;
        MAS_LOG( "good (1), http parsed protocol: %s === %s", prcontrol && proto_desc ? proto_desc->name : "?", http->protocol_name );
        MAS_LOG( "good (2), http parsed protocol: %s === %s", prcontrol
                 && prcontrol->proto_desc ? prcontrol->proto_desc->name : "?", http->protocol_name );
        if ( cstring && *cstring == '/' )
          cstring++;
        http->sversion = mas_proto_http_nonblank( cstring, &cstring );
        MAS_LOG( "http parse p.version" );
        sscanf( http->sversion, "%3f", &http->fversion );
        MAS_LOG( "HTTP parse f.version %s => %5.2f", http->sversion, http->fversion );
        http = mas_proto_http_parse_headers( prcontrol, http );

        {
          char *pstring = NULL;

#if 1
          {
            char bcpath[512];

            snprintf( bcpath, sizeof( bcpath ), "/tmp/%u-%lu-%lu-%u.tmp", ctrl.pserver_thread->pid, prcontrol->h.serial, time( NULL ),
                      __LINE__ );
            /* mas_channel_buffer_strip( prcontrol->h.pchannel, 0 ); */
            mas_channel_set_buffer_copy( prcontrol->h.pchannel, bcpath );
            HMSG( "ANY BODY %s ?", bcpath );
          }
#endif
          while ( 0 )
          {
            pstring = mas_channel_buffer_nl_dup( prcontrol->h.pchannel );
            if ( !pstring )
              break;
            /* ( pstring && *pstring && *pstring > ' ' ); */
            HMSG( "pSTRING: {%s}", pstring );
            if ( pstring )
              mas_free( pstring );
            pstring = NULL;
          }
        }


        {
          switch ( http->imethod )
          {
          case MAS_HTTP_METHOD_BAD:
          case MAS_HTTP_METHOD_NONE:
            /* Not HTTP */
            mas_proto_http_delete_request( http );
            http = NULL;
            break;
          case MAS_HTTP_METHOD_UNKNOWN:
            /* 501 Method Not Implemented */
            /* Allow: GET,HEAD,POST,OPTIONS */
            /* Title: 501 Method Not Implemented */
            /* mas_proto_http_delete_request( http ); */
            /* http = NULL;                           */
            prcontrol->keep_alive = 0;
            MAS_LOG( "KA => %u", prcontrol->keep_alive );
            break;
          case MAS_HTTP_METHOD_GET:
          case MAS_HTTP_METHOD_HEAD:
          case MAS_HTTP_METHOD_POST:
            /* prcontrol->keep_alive = 0;                    */
            /* MAS_LOG( "KA => %u", prcontrol->keep_alive ); */
            break;
          case MAS_HTTP_METHOD_PUT:
            prcontrol->keep_alive = 0;
            MAS_LOG( "KA => %u", prcontrol->keep_alive );
            break;
          case MAS_HTTP_METHOD_OPTIONS:
            prcontrol->keep_alive = 0;
            MAS_LOG( "KA => %u", prcontrol->keep_alive );
            break;
          }
        }
      }
      else
      {
        mas_proto_http_delete_request( http );
        http = NULL;
      }
    }
    mas_free( pstring );
  }
  if ( http && http->indata )
    mas_variables_log_pairs( http->indata, "inheader" );
  return http;
}

int
mas_proto_http_delete_request( mas_http_t * http )
{
  MAS_LOG( "http: to delete rq" );
  if ( http )
  {
    /* .......... */
    if ( http->protocol_name )
      mas_free( http->protocol_name );
    if ( http->smethod )
      mas_free( http->smethod );
    if ( http->URI )
      mas_free( http->URI );
    if ( http->sversion )
      mas_free( http->sversion );
    if ( http->docroot )
      mas_free( http->docroot );
    if ( http->host )
      mas_free( http->host );

    mas_fileinfo_delete( http->content );
    http->content = NULL;

    if ( http->indata )
      mas_variables_delete( http->indata );
    if ( http->outdata )
      mas_variables_delete( http->outdata );
    mas_free( http );
  }
  return 0;
}

static const mas_http_method_pair_t emethods[] = {
  {MAS_HTTP_METHOD_NONE, MSG_SSIGNATURE}
  , {MAS_HTTP_METHOD_GET, "GET"}
  , {MAS_HTTP_METHOD_HEAD, "HEAD"}
  , {MAS_HTTP_METHOD_PUT, "PUT"}
  , {MAS_HTTP_METHOD_POST, "POST"}
  , {MAS_HTTP_METHOD_OPTIONS, "OPTIONS"}
  , {MAS_HTTP_METHOD_UNKNOWN, "UNKNOWN"}
  , {.id = MAS_HTTP_METHOD_NONE,.name = NULL}
};

mas_http_method_t
mas_proto_http_parse_method( mas_http_t * http )
{
  if ( http )
  {
    http->imethod = MAS_HTTP_METHOD_NONE;
    if ( http->smethod )
    {
      http->imethod = MAS_HTTP_METHOD_UNKNOWN;
      /* EMSG( "@@@@@@@@@@@@@ smethod: %s %x", http->smethod, *( ( unsigned * ) http->smethod ) ); */
      for ( int im = 0; im < sizeof( emethods ) / sizeof( emethods[0] ); im++ )
      {
        if ( emethods[im].name )
        {
          if ( 0 == strcmp( emethods[im].name, http->smethod ) )
          {
            http->imethod = emethods[im].id;
            mas_free( http->smethod );
            http->smethod = NULL;
            break;
          }
        }
      }
    }
  }
  return http ? http->imethod : MAS_HTTP_METHOD_NONE;
}

const char *
mas_proto_http_method_name( mas_http_t * http )
{
  const char *p = "UNKNOWN";

  if ( http )
  {
    for ( int im = 0; im < sizeof( emethods ) / sizeof( emethods[0] ); im++ )
    {
      if ( emethods[im].name )
      {
        if ( http->imethod == emethods[im].id )
        {
          p = emethods[im].name;
          break;
        }
      }
    }
  }
  return p;
}

/* const char *                                                                */
/* mas_proto_http_method_name( mas_http_t * http )                             */
/* {                                                                           */
/*   const char *name = NULL;                                                  */
/*                                                                             */
/*   for ( int im = 0; im < sizeof( emethods ) / sizeof( emethods[0] ); im++ ) */
/*   {                                                                         */
/*     if ( http->imethod == emethods[im].id )                                 */
/*     {                                                                       */
/*       name = emethods[im].name;                                             */
/*       break;                                                                */
/*     }                                                                       */
/*   }                                                                         */
/*   return name;                                                              */
/* }                                                                           */

static mas_http_t *
mas_proto_http_parse_spc_header( mas_rcontrol_t * prcontrol, mas_http_t * http, const char *name, const char *value )
{
  if ( 0 == strcmp( name, "Connection" ) )
  {
    HMSG( "HTTP (spc) HEADER (parse) %s", name );
    MAS_LOG( "HTTP HEADER: %s:%s", name, value );
    if ( 0 == strcasecmp( value, "keep-alive" ) )
    {
      rMSG( ">>>>>> HTTP HEADER: Connection is Keep-Alive" );
      MAS_LOG( "HTTP HEADER: Connection is Keep-Alive" );
      prcontrol->keep_alive = 1;
      MAS_LOG( "KA => %u", prcontrol->keep_alive );
    }
    else if ( 0 == strcasecmp( value, "close" ) )
    {
      rMSG( ">>>>>> HTTP HEADER: Connection is Keep-Alive" );
      MAS_LOG( "HTTP HEADER: Connection is Keep-Alive" );
      prcontrol->keep_alive = 0;
      MAS_LOG( "KA => %u", prcontrol->keep_alive );
    }
  }
  return http;
}

mas_http_t *
mas_proto_http_parse_header( mas_rcontrol_t * prcontrol, mas_http_t * http, char *pstring )
{
  char *name = NULL, *ename = NULL, *value = NULL, *evalue = NULL;

  name = pstring;
  ename = pstring;
  while ( ename && *ename > ' ' && *ename != ':' )
    ename++;
  value = ename;
  if ( value && *value == ':' )
    value++;
  while ( value && *value == ' ' )
    value++;
  evalue = value;
  while ( evalue && *evalue && *evalue >= ' ' )
    evalue++;

  name = mas_strndup( name, ename - name );
  value = mas_strndup( value, evalue - value );
  MAS_LOG( "http parse headers name:'%s' value:'%s'", name, value );

  HMSG( "HTTP HEADER (parse) %s='%s'", name, value );
  http->indata = mas_variable_create_x( http->indata, MAS_THREAD_TRANSACTION, "inheader", name, NULL, "%s", value, 0 );

  if ( 0 == strcasecmp( name, "User-Agent" ) )
  {
    if ( 0 == strncmp( value, "httperf", 7 ) )
    {
      ctrl.messages = 0;
    }
    else
    {
    }
  }
  else
  {
  }

  http = mas_proto_http_parse_spc_header( prcontrol, http, name, value );
  if ( name )
    mas_free( name );
  if ( value )
    mas_free( value );
  MAS_LOG( "http parse headers 4" );
  return http;
}

mas_http_t *
mas_proto_http_parse_headers( mas_rcontrol_t * prcontrol, mas_http_t * http )
{
  char *pstring = NULL;

  HMSG( "HTTP HEADERS (parse)" );
  prcontrol->keep_alive = ( http->fversion >= 1.1 );
  MAS_LOG( "KA => %u", prcontrol->keep_alive );
  MAS_LOG( "http parse headers 1" );
  do
  {
    if ( pstring )
      mas_free( pstring );
    pstring = NULL;
    pstring = mas_channel_buffer_nl_dup( prcontrol->h.pchannel );
    http = mas_proto_http_parse_header( prcontrol, http, pstring );
  }
  while ( pstring && *pstring && *pstring > ' ' );
  /* while ( ( pstring = mas_channel_buffer_nl_dup( prcontrol->h.pchannel ) ) && *pstring && *pstring > ' ' ) */
  /* {                                                                                                        */
  /*   http = mas_proto_http_parse_header( prcontrol, http, pstring );                                        */
  /*   if ( pstring )                                                                                         */
  /*     mas_free( pstring );                                                                                 */
  /*   pstring = NULL;                                                                                        */
  /* }                                                                                                        */
  if ( pstring )
    mas_free( pstring );
  pstring = NULL;
  return http;
}
