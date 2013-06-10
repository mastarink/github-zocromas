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
mas_opts_destroy( MAS_PASS_OPTS_DECLARE1 )
{
  MAS_PASS_OPTS_DECL_PREF;
  mas_del_argv( MAS_PASS_OPTS_PREF hostsv.c, MAS_PASS_OPTS_PREF hostsv.v, 0 );
  MAS_PASS_OPTS_PREF hostsv.c = 0;
  MAS_PASS_OPTS_PREF hostsv.v = NULL;

  mas_del_argv( MAS_PASS_OPTS_PREF protosv.c, MAS_PASS_OPTS_PREF protosv.v, 0 );
  MAS_PASS_OPTS_PREF protosv.c = 0;
  MAS_PASS_OPTS_PREF protosv.v = NULL;

  mas_del_argv( MAS_PASS_OPTS_PREF commandsv.c, MAS_PASS_OPTS_PREF commandsv.v, 0 );
  MAS_PASS_OPTS_PREF commandsv.c = 0;
  MAS_PASS_OPTS_PREF commandsv.v = NULL;

  if ( MAS_PASS_OPTS_PREF uuid )
    mas_free( MAS_PASS_OPTS_PREF uuid );
  MAS_PASS_OPTS_PREF uuid = NULL;

  if ( MAS_PASS_OPTS_PREF dir.mods )
    mas_free( MAS_PASS_OPTS_PREF dir.mods );
  MAS_PASS_OPTS_PREF dir.mods = NULL;

  if ( MAS_PASS_OPTS_PREF dir.pids )
    mas_free( MAS_PASS_OPTS_PREF dir.pids );
  MAS_PASS_OPTS_PREF dir.pids = NULL;

  if ( MAS_PASS_OPTS_PREF msgfilename )
    mas_free( MAS_PASS_OPTS_PREF msgfilename );
  MAS_PASS_OPTS_PREF msgfilename = NULL;

  if ( MAS_PASS_OPTS_PREF dir.proto )
    mas_free( MAS_PASS_OPTS_PREF dir.proto );
  MAS_PASS_OPTS_PREF dir.proto = NULL;

  if ( MAS_PASS_OPTS_PREF dir.log )
    mas_free( MAS_PASS_OPTS_PREF dir.log );
  MAS_PASS_OPTS_PREF dir.log = NULL;

  if ( MAS_PASS_OPTS_PREF dir.history )
    mas_free( MAS_PASS_OPTS_PREF dir.history );
  MAS_PASS_OPTS_PREF dir.history = NULL;

  if ( MAS_PASS_OPTS_PREF dir.post )
    mas_free( MAS_PASS_OPTS_PREF dir.post );
  MAS_PASS_OPTS_PREF dir.post = NULL;

  if ( MAS_PASS_OPTS_PREF dir.config )
    mas_free( MAS_PASS_OPTS_PREF dir.config );
  MAS_PASS_OPTS_PREF dir.config = NULL;

  if ( MAS_PASS_OPTS_PREF configfilename )
    mas_free( MAS_PASS_OPTS_PREF configfilename );
  MAS_PASS_OPTS_PREF configfilename = NULL;

  mas_del_argv( MAS_PASS_OPTS_PREF argvv.c, MAS_PASS_OPTS_PREF argvv.v, 0 );
  MAS_PASS_OPTS_PREF argvv.c = 0;
  MAS_PASS_OPTS_PREF argvv.v = NULL;
  return 0;
}

int
mas_pre_init_default_opts( MAS_PASS_OPTS_DECLARE1 )
{
  MAS_PASS_OPTS_DECL_PREF;
#ifdef MAS_SERVER_STRING
  MAS_PASS_OPTS_PREF hostsv.c = mas_add_argv_arg( MAS_PASS_OPTS_PREF hostsv.c, &MAS_PASS_OPTS_PREF hostsv.v, MAS_SERVER_STRING );
#endif
#ifdef MAS_SERVER_DEF_PROTO
  MAS_PASS_OPTS_PREF protosv.c = mas_add_argv_arg( MAS_PASS_OPTS_PREF protosv.c, &MAS_PASS_OPTS_PREF protosv.v, MAS_SERVER_DEF_PROTO );
#endif
#ifdef MAS_BASE_DIR
#  ifdef MAS_LOG_DIR
  if ( !MAS_PASS_OPTS_PREF dir.log )
  {
    MAS_PASS_OPTS_PREF dir.log = mas_strdup( MAS_BASE_DIR "/" MAS_LOG_DIR );
    HMSG( "DEF.LOGDIR: %s < '%s'", MAS_PASS_OPTS_PREF dir.log, MAS_BASE_DIR "/" MAS_LOG_DIR );
  }
#  endif
#endif
#ifdef MAS_PIDS_DIR
  if ( !MAS_PASS_OPTS_PREF dir.pids )
  {
    MAS_PASS_OPTS_PREF dir.pids = mas_strdup( MAS_BASE_DIR "/" MAS_PIDS_DIR );
    HMSG( "DEF.PIDSDIR: %s", MAS_PASS_OPTS_PREF dir.pids );
  }
#endif


#define XSTR(s) STR(s)
#define STR(s) #s
#ifdef MAS_LIBDIR
#  ifdef MAS_MODS_DIR
  if ( !MAS_PASS_OPTS_PREF dir.mods )
  {
    MAS_PASS_OPTS_PREF dir.mods = mas_strdup( XSTR( MAS_LIBDIR ) "/" MAS_MODS_DIR );
    HMSG( "DEF.MODSDIR: %s", MAS_PASS_OPTS_PREF dir.mods );
  }
#  endif
#  ifdef MAS_PROTO_DIR
  if ( !MAS_PASS_OPTS_PREF dir.proto )
  {
    MAS_PASS_OPTS_PREF dir.proto = mas_strdup( XSTR( MAS_LIBDIR ) "/" MAS_PROTO_DIR );
    HMSG( "DEF.PROTODIR: %s", MAS_PASS_OPTS_PREF dir.proto );
  }
#  endif
#endif
  return 0;
}
