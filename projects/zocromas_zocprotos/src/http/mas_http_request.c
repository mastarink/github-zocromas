#include <mastar/wrap/mas_std_def.h>

#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/types/mas_control_types.h>
extern mas_control_t ctrl;

/* dir.post, uuid */
#include <mastar/types/mas_opts_types.h>
extern mas_options_t opts;

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>

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



static void
mas_proto_add_host( const void *env, const char *section, const char *sectvalue, const char *name, const char *value )
{
  mas_transaction_protodesc_t *proto_desc;

  proto_desc = ( mas_transaction_protodesc_t * ) env;
  HMSG( "WOW '%s:%s'.'%s' : '%s'", section, sectvalue, name, value );
  proto_desc->variables = mas_variable_create_text( proto_desc->variables, MAS_THREAD_NONE, "docroot", sectvalue, value, 0 );
}

static mas_option_parse_t opt_table[] = {
  {.section = "host",.name = "docroot",.type = MAS_OPT_TYPE_FUNC,.func = mas_proto_add_host}
  ,
};






mas_http_t *
mas_proto_http_create_request( mas_rcontrol_t * prcontrol )
{
  mas_http_t *http = NULL;

  http = mas_malloc( sizeof( mas_http_t ) );
  memset( http, 0, sizeof( mas_http_t ) );
  http->prcontrol = prcontrol;
  http->reply_content = mas_fileinfo_create(  );
  return http;
}

void
cb( void *arg )
{
  struct timeval td;
  mas_rcontrol_t *prcontrol;

  prcontrol = ( mas_rcontrol_t * ) arg;
  gettimeofday( &td, NULL );
  prcontrol->h.activity_time = td;
}


mas_http_t *
mas_proto_http_parse_request( mas_rcontrol_t * prcontrol, mas_http_t * http )
{
  char *pstring;
  const char *cstring;

#ifdef MAS_HTTP_MULTIPART
  {
    char bcpath[512];

    snprintf( bcpath, sizeof( bcpath ), "%s/%s.%lu-%u.part%u.post-%lu", opts.dir.post ? opts.dir.post : "/tmp",
              opts.uuid, prcontrol->h.serial, ctrl.pserver_thread->pid, 0, time( NULL ) );
    /* mas_channel_buffer_strip( prcontrol->h.pchannel, 0 ); */
    mas_channel_set_buffer_copy( prcontrol->h.pchannel, bcpath );
    HMSG( "ANY BODY %s ? [%s]", bcpath, opts.dir.post );
  }
#endif
  HMSG( "HTTP REQUEST (parse)" );
  pstring = mas_channel_buffer_nl_dup( prcontrol->h.pchannel );
  HMSG( "HTTP REQUEST (parse) '%s'", pstring );
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
      HMSG( "HTTP METHOD: #%d : %s", http->imethod, mas_proto_http_method_name( http ) );

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
        /* prcontrol->proto_desc = proto_desc; */
        if ( !prcontrol->proto_desc->variables )
        {
          HMSG( "HTTP proto vars" );
          ( void ) _mas_opts_restore_relative( "proto/http.conf", NULL /*popts */ , opt_table, sizeof( opt_table ) / sizeof( opt_table[0] ),
                                               ( const void * ) prcontrol->proto_desc /* arg */ , NULL, NULL, NULL );
        }

        MAS_LOG( "good, http parsed protocol: %s === %s", prcontrol->proto_desc ? prcontrol->proto_desc->name : "?", http->protocol_name );
        if ( cstring && *cstring == '/' )
          cstring++;
        http->sversion = mas_proto_http_nonblank( cstring, &cstring );
        MAS_LOG( "http parse p.version" );
        sscanf( http->sversion, "%3f", &http->fversion );
        MAS_LOG( "HTTP parse f.version %s => %5.2f", http->sversion, http->fversion );
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
            break;
          case MAS_HTTP_METHOD_POST:
            {
/*        HTTP Host='zocromas.mastar.lan:60006'                                                                        */
/*        HTTP User-Agent='Mozilla/5.0 (X11; Linux x86_64; rv:10.0.11) Gecko/20100101 Firefox/10.0.11'                */
/*        HTTP Accept='text/html,application/xhtml+xml,application/xml;q=0.9,*)*;q=0.8'                               */
/*        HTTP Accept-Language='uk,ru;q=0.8,en-us;q=0.5,en;q=0.3'                                                     */
/*        HTTP Accept-Encoding='gzip, deflate'                                                                        */
/*        HTTP DNT='1'                                                                                                */
/*        HTTP Connection='keep-alive'                                                                                */
/*  (+)   HTTP Connection                                                                                             */
/*        HTTP Referer='http://zocromas.mastar.lan:60006/post.html'                                                    */
/*        HTTP Content-Type='multipart/form-data; boundary=---------------------------213557806320239878091684659415' */
/*        HTTP Content-Length='2473'                                                                                  */

              /* boundary = mas_variables_find( http->indata, "inheader", "Host" ); */

#ifdef MAS_HTTP_MULTIPART
              http = mas_proto_http_parse_multipart( prcontrol, http );
#else
              mas_proto_http_delete_request( http );
              http = NULL;
#endif
            }
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
  else
  {
    mas_proto_http_delete_request( http );
    http = NULL;
  }
  if ( http && http->indata )
    mas_variables_log_pairs( http->indata, "inheader" );
#ifdef MAS_HTTP_MULTIPART
  mas_channel_set_buffer_copy( prcontrol->h.pchannel, NULL );
#endif
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
    http->protocol_name = NULL;

    if ( http->boundary )
      mas_free( http->boundary );
    http->boundary = NULL;

    if ( http->smethod )
      mas_free( http->smethod );
    http->smethod = NULL;

    if ( http->URI )
      mas_free( http->URI );
    http->URI = NULL;

    if ( http->sversion )
      mas_free( http->sversion );
    http->sversion = NULL;

    if ( http->docroot )
      mas_free( http->docroot );
    http->docroot = NULL;

    if ( http->host )
      mas_free( http->host );
    http->host = NULL;

    mas_fileinfo_delete( http->reply_content );
    http->reply_content = NULL;

    mas_fileinfo_delete( http->request_content );
    http->request_content = NULL;

    if ( http->indata )
      mas_variables_delete( http->indata );
    http->indata = NULL;

    if ( http->outdata )
      mas_variables_delete( http->outdata );
    http->outdata = NULL;

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
      HMSG( "HTTP METHOD %s", http->smethod );
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
