#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>
/* mas_xvstrftime */
#include <mastar/tools/mas_tools.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/log/mas_log.h>



#include <mastar/varvec/mas_varvec.h>
#include <mastar/varvec/mas_varvec_search.h>
#include <mastar/varvec/mas_varvec_search.h>
#include <mastar/varvec/mas_varvec_headtail.h>
#include <mastar/varvec/mas_varvec_namevalue.h>
#include <mastar/varset/mas_varset_search.h>
#include <mastar/varset/mas_varset.h>

#ifdef MAS_HTTP_USE_FILEINFO
#  include <mastar/fileinfo/mas_unidata.h>
#  include <mastar/fileinfo/mas_fileinfo.h>
#  include <mastar/fileinfo/mas_fileinfo_object.h>
#elif defined( MAS_HTTP_USE_AUTOOBJECT )
#  include <mastar/autoobject/mas_autoobject_object.h>
#  include <mastar/autoobject/mas_autoobject.h>
#endif

/* mas_channel_fd */
#include <mastar/channel/mas_channel.h>
#include <mastar/channel/mas_channel_open.h>

#include "mas_http_types.h"

#include "mas_http_utils.h"
#include "mas_http_request.h"
#include "mas_http_reply.h"

/*
this:
  mas_http_reply.c
more:
  mas_http_get.c
  mas_http_get.h
  mas_http_reply.h
  mas_http_request.c
  mas_http_request.h
  mas_http_types.h
  mas_http_utils.c
  mas_http_utils.h
  mas_transaction.c
  mas_transaction_http.c
  mas_transaction_http.h

*/

/* moved to fileinfo */
/* mas_http_t *                                                                                            */
/* mas_http_make_etag( mas_rcontrol_t * prcontrol, mas_http_t * http )                                     */
/* {                                                                                                       */
/*   HMSG( "HTTP make ETAG" );                                                                             */
/*   if ( http )                                                                                           */
/*     ( void ) (* r = *) mas_fileinfo_make_etag( http->reply_content );                                   */
/*   (* rMSG( "(%d) CHECK ETAG %s : %s", r, http->reply_content->filepath, http->reply_content->etag ); *) */
/*   return http;                                                                                          */
/* }                                                                                                       */

mas_http_t *
mas_http_make_out_header_simple( mas_http_t * http, const char *name, const char *value )
{
  http->outdata = mas_varvec_search_variable( http->outdata, NULL, name, value );
  return http;
}

mas_http_t *
mas_http_make_out_header( mas_http_t * http, const char *name, const char *fmt, ... )
{
  va_list args;

  va_start( args, fmt );
  http->outdata = mas_varvec_search_variable_va( http->outdata, NULL, name, NULL, fmt, args );
  va_end( args );
  return http;
}

#if defined( MAS_HTTP_USE_AUTOOBJECT )
mas_varvec_t *
mas_http_make_data_headers( mas_varvec_t * outdata, mas_autoobject_t * ao )
{
  outdata = mas_varvec_search_variablef( outdata, NULL, "Content-Length", NULL, "%d",
                                         mas_autoobject_is_regular( ao ) ? mas_autoobject_size( ao ) : 0 );
  {
    char *content_type;

    content_type = mas_autoobject_content_type_string( ao );
    if ( content_type )
    {
      outdata = mas_varvec_search_variable( outdata, NULL, "Content-Type", content_type );
      mas_free( content_type );
    }
  }
  outdata = mas_varvec_search_variable( outdata, NULL, "ETag", mas_autoobject_etag( ao ) );
  /* outdata =                                                                                                      */
  /*       mas_varvec_search_variablef( outdata, NULL, "Last-Modified", mas_xvstrftime_time, "%a, %d %b %Y %T GMT", */
  /*                                    mas_autoobject_time( ao ) );                                                */
  outdata = mas_varvec_search_variable( outdata, NULL, "Last-Modified", mas_autoobject_gtime( ao ) );
  return outdata;
}
#endif

mas_http_t *
mas_http_make_out_std_headers( mas_rcontrol_t * prcontrol, mas_http_t * http )
{
  /* extern unsigned long __MAS_LINK_DATE__; */
  extern unsigned long __MAS_LINK_TIME__;

  MAS_LOG( "to make http version/status header (keepalive:%d;close:%d)", http->connection_keep_alive, http->connection_close );
  http->outdata = mas_varvec_set_headf( http->outdata, "header", NULL, "HTTP/1.1 %d %s", http->status_code,
                                        mas_http_status_code_message( prcontrol, http ) );

  http = mas_http_make_out_header_simple( http, "Accept-Ranges", "bytes" );
  MAS_LOG( "to make misc headers" );

  /* extern unsigned long __MAS_LINK_TIMESTAMP__; */
  HMSG( "HTTP make OUT STD HEADERS" );

  if ( http )
  {
    MAS_LOG( "to make date" );

    http->outdata = mas_varvec_search_variablef( http->outdata, NULL, "Date", mas_xvstrftime, "%a, %d %b %Y %T GMT", mas_xgmtime(  ) );
    MAS_LOG( "to make server" );
    http->outdata =
          mas_varvec_search_variablef( http->outdata, NULL, "Server", mas_xvsnprintf, "mas-%lu", ( unsigned long ) ( &__MAS_LINK_TIME__ ) );
    http->outdata = mas_varvec_search_variablef( http->outdata, NULL, "Mas-Version", mas_xvsnprintf, "%3.1f", http->fversion );
    /* {                                                                                                                */
    /*   char buf[128];                                                                                                 */
    /*                                                                                                                  */
    /*   mas_tstrftime( buf, sizeof( buf ), "%Y%m%d %T", ctrl.stamp.first_lts );                                        */
    /*   http->outdata = mas_varvec_search_variablef( http->outdata, NULL, "Mas-Launched", mas_xvsnprintf, "%s", buf ); */
    /* }                                                                                                                */
    MAS_LOG( "to make data headers" );
#ifdef MAS_HTTP_USE_FILEINFO
    http->outdata = mas_fileinfo_make_headers( http->outdata, http->reply_content );
#elif defined( MAS_HTTP_USE_AUTOOBJECT )
    http->outdata = mas_http_make_data_headers( http->outdata, http->reply_content );
#endif
    MAS_LOG( "to make connection headers" );
    if ( http->connection_keep_alive )
    {
      http = mas_http_make_out_header_simple( http, "Connection", "Keep-Alive" );
      /* http = mas_http_make_out_header_simple( http, "Keep-Alive", "timeout=15, max=100" ); */
    }
    else if ( http->connection_close )
      http = mas_http_make_out_header_simple( http, "Connection", "close" );
    MAS_LOG( "to make headers tail" );
    http->outdata = mas_varvec_add_tail( http->outdata, "header", "" );
    MAS_LOG( "made headers" );
  }
  return http;
}

/* mas_http_t *                                                                                                           */
/* mas_http_make_body_simple( mas_rcontrol_t * prcontrol, mas_http_t * http )                                             */
/* {                                                                                                                      */
/*   if ( http )                                                                                                          */
/*     http->outdata = mas_fileinfo_make_body( http->outdata, http->reply_content );                                            */
/*   return http;                                                                                                         */
/* }                                                                                                                      */
/*                                                                                                                        */
/* mas_http_t *                                                                                                           */
/* mas_http_make_body( mas_rcontrol_t * prcontrol, mas_http_t * http, const char *fmt, ... )                              */
/* {                                                                                                                      */
/*   va_list args;                                                                                                        */
/*                                                                                                                        */
/*   va_start( args, fmt );                                                                                               */
/*   http->outdata = mas_variable_vcreate_x( http->outdata, MAS_THREAD_TRANSACTION, "body", "body", NULL, fmt, args, 0 ); */
/*   va_end( args );                                                                                                      */
/*   return http;                                                                                                         */
/* }                                                                                                                      */

const char *
mas_http_status_code_message( mas_rcontrol_t * prcontrol, mas_http_t * http )
{
  const char *s = NULL;

  switch ( http->status_code )
  {
  case MAS_HTTP_CODE_OK:
    s = "OK";
    break;
  case MAS_HTTP_CODE_NOT_FOUND:
    s = "Not Found";
    break;
  case MAS_HTTP_CODE_NOT_IMPLEMENTED:
    s = "Method Not Implemented";
    break;
  default:
    s = "?CODE?";
    break;
  }
  return s;
}

mas_http_t *
mas_http_reply_test( mas_rcontrol_t * prcontrol, mas_http_t * http )
{
#if defined( MAS_HTTP_USE_FILEINFO ) || defined( MAS_HTTP_USE_AUTOOBJECT )
  const char *text = "HTTP/1.1 200 OK\n"
        "Accept-Ranges: bytes\n"
        "Date: Wed, 05 Jun 2013 01:01:01 GMT\n"
        "Server: mas-1251337\n"
        "Content-Length: 358\n"
        "Content-Type: text/html\n"
        "ETag: \"2c47af-166-4cb058c5ca740\"\n" "Last-Modified: Mon, 01 Oct 2012 20:49:57 GMT\n" "Connection: Keep-Alive\n" "\n";
#endif

#ifdef MAS_HTTP_USE_FILEINFO
  const mas_evaluated_t data = {.data = ( char * ) text };
  http = mas_proto_http_write( http, data.data, strlen( data.data ) + 1 );
#elif defined( MAS_HTTP_USE_AUTOOBJECT )
  mas_autoobject_set_iaccess_type( http->reply_content, MAS_IACCESS_CHAR );
  mas_autoobject_set_data( http->reply_content, text );
  mas_autoobject_cat( mas_channel_fd( http->prcontrol->h.pchannel ), http->reply_content, 0 );
#endif
  return http;
}

mas_http_t *
mas_http_reply( mas_rcontrol_t * prcontrol, mas_http_t * http )
{
  HMSG( "HTTP REPLY" );

  if ( prcontrol && prcontrol->plcontrol )
  {
#ifdef MAS_HTTP_USE_FILEINFO
    mas_evaluated_t *data = NULL;

    if ( http )
      data = mas_fileinfo_data( prcontrol->plcontrol->popts, http->reply_content );
#elif defined( MAS_HTTP_USE_AUTOOBJECT )
#endif
    prcontrol->h.substatus = MAS_SUBSTATUS_MAKEA;
    prcontrol->h.subpoint = __LINE__;

/* moved to fileinfo */
    /* if ( http )                                     */
    /*   http = mas_http_make_etag( prcontrol, http ); */


    /* if ( data ) */
    {
      if ( http && http->status_code == MAS_HTTP_CODE_NONE )
      {
        ssize_t test_size = -1;

        MAS_LOG( "to make http status" );
#ifdef MAS_HTTP_USE_FILEINFO
        if ( http->reply_content && ( test_size = mas_unidata_data_size( http->reply_content->udata ) ) )
#elif defined( MAS_HTTP_USE_AUTOOBJECT )
        if ( http->reply_content && mas_autoobject_is_regular( http->reply_content )
             && ( test_size = mas_autoobject_size( http->reply_content ) ) > 0 )
#else
        if ( 0 )
#endif
          http->status_code = MAS_HTTP_CODE_OK;
        else
          http->status_code = MAS_HTTP_CODE_NOT_FOUND;
        prcontrol->h.subpoint = __LINE__;
      }
      if ( http )
      {
        HMSG( "HTTP REPLY status %d", http->status_code );
      }
      HMSG( "HTTP HTTP" );
      MAS_LOG( "to make std headers" );
      prcontrol->h.subpoint = __LINE__;
      if ( http )
        http = mas_http_make_out_std_headers( prcontrol, http );
      prcontrol->h.subpoint = __LINE__;
      /* CORK */
      /* mas_channel_cork( http->prcontrol->h.pchannel, 1 ); */
      if ( http )
      {
        prcontrol->h.subpoint = __LINE__;
        MAS_LOG( "to write header" );
        http = mas_proto_http_write_pairs( http, "header" );
        http->written_header = http->written;
        MAS_LOG( "written %lu", http ? http->written : 0 );
      }
      prcontrol->h.subpoint = __LINE__;
      if ( http && http->imethod == MAS_HTTP_METHOD_GET )
      {
        size_t datasz;

        prcontrol->h.subpoint = __LINE__;
#ifdef MAS_HTTP_USE_FILEINFO
        datasz = mas_fileinfo_data_size( http->reply_content );
#elif defined( MAS_HTTP_USE_AUTOOBJECT )
        datasz = mas_autoobject_is_regular( http->reply_content ) ? mas_autoobject_size( http->reply_content ) : 0;
#else
        datasz = 0;
#endif
        HMSG( "HTTP write DATA (%lu)", ( unsigned long ) datasz );


        /* http = mas_proto_http_write_values( http, "body" ); */
        /* mas_transaction_write( prcontrol, _mas_fileinfo_data( fileinfo ), mas_fileinfo_data_size( fileinfo ) ); */

        prcontrol->h.subpoint = __LINE__;
        http->written_body = 0;
#ifdef MAS_HTTP_USE_FILEINFO
        http = mas_proto_http_write( http, data->data, datasz );
#elif defined( MAS_HTTP_USE_AUTOOBJECT )
        if ( datasz > 0 )
          http->written_body = mas_autoobject_cat( mas_channel_fd( http->prcontrol->h.pchannel ), http->reply_content, 0 );
        if ( http->written_body >= 0 )
          http->written += http->written_body;
#else
#endif
        prcontrol->h.subpoint = __LINE__;

/* ????????? */
        /* pthread_yield(  ); */
/* ????????? */

        MAS_LOG( "written %lu of %lu", http ? http->written : 0, ( unsigned long ) datasz );
        HMSG( "HTTP written DATA (%lu)", ( unsigned long ) http ? http->written : 0 );
      }
      /* mas_channel_cork( http->prcontrol->h.pchannel, 0 ); */
      /* unCORK */
    }
  }
  prcontrol->h.subpoint = __LINE__;
  /* to close connection */
  return http;
}

mas_http_t *
mas_http_make_docroot( mas_rcontrol_t * prcontrol, mas_http_t * http )
{
  HMSG( "HTTP make DOCROOT [%s]", http->docroot );
  if ( !http->docroot )
  {
    mas_varvec_element_t *host_var;

    host_var = mas_varvec_find_variable( http->indata, "Host" );
    HMSG( "host_var %s", host_var ? "present" : "absent" );
    if ( host_var )
    {
      char *p;

      http->host = mas_varvec_variable_get_value( host_var );
      HMSG( "Host: %s", http->host );
      p = strchr( http->host, ':' );
      if ( p )
      {
        *p++ = 0;
        sscanf( p, "%u", &http->port );
      }
      else
      {
        http->port = prcontrol->h.pchannel->port;
      }
    }
    else
    {
      EMSG( "NO 'Host'" );
    }
    if ( !http->docroot && http->host )
    {
      mas_varvec_element_t *tv;

      tv = mas_varset_find_variable( prcontrol->proto_desc->variables, "docroot", http->host );
      if ( tv )
        http->docroot = mas_varvec_variable_get_value( tv );
    }
    if ( !http->docroot )
      http->docroot = mas_strdup( "/var/www/mastarink.net/mastarink.net/1/htdocs" );
  }
  if ( http->docroot )
  {
    HMSG( "HTTP made DOCROOT [%s]", http->docroot );
    MAS_LOG( "DOCROOT: %s", http->docroot );
  }
  else
  {
    EMSG( "NO DOCROOT" );
    MAS_LOG( "NO DOCROOT" );
  }
  return http;
}

mas_http_t *
mas_http_make_data_auto( mas_rcontrol_t * prcontrol, mas_http_t * http )
{
  const char *fmt = NULL;

  switch ( http->imethod )
  {
  case MAS_HTTP_METHOD_UNKNOWN:
    MAS_LOG( "to make out : FMT" );
    fmt = "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n"
          "<html><head>\n"
          "<title>%u %s</title>\n"
          "</head><body>\n"
          "<h1>%s</h1>\n"
          "<p>%s to %s not supported.<br />\n" "</p>\n" "<hr>\n" "<address>Server at %s Port %u</address>\n" "</body></html>\n";
    break;
  default:
    break;
  }
  if ( fmt )
  {
    const char *sm;

    sm = mas_http_status_code_message( prcontrol, http );
    {
      char *text;
      size_t txsize = 1024 * 10;

      text = mas_malloc( txsize );
      /* snprintf( text, txsize, fmt, http->status_code, sm, sm, mas_proto_http_method_name( http ), http->URI ); */
      snprintf( text, txsize, fmt, http->status_code, sm, sm, http->smethod, http->URI, http->host, http->port );

      /* set data from text as content */
#ifdef MAS_HTTP_USE_FILEINFO
      if ( http->reply_content )
      {
        mas_free( http->reply_content->etag );
        http->reply_content->etag = NULL;
        http->reply_content->filetime = 0;
        /* http->reply_content->icontent_type = MAS_CONTENT_NONE; */
      }
      _mas_fileinfo_link_dataz( prcontrol
                                && prcontrol->plcontrol ? prcontrol->plcontrol->popts : NULL, http->reply_content,
                                mas_evaluated_wrap_pchar( text ) );
#elif defined( MAS_HTTP_USE_AUTOOBJECT )
      mas_autoobject_set_iaccess_type( http->reply_content, MAS_IACCESS_CHAR );
      mas_autoobject_set_data( http->reply_content, text );
#endif
      /* mas_free( text ); */
    }
    /* http = mas_http_make_body( prcontrol, http, fmt, http->status_code, sm, sm, mas_proto_http_method_name( http ), http->URI ); */
  }
  return http;
}
