#include <mastar/wrap/mas_std_def.h>

#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>

/* #include <mastar/types/mas_control_types.h> */
/* extern mas_control_t ctrl; */

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>

#include <mastar/fileinfo/mas_unidata.h>
#include <mastar/fileinfo/mas_fileinfo.h>
#include <mastar/fileinfo/mas_fileinfo_object.h>

#ifdef MAS_OLD_VARIABLES_HTTP
#  include <mastar/variables/mas_variables.h>
#else
#  include <mastar/types/mas_varset_types.h>
#  include <mastar/varset/mas_varset_vclass.h>
#  include <mastar/varset/mas_varset.h>
#endif

#include <mastar/channel/mas_channel_buffer.h>

#include "mas_http_request_headers.h"



static mas_http_t *
mas_proto_http_parse_known_header( mas_rcontrol_t * prcontrol, mas_http_t * http, const char *name, const char *value )
{
  HMSG( "HTTP H.KNOWN %s", name );
  if ( 0 == strcmp( name, "Connection" ) )
  {
    HMSG( "HTTP (known) HEADER (parse) %s", name );
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
  else if ( 0 == strcmp( name, "Content-Length" ) )
  {
    if ( !http->request_content )
      http->request_content = mas_fileinfo_create(  );
    mas_fileinfo_set_content_size( http->request_content, value );
  }
  else if ( 0 == strcmp( name, "Content-Type" ) )
  {
    if ( !http->request_content )
      http->request_content = mas_fileinfo_create(  );
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
        mas_fileinfo_set_icontent_type( http->request_content, ict );
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
/*         mas_var_t *vec;                                                                                                          */
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
  char *name = NULL, *ename = NULL, *value = NULL, *evalue = NULL;

  if ( pstring )
  {
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

    HMSG( "HTTP H: '%s' = '%s'", name, value );
    name = mas_strndup( name, ename - name );
    value = mas_strndup( value, evalue - value );
    MAS_LOG( "http parse headers name:'%s' value:'%s'", name, value );
    HMSG( "HTTP H: '%s' = '%s'", name, value );

#ifdef MAS_OLD_VARIABLES_HTTP
    http->indata = mas_variable_create_x( http->indata, /* MAS_THREAD_TRANSACTION, */ "inheader", name, NULL, "%s", value, 0 );
#else
    /* http->indata = mas_varset_search_variablef( http->indata, "inheader", name, NULL, "%s", value ); */
    http->indata = mas_varset_search_variable( http->indata, "inheader", name, value );
    /* {                                                                                                         */
    /*   mas_var_t *tv;                                                                                          */
    /*                                                                                                           */
    /*   tv = mas_varset_find_variable( http->indata, "inheader", name );                                        */
    /*   if ( tv )                                                                                               */
    /*   {                                                                                                       */
    /*     char *s;                                                                                              */
    /*                                                                                                           */
    /*     s = mas_varset_vclass_variable_get_value( tv );                                                       */
    /*     HMSG( "TO SET inheader '%s'='%s' -- '%s'", name, mas_varset_vclass_variable_get_value_ref( tv ), s ); */
    /*     mas_free( s );                                                                                        */
    /*   }                                                                                                       */
    /*   else                                                                                                    */
    /*   {                                                                                                       */
    /*     EMSG( "DIDN'T APPEAR VAR %s %d", name, http->indata ? 1 : 0 );                                        */
    /*     mas_varset_walk_classes( http->indata, w_action );                                                    */
    /*   }                                                                                                       */
    /* }                                                                                                         */
#endif
    HMSG( "HTTP HEADER (parse) %s='%s'", name, value );

#if 0
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
#endif
    HMSG( "HTTP HEADER (known) %s='%s'", name, value );
    http = mas_proto_http_parse_known_header( prcontrol, http, name, value );
    if ( name )
      mas_free( name );
    if ( value )
      mas_free( value );
    MAS_LOG( "http parse headers 4" );
  }
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
  if ( pstring )
    mas_free( pstring );
  pstring = NULL;
  HMSG( "L:%lu ; B: %s", ( unsigned long ) mas_fileinfo_data_size( http->request_content ), http->boundary );
  return http;
}
