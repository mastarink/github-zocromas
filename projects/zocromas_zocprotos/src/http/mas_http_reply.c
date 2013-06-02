#include <mastar/wrap/mas_std_def.h>

#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>
/* mas_xvstrftime */
#include <mastar/tools/mas_tools.h>

/* #include <mastar/types/mas_control_types.h> */
/* extern mas_control_t ctrl; */
/* #include <mastar/types/mas_opts_types.h> */
/* extern mas_options_t opts; */

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/log/mas_log.h>

#ifdef MAS_OLD_VARIABLES_HTTP
#  include <mastar/variables/mas_variables.h>
#else
#  include <mastar/types/mas_varset_types.h>
#  include <mastar/varset/mas_varset_vclass.h>
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

mas_http_t *
mas_http_make_etag( mas_rcontrol_t * prcontrol, mas_http_t * http )
{
  HMSG( "HTTP make ETAG" );
  if ( http )
    ( void ) /* r = */ mas_fileinfo_make_etag( http->reply_content );
  /* rMSG( "(%d) CHECK ETAG %s : %s", r, http->reply_content->filepath, http->reply_content->etag ); */
  return http;
}

mas_http_t *
mas_http_make_out_header_simple( mas_http_t * http, const char *name, const char *value )
{
#ifdef MAS_OLD_VARIABLES_HTTP
  http->outdata = mas_variable_create_text( http->outdata, /* MAS_THREAD_TRANSACTION, */ "header", name, value, 0 );
#elif defined(MAS_VARSET_VARIABLES_HTTP)
  http->outdata = mas_varset_search_variable( http->outdata, "header", name, value );
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
#else
    http->outdata =
          mas_varset_vclass_search_variablef( http->outdata, NULL, "Date", mas_xvstrftime, "%a, %d %b %Y %T GMT", mas_xgmtime(  ) );
    http->outdata =
          mas_varset_vclass_search_variablef( http->outdata, NULL, "Server", mas_xvsnprintf, "mas-%lu",
                                              ( unsigned long ) ( &__MAS_LINK_TIME__ ) );
#endif
    http->outdata = mas_fileinfo_make_headers( http->outdata, http->reply_content );
    http = mas_http_make_out_header_simple( http, "Connection", prcontrol->keep_alive ? "Keep-Alive" : "close" );
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
mas_http_reply( mas_rcontrol_t * prcontrol, mas_http_t * http )
{
  char *data;

  HMSG( "HTTP REPLY" );
  MAS_LOG( "to write protocol name/version" );
  data = mas_fileinfo_data( http->reply_content );

  if ( http && http->status_code == MAS_HTTP_CODE_NONE )
  {
    if ( http->reply_content && mas_unidata_data_size( http->reply_content->udata ) )
      http->status_code = MAS_HTTP_CODE_OK;
    else
      http->status_code = MAS_HTTP_CODE_NOT_FOUND;
  }
  HMSG( "HTTP REPLY status %d", http->status_code );
  HMSG( "HTTP HTTP" );
  http = mas_proto_http_writef( http, "HTTP/1.1 %d %s\r\n", http->status_code, mas_http_status_code_message( prcontrol, http ) );
  MAS_LOG( "to make ... headers" );
  if ( http )
    http = mas_http_make_out_std_headers( prcontrol, http );
  if ( http )
  {
    MAS_LOG( "to write header" );
    http = mas_proto_http_write_pairs( http, "header" );
    MAS_LOG( "written %lu", http ? http->written : 0 );
  }
  if ( http /* && http->imethod == MAS_HTTP_METHOD_GET */  )
  {
    size_t datasz;

    datasz = mas_fileinfo_data_size( http->reply_content );
    HMSG( "HTTP write DATA (%lu)", ( unsigned long ) datasz );


    MAS_LOG( "to write body %lu [%s]", ( unsigned long ) datasz, datasz < 100 ? data : "..." );
    /* http = mas_proto_http_write_values( http, "body" ); */
    /* mas_transaction_write( prcontrol, _mas_fileinfo_data( fileinfo ), mas_fileinfo_data_size( fileinfo ) ); */

    http = mas_proto_http_write( http, data, datasz );
    MAS_LOG( "written %lu of %lu", http ? http->written : 0, ( unsigned long ) datasz );
    HMSG( "HTTP written DATA (%lu)", ( unsigned long ) http ? http->written : 0 );
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
      _mas_fileinfo_link_dataz( http->reply_content, text );
      /* mas_free( text ); */
    }
    /* http = mas_http_make_body( prcontrol, http, fmt, http->status_code, sm, sm, mas_proto_http_method_name( http ), http->URI ); */
  }
  return http;
}
