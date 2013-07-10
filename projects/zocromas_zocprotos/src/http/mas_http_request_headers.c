#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>

#include <mastar/types/mas_varset_types.h>
#include <mastar/types/mas_varvec_types.h>
#include <mastar/varvec/mas_varvec.h>
#include <mastar/varvec/mas_varvec_search.h>
#include <mastar/varset/mas_varset.h>

#include <mastar/fileinfo/mas_unidata.h>
#include <mastar/fileinfo/mas_fileinfo.h>
#include <mastar/fileinfo/mas_fileinfo_object.h>

#include <mastar/channel/mas_channel_buffer.h>

#include "mas_http_request_headers.h"



static mas_http_t *
mas_proto_http_parse_known_header( mas_rcontrol_t * prcontrol, mas_http_t * http, const char *name, const char *value )
{
  HMSG( "HTTP H.KNOWN %s", name );
  if ( 0 == strcmp( name, "Connection" ) )
  {
    HMSG( "HTTP (known) HEADER (parse) %s", name );
    MAS_LOG( "HTTP connection HEADER: %s:%s", name, value );
    if ( 0 == strncasecmp( value, "keep-alive,", 11 ) || 0 == strcasecmp( value, "keep-alive" ) )
    {
      rMSG( ">>>>>> HTTP HEADER: Connection: Keep-Alive" );
      MAS_LOG( "HTTP keepalive HEADER" );
      http->connection_keep_alive = 1;
    }
    else if ( 0 == strncasecmp( value, "close,", 6 ) || 0 == strcasecmp( value, "close" ) )
    {
      rMSG( ">>>>>> HTTP known HEADER: Connection: close" );
      MAS_LOG( "HTTP close HEADER" );
      http->connection_close = 1;
    }
    else
    {
    }
    MAS_LOG( "KA : %u (%u:%u)", prcontrol->connection_keep_alive, http->connection_keep_alive, http->connection_close );
  }
  else if ( 0 == strcmp( name, "Content-Length" ) )
  {
#ifdef MAS_HTTP_USE_FILEINFO
    if ( !http->request_content )
      http->request_content = mas_fileinfo_create(  );
    mas_fileinfo_set_content_size( http->request_content, value );
#elif defined( MAS_HTTP_USE_AUTOOBJECT )
#endif
  }
  else if ( 0 == strcmp( name, "Content-Type" ) )
  {
#ifdef MAS_HTTP_USE_FILEINFO
    if ( !http->request_content )
      http->request_content = mas_fileinfo_create(  );
#elif defined( MAS_HTTP_USE_AUTOOBJECT )
#endif
    {
      mas_content_type_t ict = -1;
      const char *p;

      p = strchr( value, ';' );
      if ( p )
      {
        {
          char *cct;

          cct = mas_strndup( value, p - value );
          ict = mas_unidata_parse_content_type( cct );
          mas_free( cct );
        }
#ifdef MAS_HTTP_USE_FILEINFO
        mas_fileinfo_set_icontent_type( http->request_content, ict );
#elif defined( MAS_HTTP_USE_AUTOOBJECT )
#endif
        /* {                                                                    */
        /*   char *ctest;                                                       */
        /*                                                                      */
        /*   ctest = mas_fileinfo_content_type_string( http->request_content ); */
        /*   HMSG( "CT TEST [%d] %s", ict, ctest );                             */
        /*   mas_free( ctest );                                                 */
        /* }                                                                    */
        p++;
        while ( p && *p && *p <= ' ' )
          p++;
        if ( ict == MAS_CONTENT_FORM_DATA )
        {
          const char *bouname = "boundary=";
          size_t boulen;

          boulen = strlen( bouname );
          if ( 0 == strncmp( bouname, p, boulen ) )
          {
            const char *bou;

            p += boulen;
            bou = p;
            while ( p && *p && *p != ';' )
              p++;
            if ( *bou == '"' && p > bou + 1 && *( p - 1 ) == '"' )
              http->boundary = mas_strndup( bou + 1, p - bou - 2 );
            else
              http->boundary = mas_strndup( bou, p - bou );
            HMSG( "BOUNDARY '%s'", bou );
          }
          else
          {
            HMSG( "NOT BOUNDARY '%s'", p );
          }
        }
      }
      /* ict = mas_fileinfo_content_type_by_ext( fileinfo ); */
      /* if ( ict > 0 )                                      */
      /* mas_fileinfo_set_icontent_type( http->request_content, ict ); */
    }

  }
  return http;
}

/* static void                                                                                                                      */
/* w_action( const void *nodep, const VISIT which, const int depth )                                                                */
/* {                                                                                                                                */
/*   if ( which == endorder || which == leaf )                                                                                      */
/*   {                                                                                                                              */
/*     const char *name = NULL;                                                                                                     */
/*                                                                                                                                  */
/*     if ( nodep )                                                                                                                 */
/*     {                                                                                                                            */
/*       mas_varset_class_t *vclass;                                                                                                */
/*       unsigned long id;                                                                                                          */
/*                                                                                                                                  */
/*       vclass = *( ( mas_varset_class_t ** ) nodep );                                                                             */
/*       id = mas_varset_vclass_id( vclass );                                                                                       */
/*       name = mas_varset_vclass_name( vclass );                                                                                   */
/*       (* fprintf( stderr, "nodep:%p; which:%lu; depth:%lu --- %s\n", nodep, ( unsigned long ) which, ( unsigned long ) depth, *) */
/*       (*          name ? name : "-" );                                                                                        *) */
/*       EMSG( "[%lu] %lu. %s\n", ( unsigned long ) depth, id, name ? name : "-" );                                                 */
/*       if ( vclass->veccnt )                                                                                                      */
/*       {                                                                                                                          */
/*         mas_varvec_element_t *vec;                                                                                               */
/*                                                                                                                                  */
/*         vec = vclass->vec;                                                                                                       */
/*         for ( int i = 0; i < vclass->veccnt; i++ )                                                                               */
/*         {                                                                                                                        */
/*           int j;                                                                                                                 */
/*           char *name;                                                                                                            */
/*           char *val;                                                                                                             */
/*                                                                                                                                  */
/*           j = i * 2;                                                                                                             */
/*           name = mas_varset_vclass_variable_get_name( &vec[j] );                                                                 */
/*           val = mas_varset_vclass_variable_get_value( &vec[j] );                                                                 */
/*           EMSG( "\t\t\t-- %s='%s'\n", name, val );                                                                               */
/*           mas_free( val );                                                                                                       */
/*           mas_free( name );                                                                                                      */
/*         }                                                                                                                        */
/*       }                                                                                                                          */
/*     }                                                                                                                            */
/*   }                                                                                                                              */
/* }                                                                                                                                */

static mas_http_t *
mas_proto_http_parse_header( mas_rcontrol_t * prcontrol, mas_http_t * http, char *pstring )
{
  char *name = NULL, *ename = NULL, *values = NULL, *evalue = NULL;

  if ( pstring )
  {
    name = pstring;
    ename = pstring;
    while ( ename && *ename > ' ' && *ename != ':' )
      ename++;
    values = ename;
    if ( values && *values == ':' )
      values++;
    while ( values && *values == ' ' )
      values++;
    evalue = values;
    while ( evalue && *evalue && *evalue >= ' ' )
      evalue++;

    HMSG( "HTTP H: '%s' = '%s'", name, values );
    name = mas_strndup( name, ename - name );
    values = mas_strndup( values, evalue - values );
    MAS_LOG( "http parse headers name:'%s' values:'%s'", name, values );
    HMSG( "HTTP H: '%s' = '%s'", name, values );

    http->indata = mas_varvec_search_variable( http->indata, NULL, name, values );
    HMSG( "HTTP HEADER (parse) %s='%s'", name, values );

#if 0
    if ( 0 == strcasecmp( name, "User-Agent" ) )
    {
      if ( 0 == strncmp( values, "httperf", 7 ) )
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
#endif
    HMSG( "HTTP HEADER (known) %s='%s'", name, values );
    http = mas_proto_http_parse_known_header( prcontrol, http, name, values );
    if ( name )
      mas_free( name );
    if ( values )
      mas_free( values );
    MAS_LOG( "http parse headers 4" );
  }
  return http;
}

mas_http_t *
mas_proto_http_parse_headers( mas_rcontrol_t * prcontrol, mas_http_t * http )
{
  char *pstring = NULL;

  HMSG( "HTTP HEADERS (parse)" );
  MAS_LOG( "http parse headers 1" );
  do
  {
    if ( pstring )
      mas_free( pstring );
    pstring = mas_channel_buffer_nl_dup( prcontrol->h.pchannel );
    MAS_LOG( "http parse header '%s'", pstring );
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

  if ( !http->connection_keep_alive && !http->connection_close )
    prcontrol->connection_keep_alive = ( http->fversion >= 1.1 );
  else if ( http->connection_close )
    prcontrol->connection_keep_alive = 0;
  else if ( http->connection_keep_alive )
    prcontrol->connection_keep_alive = 1;
  MAS_LOG( "KA => %u", prcontrol->connection_keep_alive );
  if ( pstring )
    mas_free( pstring );
  pstring = NULL;
#ifdef MAS_HTTP_USE_FILEINFO
  HMSG( "L:%lu ; B: %s", ( unsigned long ) mas_fileinfo_data_size( http->request_content ), http->boundary );
#elif defined( MAS_HTTP_USE_AUTOOBJECT )
#endif
  return http;
}
