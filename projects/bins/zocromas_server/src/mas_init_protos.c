#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib0.h>
#include <mastar/wrap/mas_lib.h>

#include <mastar/tools/mas_tools.h>


#include <mastar/varset/mas_varset_object.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#ifndef MAS_SERVER_NOLOG
#  include <mastar/log/mas_logger.h>
#  include <mastar/log/mas_log.h>
#endif

#include <mastar/types/mas_control_types.h>

#include <mastar/modules/mas_modules_load_module.h>

#include "mas_init_protos.h"

/* int                                                            */
/* mas_protos_load( mas_options_t * popts, const char **message ) */
INIT_HANDLER( mas_protos_init )
{
  CTRL_PREPARE;
  EVAL_PREPARE;
  int r = 0;
  mas_transaction_protodesc_t *proto_descs = NULL;

  MAS_LOG( "(%d) init / load protos", r );
  if ( !popts->protosv.c )
  {
    HMSG( "NO PROTCOLOS DEFINED" );
    WMSG( "no protcolos defined" );
    IEVAL( r, -1 );
  }
  if ( !ctrl.proto_descs )
  {
    int protos_num = 0;

    proto_descs = mas_calloc( popts->protosv.c, sizeof( mas_transaction_protodesc_t ) );
    memset( proto_descs, 0, popts->protosv.c * sizeof( mas_transaction_protodesc_t ) );
    for ( int ipr = 0; ipr < popts->protosv.c; ipr++ )
    {
      /* from one */
      proto_descs[ipr].proto_id = protos_num + 1;
      proto_descs[ipr].name = mas_strdup( popts->protosv.v[ipr] );
      proto_descs[ipr].func =
            ( mas_transaction_fun_t ) mas_modules_load_func_from( popts->protosv.v[ipr], "mas_proto_main", popts->dir.proto );
      if ( !proto_descs[ipr].func )
      {
        EMSG( "PROTO LOAD %s FAIL", proto_descs[ipr].name );
        /* IEVAL( 20130731 --- r, -1 ); */
        WMSG( "INIT PROTOS - #%d: %s", ipr, popts->protosv.v[ipr] );
      }
      else
      {
        WMSG( "INIT PROTOS + #%d: %s", ipr, popts->protosv.v[ipr] );
      }
      protos_num++;
      MAS_LOG( "(%d) init / load protos #%d", r, protos_num );
    }
    ctrl.protos_num = protos_num;
    ctrl.proto_descs = proto_descs;
    /* 20130731 ---                                */
    /* if ( popts->protosv.c && !ctrl.protos_num ) */
    /* {                                           */
    /*   IEVAL( r, -1 );                           */
    /* }                                           */
    HMSG( "(%d) INIT S PROTOS %d of %d", r, protos_num, popts->protosv.c );
  }
  else
  {
    IEVAL( r, -1 );
  }
  /* r = ctrl.proto_descs ? 0 : -1; */

  /*  20130731 --- IEVAL( r, ctrl.proto_descs ? 0 : -1 ); */

  MAS_LOG( "(%d) init / load protos done", r );
  if ( message )
    *message = __func__;
  return r < 0 ? r : 0;
}

int
mas_protos_destroy( void )
{
  CTRL_PREPARE;
  int r = 0;

  for ( int ipr = 0; ipr < ctrl.protos_num; ipr++ )
  {
    mas_varset_t *vars;

    vars = ctrl.proto_descs[ipr].variables;
    ctrl.proto_descs[ipr].variables = NULL;
    if ( vars )
      mas_varset_delete( vars );
  }
  for ( int ipr = 0; ipr < ctrl.protos_num; ipr++ )
    if ( ctrl.proto_descs[ipr].name )
      mas_free( ctrl.proto_descs[ipr].name );
  if ( ctrl.proto_descs )
    mas_free( ctrl.proto_descs );
  ctrl.protos_num = 0;
  ctrl.proto_descs = NULL;

  return r;
}

__attribute__ ( ( constructor( 1001 ) ) )
     static void mas_constructor( void )
{
  /* fprintf( stderr, "******************** CONSTRUCTOr %s e%d\n", __FILE__, errno ); */
  mas_common_constructor( IL, 0 );
}
