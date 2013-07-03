#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <stddef.h>
#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

/* #include <mastar/types/mas_control_types.h> */
#include <mastar/types/mas_opts_types.h>


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
mas_opts_destroy( mas_options_t * popts )
{
  mas_del_argv( popts->hostsv.c, popts->hostsv.v, 0 );
  popts->hostsv.c = 0;
  popts->hostsv.v = NULL;

  mas_del_argv( popts->protosv.c, popts->protosv.v, 0 );
  popts->protosv.c = 0;
  popts->protosv.v = NULL;

  mas_del_argv( popts->commandsv.c, popts->commandsv.v, 0 );
  popts->commandsv.c = 0;
  popts->commandsv.v = NULL;

  if ( popts->uuid )
    mas_free( popts->uuid );
  popts->uuid = NULL;

  if ( popts->dir.mods )
    mas_free( popts->dir.mods );
  popts->dir.mods = NULL;

  if ( popts->dir.pids )
    mas_free( popts->dir.pids );
  popts->dir.pids = NULL;

  if ( popts->msgfilename )
    mas_free( popts->msgfilename );
  popts->msgfilename = NULL;

  if ( popts->dir.proto )
    mas_free( popts->dir.proto );
  popts->dir.proto = NULL;

  if ( popts->dir.log )
    mas_free( popts->dir.log );
  popts->dir.log = NULL;

  if ( popts->dir.history )
    mas_free( popts->dir.history );
  popts->dir.history = NULL;

  if ( popts->dir.post )
    mas_free( popts->dir.post );
  popts->dir.post = NULL;

  if ( popts->dir.config )
    mas_free( popts->dir.config );
  popts->dir.config = NULL;

  if ( popts->configfilename )
    mas_free( popts->configfilename );
  popts->configfilename = NULL;

  mas_del_argv( popts->argvv.c, popts->argvv.v, 0 );
  popts->argvv.c = 0;
  popts->argvv.v = NULL;
  return 0;
}

int
mas_pre_init_default_opts( mas_options_t * popts )
{
#ifdef MAS_SERVER_STRING
  popts->hostsv.c = mas_add_argv_arg( popts->hostsv.c, &popts->hostsv.v, MAS_SERVER_STRING );
#endif
#ifdef MAS_SERVER_DEF_PROTO
  popts->protosv.c = mas_add_argv_arg( popts->protosv.c, &popts->protosv.v, MAS_SERVER_DEF_PROTO );
#endif
#ifdef MAS_BASE_DIR
#  ifdef MAS_LOG_DIR
  if ( !popts->dir.log )
  {
    popts->dir.log = mas_strdup( MAS_BASE_DIR "/" MAS_LOG_DIR );
    HMSG( "DEF.LOGDIR: %s < '%s'", popts->dir.log, MAS_BASE_DIR "/" MAS_LOG_DIR );
  }
#  endif
#endif
#ifdef MAS_PIDS_DIR
  if ( !popts->dir.pids )
  {
    popts->dir.pids = mas_strdup( MAS_BASE_DIR "/" MAS_PIDS_DIR );
    HMSG( "DEF.PIDSDIR: %s", popts->dir.pids );
  }
#endif


#define XSTR(s) STR(s)
#define STR(s) #s
#ifdef MAS_LIBDIR
#  ifdef MAS_MODS_DIR
  if ( !popts->dir.mods )
  {
    popts->dir.mods = mas_strdup( XSTR( MAS_LIBDIR ) "/" MAS_MODS_DIR );
    HMSG( "DEF.MODSDIR: %s", popts->dir.mods );
  }
#  endif
#  ifdef MAS_PROTO_DIR
  if ( !popts->dir.proto )
  {
    popts->dir.proto = mas_strdup( XSTR( MAS_LIBDIR ) "/" MAS_PROTO_DIR );
    HMSG( "DEF.PROTODIR: %s", popts->dir.proto );
  }
#  endif
#endif
  return 0;
}
