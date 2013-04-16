#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <stddef.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#ifdef HAVE_LIBUUID
#  include <uuid/uuid.h>
#endif
#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/tools/mas_tools.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>

/* #include <mastar/variables/mas_variables.h> */

#include "mas_opts_common.h"
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
mas_opts_set_configdir( const char *dirname )
{
  char *wdirname = NULL;

  if ( opts.dir.config )
  {
    mas_free( opts.dir.config );
    opts.dir.config = NULL;
  }
  wdirname = opts.dir.config;
  if ( !wdirname )
    wdirname = dirname ? mas_strdup( dirname ) : NULL;
  if ( !wdirname )
    wdirname = opts.dir.config ? mas_strdup( opts.dir.config ) : NULL;
  if ( !wdirname )
  {
    char *t;

    t = getenv( "MAS_ZOCROMAS_CONFIGDIR" );
    if ( t )
      wdirname = mas_strdup( t );
  }
  if ( !wdirname )
  {
    wdirname = mas_strdup( getenv( "HOME" ) );
    wdirname = mas_strcat_x( wdirname, "/.zocromas/" );
  }
  if ( opts.dir.config && *opts.dir.config && *( opts.dir.config + strlen( opts.dir.config ) - 1 ) != '/' )
  {
    wdirname = mas_strcat_x( wdirname, "/" );
  }
  opts.dir.config = wdirname;
  /* FMSG( "setting opts.dir.config:%s", opts.dir.config ); */
  return 0;
}

int
mas_opts_set_configfilename( const char *filename )
{
  char *wfilename = NULL;

  if ( opts.configfilename )
    mas_free( opts.configfilename );
  opts.configfilename = NULL;

  wfilename = filename ? mas_strdup( filename ) : NULL;
  if ( !wfilename )
  {
    char *t;

    t = getenv( "MAS_ZOCROMAS_CONFIGFILE" );
    if ( t )
      wfilename = mas_strdup( t );
  }
  if ( !wfilename )
  {
    wfilename = mas_strdup( "zocromasrc" );
  }
  opts.configfilename = wfilename;
  /* FMSG( "setting opts.configfilename:%s", opts.configfilename ); */
  return 0;
}

int
mas_opts_check_dir( void )
{
  int r = 0;
  struct stat dir_stat;

  if ( !opts.dir.config || !*opts.dir.config )
    IEVALM( r, -1, "(%d)config dir not set", NULL );
  /* r = stat( opts.dir.config, &dir_stat ); */
  IEVALM( r, stat( opts.dir.config, &dir_stat ), "(%d)no configdir: '%s'", opts.dir.config );
  /* if ( r < 0 )                                     */
  /* {                                                */
  /*   if ( errno == ENOENT )                         */
  /*   {                                              */
  /*     EMSG( "no configdir : %s", opts.dir.config ); */
  /*   }                                              */
  /*   else                                           */
  /*   {                                              */
  /*     EMSG( "why? : %s", opts.dir.config );         */
  /*   }                                              */
  /* }                                                */
  /* else                                             */
  /* {                                                */
  /* }                                                */
  return r;
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
