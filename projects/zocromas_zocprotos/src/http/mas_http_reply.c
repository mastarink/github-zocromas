#define MAS_USE_VARVEC

#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>
/* mas_xvstrftime */
#include <mastar/tools/mas_tools.h>

/* #include <mastar/types/mas_control_types.h> */
/* extern mas_control_t ctrl; */
/* #include <mastar/types/mas_opts_types.h> */

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/log/mas_log.h>

#ifdef MAS_OLD_VARIABLES_HTTP
#  include <mastar/variables/mas_variables.h>
#elif defined(MAS_USE_VARVEC)
#  include <mastar/types/mas_varvec_types.h>
#  include <mastar/varvec/mas_varvec.h>
#  include <mastar/varvec/mas_varvec_search.h>
#  include <mastar/varvec/mas_varvec_search.h>
#  include <mastar/varvec/mas_varvec_headtail.h>
#  include <mastar/varvec/mas_varvec_namevalue.h>
#  include <mastar/varset/mas_varset_search.h>
#  include <mastar/varset/mas_varset.h>
#else
#  include <mastar/types/mas_varset_types.h>
#  include <mastar/varset/mas_varset_vclass.h>
#  include <mastar/varset/mas_varset_vclass_search.h>
#  include <mastar/varset/mas_varset_vclass_headtail.h>
#  include <mastar/varset/mas_varset_vclass_namevalue.h>
#  include <mastar/varset/mas_varset_search.h>
#  include <mastar/varset/mas_varset.h>
#endif

#include <mastar/fileinfo/mas_unidata.h>
#include <mastar/fileinfo/mas_fileinfo.h>
#include <mastar/fileinfo/mas_fileinfo_object.h>

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
#ifdef MAS_OLD_VARIABLES_HTTP
  http->outdata = mas_variable_create_text( http->outdata, /* MAS_THREAD_TRANSACTION, */ "header", name, value, 0 );
#elif defined(MAS_VARSET_VARIABLES_HTTP)
  http->outdata = mas_varset_search_variable( http->outdata, "header", name, value );
#elif defined(MAS_USE_VARVEC)
  http->outdata = mas_varvec_search_variable( http->outdata, NULL, name, value );
#else
  http->outdata = mas_varset_vclass_search_variable( http->outdata, NULL, name, value );
#endif
  return http;
}

mas_http_t *
mas_http_make_out_header( mas_http_t * http, const char *name, const char *fmt, ... )
{
  va_list args;

  va_start( args, fmt );
#ifdef MAS_OLD_VARIABLES_HTTP
  http->outdata = mas_variable_vcreate_x( http->outdata, /* MAS_THREAD_TRANSACTION, */ "header", name, NULL, fmt, args, 0 );
#elif defined(MAS_VARSET_VARIABLES_HTTP)
  http->outdata = mas_varset_search_variable_va( http->outdata, "header", name, NULL, fmt, args );
#elif defined(MAS_USE_VARVEC)
  http->outdata = mas_varvec_search_variable_va( http->outdata, NULL, name, NULL, fmt, args );
#else
  http->outdata = mas_varset_vclass_search_variable_va( http->outdata, NULL, name, NULL, fmt, args );
#endif
  va_end( args );
  return http;
}

mas_http_t *
mas_http_make_out_std_headers( mas_rcontrol_t * prcontrol, mas_http_t * http )
{
  /* extern unsigned long __MAS_LINK_DATE__; */
  extern unsigned long __MAS_LINK_TIME__;

#ifdef MAS_OLD_VARIABLES_HTTP
  http = mas_proto_http_writef( http, "HTTP/1.1 %d %s\r\n", http->status_code, mas_http_status_code_message( prcontrol, http ) );
#elif defined(MAS_VARSET_VARIABLES_HTTP)
  http = mas_proto_http_writef( http, "HTTP/1.1 %d %s\r\n", http->status_code, mas_http_status_code_message( prcontrol, http ) );
#elif defined(MAS_USE_VARVEC)
  http->outdata = mas_varvec_set_headf( http->outdata, "header", NULL, "HTTP/1.1 %d %s", http->status_code,
                                        mas_http_status_code_message( prcontrol, http ) );
#else
  /* http = mas_proto_http_writef( http, "HTTP/1.1 %d %s\r\n", http->status_code, mas_http_status_code_message( prcontrol, http ) ); */
  http->outdata = mas_varset_vclass_set_headf( http->outdata, "header", NULL, "HTTP/1.1 %d %s", http->status_code,
                                               mas_http_status_code_message( prcontrol, http ) );
  /* mas_varset_vclass_write( STDERR_FILENO, http->outdata ); */
#endif

  http = mas_http_make_out_header_simple( http, "Accept-Ranges", "bytes" );
  MAS_LOG( "to make std headers" );

  /* extern unsigned long __MAS_LINK_TIMESTAMP__; */
  HMSG( "HTTP make OUT STD HEADERS" );

  if ( http )
  {
    MAS_LOG( "to make date" );
#ifdef MAS_OLD_VARIABLES_HTTP
    http->outdata =
          mas_variable_create_x( http->outdata, /* MAS_THREAD_TRANSACTION, */ "header", "Date", mas_xvstrftime, "%a, %d %b %Y %T GMT",
                                 mas_xgmtime(  ), 0 );
    http->outdata = mas_variable_create_x( http->outdata, /* MAS_THREAD_TRANSACTION, */ "header", "Server", mas_xvsnprintf, "mas-%lu",
                                           ( unsigned long ) ( &__MAS_LINK_TIME__ ), 0 );
#elif defined(MAS_VARSET_VARIABLES_HTTP)
    http->outdata = mas_varset_search_variablef( http->outdata, "header", "Date", mas_xvstrftime, "%a, %d %b %Y %T GMT", mas_xgmtime(  ) );
    /* TODO http->indata and http->outdata etc should be vclass, not varset */
    /* TODO instead if prev line : mas_varset_vclass_search_variable + mas_varset_vclass_variable_set_valuef */

    http->outdata =
          mas_varset_search_variablef( http->outdata, "header", "Server", mas_xvsnprintf, "mas-%lu",
                                       ( unsigned long ) ( &__MAS_LINK_TIME__ ) );
#elif defined(MAS_USE_VARVEC)
    http->outdata = mas_varvec_search_variablef( http->outdata, NULL, "Date", mas_xvstrftime, "%a, %d %b %Y %T GMT", mas_xgmtime(  ) );
    http->outdata =
          mas_varvec_search_variablef( http->outdata, NULL, "Server", mas_xvsnprintf, "mas-%lu", ( unsigned long ) ( &__MAS_LINK_TIME__ ) );
    http->outdata = mas_varvec_search_variablef( http->outdata, NULL, "Mas-Version", mas_xvsnprintf, "%3.1f", http->fversion );
    {
      char buf[128];

      mas_tstrftime( buf, sizeof( buf ), "%Y%m%d %T", ctrl.stamp.first_lts );
      http->outdata = mas_varvec_search_variablef( http->outdata, NULL, "Mas-Launched", mas_xvsnprintf, "%s", buf );
    }
#else
    {
      struct tm *t;
      char buf[512];

      t = mas_xgmtime(  );
      strftime( buf, sizeof( buf ), "%a, %d %b %Y %T GMT", t );
      HMSG( "Time: %s\n", buf );
    }

    http->outdata =
          mas_varset_vclass_search_variablef( http->outdata, NULL, "Date", mas_xvstrftime, "%a, %d %b %Y %T GMT", mas_xgmtime(  ) );
    http->outdata =
          mas_varset_vclass_search_variablef( http->outdata, NULL, "Server", mas_xvsnprintf, "mas-%lu",
                                              ( unsigned long ) ( &__MAS_LINK_TIME__ ) );
    http->outdata = mas_varset_vclass_search_variablef( http->outdata, NULL, "MasV", mas_xvsnprintf, "%3.1f", http->fversion );
#endif
    http->outdata = mas_fileinfo_make_headers( http->outdata, http->reply_content );
    if ( http->connection_keep_alive )
    {
      http = mas_http_make_out_header_simple( http, "Connection", "Keep-Alive" );
      http = mas_http_make_out_header_simple( http, "Keep-Alive", "timeout=15, max=100" );
    }
    else if ( http->connection_close )
      http = mas_http_make_out_header_simple( http, "Connection", "close" );
#if defined(MAS_USE_VARVEC)
    http->outdata = mas_varvec_add_tail( http->outdata, "header", "" );
#elif defined(MAS_VARSET_VARIABLES_HTTP)
    http->outdata = mas_varset_vclass_add_tail( http->outdata, "header", "" );
#endif
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
  const mas_evaluated_t data = {.data = "HTTP/1.1 200 OK\n"
          "Accept-Ranges: bytes\n"
          "Date: Wed, 05 Jun 2013 01:01:01 GMT\n"
          "Server: mas-1251337\n"
          "Content-Length: 358\n"
          "Content-Type: text/html\n"
          "ETag: \"2c47af-166-4cb058c5ca740\"\n" "Last-Modified: Mon, 01 Oct 2012 20:49:57 GMT\n" "Connection: Keep-Alive\n" "\n"
  };
  /* const char data[] =                                                                                                            */
  /*       "HTTP/1.1 200 OK\n"                                                                                                      */
  /*       "Accept-Ranges: bytes\n"                                                                                                 */
  /*       "Date: Wed, 05 Jun 2013 01:01:01 GMT\n"                                                                                  */
  /*       "Server: mas-1251337\n"                                                                                                  */
  /*       "Content-Length: 358\n"                                                                                                  */
  /*       "Content-Type: text/html\n"                                                                                              */
  /*       "ETag: \"2c47af-166-4cb058c5ca740\"\n" "Last-Modified: Mon, 01 Oct 2012 20:49:57 GMT\n" "Connection: Keep-Alive\n" "\n"; */
  /* size_t datasz = sizeof( data ); */

  http = mas_proto_http_write( http, data.data, strlen( data.data ) + 1 );
  return http;
}

mas_http_t *
mas_http_reply( mas_rcontrol_t * prcontrol, mas_http_t * http )
{
  mas_evaluated_t *data;

  HMSG( "HTTP REPLY" );
  MAS_LOG( "to write protocol name/version" );

  if ( prcontrol && prcontrol->plcontrol )
  {
    if ( http )
      data = mas_fileinfo_data( prcontrol->plcontrol->popts, http->reply_content );

/* moved to fileinfo */
    /* if ( http )                                     */
    /*   http = mas_http_make_etag( prcontrol, http ); */

    if ( http && http->status_code == MAS_HTTP_CODE_NONE )
    {
      if ( http->reply_content && mas_unidata_data_size( http->reply_content->udata ) )
        http->status_code = MAS_HTTP_CODE_OK;
      else
        http->status_code = MAS_HTTP_CODE_NOT_FOUND;
    }
    if ( http )
    {
      HMSG( "HTTP REPLY status %d", http->status_code );
    }
    HMSG( "HTTP HTTP" );
    if ( http )
      http = mas_http_make_out_std_headers( prcontrol, http );
    if ( http )
    {
      MAS_LOG( "to write header" );
      http = mas_proto_http_write_pairs( http, "header" );
      MAS_LOG( "written %lu", http ? http->written : 0 );
    }
    if ( http && http->imethod == MAS_HTTP_METHOD_GET )
    {
      size_t datasz;

      datasz = mas_fileinfo_data_size( http->reply_content );
      HMSG( "HTTP write DATA (%lu)", ( unsigned long ) datasz );


      MAS_LOG( "to write body %lu [%s]", ( unsigned long ) datasz, datasz < 100 ? ( char * ) data->data : "..." );
      /* http = mas_proto_http_write_values( http, "body" ); */
      /* mas_transaction_write( prcontrol, _mas_fileinfo_data( fileinfo ), mas_fileinfo_data_size( fileinfo ) ); */

      http = mas_proto_http_write( http, data->data, datasz );

/* ????????? */
      /* pthread_yield(  ); */
/* ????????? */

      MAS_LOG( "written %lu of %lu", http ? http->written : 0, ( unsigned long ) datasz );
      HMSG( "HTTP written DATA (%lu)", ( unsigned long ) http ? http->written : 0 );
    }
  }
  /* to close connection */
  return http;
}

mas_http_t *
mas_http_make_docroot( mas_rcontrol_t * prcontrol, mas_http_t * http )
{
  HMSG( "HTTP make DOCROOT [%s]", http->docroot );
  if ( !http->docroot )
  {
#ifdef MAS_OLD_VARIABLES_HTTP
    mas_variable_t *host_var;

    host_var = mas_variables_find( http->indata, "inheader", "Host" );
#elif defined(MAS_VARSET_VARIABLES_HTTP)
    mas_vclass_element_t *host_var;

    host_var = mas_varset_find_variable( http->indata, "inheader", "Host" );
#elif defined(MAS_USE_VARVEC)
    mas_varvec_element_t *host_var;

    host_var = mas_varvec_find_variable( http->indata, "Host" );
#else
    mas_vclass_element_t *host_var;

    host_var = mas_varset_vclass_find_variable( http->indata, "Host" );
#endif
    HMSG( "host_var %s", host_var ? "present" : "absent" );
    if ( host_var )
    {
      char *p;

#ifdef MAS_OLD_VARIABLES_HTTP
      http->host = mas_strdup( host_var->value );
#elif defined(MAS_VARSET_VARIABLES_HTTP)
      http->host = mas_varset_vclass_variable_get_value( host_var );
#elif defined(MAS_USE_VARVEC)
      http->host = mas_varvec_variable_get_value( host_var );
#else
      http->host = mas_varset_vclass_variable_get_value( host_var );
#endif
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
#ifdef MAS_OLD_VARIABLES_HTTP
      mas_variable_t *tv;

      tv = mas_variables_find( prcontrol->proto_desc->variables, "docroot", http->host );
      if ( tv && tv->value )
        http->docroot = mas_strdup( tv->value );
#elif defined(MAS_VARSET_VARIABLES_HTTP)
      mas_vclass_element_t *tv;

      tv = mas_varset_find_variable( prcontrol->proto_desc->variables, "docroot", http->host );
      if ( tv )
        http->docroot = mas_varset_vclass_variable_get_value( tv );
#elif defined(MAS_USE_VARVEC)
      mas_varvec_element_t *tv;

      tv = mas_varset_find_variable( prcontrol->proto_desc->variables, "docroot", http->host );
      if ( tv )
        http->docroot = mas_varvec_variable_get_value( tv );
#else
      mas_vclass_element_t *tv;

      tv = mas_varset_find_variable( prcontrol->proto_desc->variables, "docroot", http->host );
      if ( tv )
        http->docroot = mas_varset_vclass_variable_get_value( tv );
#endif
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
      /* mas_free( text ); */
    }
    /* http = mas_http_make_body( prcontrol, http, fmt, http->status_code, sm, sm, mas_proto_http_method_name( http ), http->URI ); */
  }
  return http;
}
