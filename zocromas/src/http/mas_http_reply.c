#include "mas_server_def.h"
#include "mas_basic_def.h"

#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>
/* mas_xvstrftime */
#include <mastar/tools/mas_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

/* #include "mas_common.h" */
#include <mastar/log/mas_log.h>

#include <mastar/variables/mas_variables.h>

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
  int r = -1;

  if ( http )
    r = mas_fileinfo_make_etag( http->content );
  /* rMSG( "(%d) CHECK ETAG %s : %s", r, http->content->filepath, http->content->etag ); */
  return http;
}

mas_http_t *
mas_http_make_out_header_simple( mas_http_t * http, const char *name, const char *value )
{
  http->outdata = mas_variable_create_text( http->outdata, MAS_THREAD_TRANSACTION, "header", name, value, 0 );
  return http;
}

mas_http_t *
mas_http_make_out_header( mas_http_t * http, const char *name, const char *fmt, ... )
{
  va_list args;

  va_start( args, fmt );
  http->outdata = mas_variable_vcreate_x( http->outdata, MAS_THREAD_TRANSACTION, "header", name, NULL, fmt, args, 0 );
  va_end( args );
  return http;
}

mas_http_t *
mas_http_make_out_std_headers( mas_rcontrol_t * prcontrol, mas_http_t * http )
{
  if ( http )
  {
    MAS_LOG( "to make date" );
    http->outdata =
          mas_variable_create_x( http->outdata, MAS_THREAD_TRANSACTION, "header", "Date", mas_xvstrftime, "%a, %d %b %Y %T GMT",
                                 mas_xgmtime(  ), 0 );
    http->outdata =
          mas_variable_create_x( http->outdata, MAS_THREAD_TRANSACTION, "header", "Server", mas_xvsnprintf, "mas-%lu",
                                 ( unsigned long ) ctrl.stamp.vtime, 0 );
    http->outdata = mas_fileinfo_make_headers( http->outdata, http->content );
    http = mas_http_make_out_header_simple( http, "Connection", prcontrol->keep_alive ? "Keep-Alive" : "close" );
  }
  return http;
}

/* mas_http_t *                                                                                                           */
/* mas_http_make_body_simple( mas_rcontrol_t * prcontrol, mas_http_t * http )                                             */
/* {                                                                                                                      */
/*   if ( http )                                                                                                          */
/*     http->outdata = mas_fileinfo_make_body( http->outdata, http->content );                                            */
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

  MAS_LOG( "to write protocol name/version" );
  data = mas_fileinfo_data( http->content );

  if ( http && http->status_code == MAS_HTTP_CODE_NONE )
  {
    if ( http->content && mas_unidata_data_size( http->content->udata ) )
      http->status_code = MAS_HTTP_CODE_OK;
    else
      http->status_code = MAS_HTTP_CODE_NOT_FOUND;
  }
  http = mas_proto_http_writef( http, "HTTP/1.1 %d %s\r\n", http->status_code, mas_http_status_code_message( prcontrol, http ) );
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

    datasz = mas_fileinfo_data_size( http->content );


    MAS_LOG( "to write body %lu [%s]", datasz, datasz < 100 ? data : "..." );
    /* http = mas_proto_http_write_values( http, "body" ); */
    /* mas_transaction_write( prcontrol, _mas_fileinfo_data( fileinfo ), mas_fileinfo_data_size( fileinfo ) ); */

    http = mas_proto_http_write( http, data, datasz );
    MAS_LOG( "written %lu of %lu", http ? http->written : 0, datasz );
  }
  /* to close connection */
  return http;
}

mas_http_t *
mas_http_make_docroot( mas_rcontrol_t * prcontrol, mas_http_t * http )
{
  if ( !http->docroot )
  {
    mas_variable_t *host_var;

    host_var = mas_variables_find( http->indata, "inheader", "Host" );
    if ( host_var )
    {
      char *p;

      http->host = mas_strdup( host_var->value );
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
    }
    mas_variable_t *docroot_var = NULL;

    if ( !docroot_var && http->host )
      docroot_var = mas_variables_find( ctrl.hostvars, http->host, "docroot" );
    if ( !docroot_var )
      docroot_var = mas_variables_find( ctrl.hostvars, "zocromas.mastar.lan", "docroot" );
    if ( !docroot_var )
      docroot_var = mas_variables_find( ctrl.hostvars, "zocromas.mastar.lan:5005", "docroot" );
    if ( docroot_var && docroot_var->value )
      http->docroot = mas_strdup( docroot_var->value );
    else
      http->docroot = mas_strdup( "/var/www/mastarink.net/mastarink.net/htdocs" );
  }
  if ( http->docroot )
  {
    MAS_LOG( "DOCROOT: %s", http->docroot );
  }
  else
  {
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
      if ( http->content )
      {
        mas_free( http->content->etag );
        http->content->etag = NULL;
        http->content->filetime = 0;
        /* http->content->icontent_type = MAS_CONTENT_NONE; */
      }
      _mas_fileinfo_link_dataz( http->content, text );
      /* mas_free( text ); */
    }
    /* http = mas_http_make_body( prcontrol, http, fmt, http->status_code, sm, sm, mas_proto_http_method_name( http ), http->URI ); */
  }
  return http;
}
