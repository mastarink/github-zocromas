#include "mas_server_def.h"
#include "mas_basic_def.h"

#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>

/* #include "server/inc/mas_server_tools.h" */
#include <mastar/thtools/mas_ocontrol_tools.h>
#include <mastar/variables/mas_variables.h>
#include <mastar/fileinfo/mas_fileinfo.h>
#include <mastar/fileinfo/mas_fileinfo_object.h>

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
mas_proto_http_parse_request( mas_rcontrol_t * prcontrol, mas_http_t * http, const char *smessage )
{
  const char *pstring;

  /* EMSG( "@@>>>@@@@@@@@@@@ s1: %x", *( ( unsigned * ) smessage ) ); */
  pstring = smessage;
  /* EMSG( "@@>>>@@@@@@@@@@@ s2: %x", *( ( unsigned * ) pstring ) ); */
  MAS_LOG( "http parse 1" );
  /* if ( 0 == strncasecmp( pstring, "http:", 5 ) || 0 == strncasecmp( pstring, "https:", 6 ) ) */
  if ( http )
  {
    http->smethod = mas_proto_http_nonblank( pstring, &pstring );
    /* EMSG( "@@>>>@@@@@@@@@@@ s3: %x", *( ( unsigned * ) http->smethod ) ); */
    MAS_LOG( "http parse smethod: %s - %s", http->smethod, pstring );
    mas_proto_http_parse_method( http );
    MAS_LOG( "http parse method: %s (%u)", mas_proto_http_method_name( http ), http->imethod );
    http->URI = mas_proto_http_nonblank( pstring, &pstring );
    MAS_LOG( "URI: %s", http->URI );
    MAS_LOG( "http parse URI: %s", http->URI );
    http->protocol_name = mas_proto_http_nonc( pstring, &pstring, " /" );
    MAS_LOG( "http parse protocol: %s", http->protocol_name );
    if ( http->protocol_name && ( 0 == strcmp( http->protocol_name, "HTTP" ) || 0 == strcmp( http->protocol_name, "HTTPS" ) ) )
      prcontrol->proto = MAS_TRANSACTION_PROTOCOL_HTTP;
    else if ( http->URI && *http->URI == '/' && http->imethod != MAS_HTTP_METHOD_NONE  && http->imethod != MAS_HTTP_METHOD_BAD )
      prcontrol->proto = MAS_TRANSACTION_PROTOCOL_HTTP;
    MAS_LOG( "http parse protocol: %s === %s", mas_rcontrol_protocol_name( prcontrol ), http->protocol_name );
    if ( prcontrol->proto == MAS_TRANSACTION_PROTOCOL_HTTP )
    {
      if ( pstring && *pstring == '/' )
        pstring++;
      http->sversion = mas_proto_http_nonblank( pstring, &pstring );
      MAS_LOG( "http parse p.version" );
      sscanf( http->sversion, "%3f", &http->fversion );
      MAS_LOG( "HTTP parse f.version %s => %5.2f", http->sversion, http->fversion );
      if ( http->rest )
        mas_free( http->rest );
      http->rest = mas_strdup( pstring );
      http = mas_proto_http_parse_headers( prcontrol, http );
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
  }
  /* else                                          */
  /* {                                             */
  /*   MAS_LOG( "http parse 3 bad: %s", pstring ); */
  /*   mas_proto_http_delete_request( http );      */
  /*   http = NULL;                                */
  /* }                                             */
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

    if ( http->rest )
      mas_free( http->rest );

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

static mas_http_t *
mas_proto_http_parse_spc_headers( mas_rcontrol_t * prcontrol, mas_http_t * http, const char *name, const char *value )
{
  if ( 0 == strcmp( name, "Connection" ) )
  {
    rMSG( ">>>>>> HTTP HEADER: %s:%s", name, value );
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
mas_proto_http_parse_headers0( mas_rcontrol_t * prcontrol, mas_http_t * http )
{
  char *s;
  const char *pstring;

  prcontrol->keep_alive = ( http->fversion >= 1.1 );
  MAS_LOG( "KA => %u", prcontrol->keep_alive );
  pstring = http->rest;
  MAS_LOG( "http parse headers 1" );
  while ( pstring && *pstring )
  {
    char *name = NULL, *value = NULL;
    const char *sp;

    s = mas_proto_http_nonc( pstring, &pstring, "\r\n" );
    if ( !s || !*s )
      break;
    sp = s;
    MAS_LOG( "http parse headers 2" );
    if ( sp )
      name = mas_proto_http_nonc( sp, &sp, ": " );
    MAS_LOG( "http parse headers 3" );
    value = mas_strdup( sp );
    /* rMSG( "HTTP HEADER:'%s' --> '%s' = '%s'", s, name, value ); */
    http->indata = mas_variable_create_x( http->indata, MAS_THREAD_TRANSACTION, "inheader", name, NULL, "%s", value, 0 );

    http = mas_proto_http_parse_spc_headers( prcontrol, http, name, value );
    if ( name )
      mas_free( name );
    if ( value )
      mas_free( value );
    if ( s )
      mas_free( s );
    MAS_LOG( "http parse headers 4" );
  }
  return http;
}

mas_http_t *
mas_proto_http_parse_headers( mas_rcontrol_t * prcontrol, mas_http_t * http )
{
  char *pstring;

  prcontrol->keep_alive = ( http->fversion >= 1.1 );
  MAS_LOG( "KA => %u", prcontrol->keep_alive );
  pstring = http->rest;
  MAS_LOG( "http parse headers 1" );
  while ( pstring && *pstring && *pstring > ' ' )
  {
    char *name = NULL, *ename = NULL, *value = NULL, *evalue = NULL;

    /* fprintf( stderr, "\n'%s'\n", pstring ); */
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
    while ( evalue && *evalue >= ' ' )
      evalue++;

    name = mas_strndup( name, ename - name );
    value = mas_strndup( value, evalue - value );
    MAS_LOG( "http parse headers name:'%s' value:'%s'", name, value );
    /* fprintf( stderr, "http parse headers name:'%s' value:'%s'\n", name, value ); */

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


    http = mas_proto_http_parse_spc_headers( prcontrol, http, name, value );
    if ( name )
      mas_free( name );
    if ( value )
      mas_free( value );
    MAS_LOG( "http parse headers 4" );
    pstring = evalue;
    while ( pstring && *pstring && *pstring < ' ' )
      pstring++;
  }
  return http;
}
