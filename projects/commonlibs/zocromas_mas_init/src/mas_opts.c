#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <stddef.h>
#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

/* #include <mastar/types/mas_control_types.h> */
#include <mastar/types/mas_opts_types.h>
/* extern mas_control_t ctrl; */
extern mas_options_t opts;

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
/* #include <mastar/log/mas_log.h> */

#include "mas_opts.h"


/*
this:
  mas_opts.c
related:
  mas_opts_common.c
  mas_opts_types.h
  mas_opts_data.c
  mas_msg_tools.c
  mas_control.c

*/

int
mas_opts_destroy( void )
{
  mas_del_argv( opts.hostsv.c, opts.hostsv.v, 0 );
  opts.hostsv.c = 0;
  opts.hostsv.v = NULL;

  mas_del_argv( opts.protosv.c, opts.protosv.v, 0 );
  opts.protosv.c = 0;
  opts.protosv.v = NULL;

  mas_del_argv( opts.commandsv.c, opts.commandsv.v, 0 );
  opts.commandsv.c = 0;
  opts.commandsv.v = NULL;

  if ( opts.uuid )
    mas_free( opts.uuid );
  opts.uuid = NULL;

  if ( opts.dir.mods )
    mas_free( opts.dir.mods );
  opts.dir.mods = NULL;

  if ( opts.dir.pids )
    mas_free( opts.dir.pids );
  opts.dir.pids = NULL;

  if ( opts.msgfilename )
    mas_free( opts.msgfilename );
  opts.msgfilename = NULL;

  if ( opts.dir.proto )
    mas_free( opts.dir.proto );
  opts.dir.proto = NULL;

  if ( opts.dir.log )
    mas_free( opts.dir.log );
  opts.dir.log = NULL;

  if ( opts.dir.history )
    mas_free( opts.dir.history );
  opts.dir.history = NULL;

  if ( opts.dir.post )
    mas_free( opts.dir.post );
  opts.dir.post = NULL;

  if ( opts.dir.config )
    mas_free( opts.dir.config );
  opts.dir.config = NULL;

  if ( opts.configfilename )
    mas_free( opts.configfilename );
  opts.configfilename = NULL;

  mas_del_argv( opts.argvv.c, opts.argvv.v, 0 );
  opts.argvv.c = 0;
  opts.argvv.v = NULL;
  return 0;
}

int
mas_pre_init_default_opts( void )
{
#ifdef MAS_SERVER_STRING
  opts.hostsv.c = mas_add_argv_arg( opts.hostsv.c, &opts.hostsv.v, MAS_SERVER_STRING );
#endif
#ifdef MAS_SERVER_DEF_PROTO
  opts.protosv.c = mas_add_argv_arg( opts.protosv.c, &opts.protosv.v, MAS_SERVER_DEF_PROTO );
#endif
#ifdef MAS_BASE_DIR
#  ifdef MAS_LOG_DIR
  if ( !opts.dir.log )
  {
    opts.dir.log = mas_strdup( MAS_BASE_DIR "/" MAS_LOG_DIR );
    HMSG( "DEF.LOGDIR: %s < '%s'", opts.dir.log, MAS_BASE_DIR "/" MAS_LOG_DIR );
  }
#  endif
#endif
#ifdef MAS_PIDS_DIR
  if ( !opts.dir.pids )
  {
    opts.dir.pids = mas_strdup( MAS_BASE_DIR "/" MAS_PIDS_DIR );
    HMSG( "DEF.PIDSDIR: %s", opts.dir.pids );
  }
#endif


#define XSTR(s) STR(s)
#define STR(s) #s
#ifdef MAS_LIBDIR
#  ifdef MAS_MODS_DIR
  if ( !opts.dir.mods )
  {
    opts.dir.mods = mas_strdup( XSTR( MAS_LIBDIR ) "/" MAS_MODS_DIR );
    HMSG( "DEF.MODSDIR: %s", opts.dir.mods );
  }
#  endif
#  ifdef MAS_PROTO_DIR
  if ( !opts.dir.proto )
  {
    opts.dir.proto = mas_strdup( XSTR( MAS_LIBDIR ) "/" MAS_PROTO_DIR );
    HMSG( "DEF.PROTODIR: %s", opts.dir.proto );
  }
#  endif
#endif
  return 0;
}
