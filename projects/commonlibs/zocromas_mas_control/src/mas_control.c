#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/prctl.h>

#include <errno.h>
#include <pthread.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>

#include "mas_control.h"

/*
this:
  mas_control.c
related:
  mas_control_data.c
  mas_common.h
  mas_tools.c
  mas_opts.c
  mas_cs.c
  mas_cs.h

more:
  mas_rcontrol_object.c
*/


/*
 * */
int
mas_ctrl_init( mas_options_t * popts, const char **message )
{
  CTRL_PREPARE;
  /* ctrl.is_client / ctrl.is_server set at the beginning of mas_init_client / mas_init_server */
  ctrl.in_client = 0;
  ctrl.keep_listening = 0;
  if ( ctrl.is_client )
    ctrl.in_client = 1;
  else
    ctrl.keep_listening = 1;

  ctrl.messages = !popts->nomessages;
  ctrl.messages_child = !popts->nomessages_child && ctrl.messages;
  ctrl.messages_parent = !popts->nomessages_parent && ctrl.messages;
  ctrl.messages_set = 1;
  if ( !ctrl.msgfile )
    ctrl.msgfile = ctrl.stderrfile;

  ctrl.daemon = !popts->daemon.disable;
  ctrl.ticker_mode = popts->ticker_mode;
  ctrl.redirect_std = !popts->daemon.disable_redirect_std;
  ctrl.close_std = !popts->daemon.disable_close_std;
  if ( message )
    *message = __func__;

  return 0;
}

int
mas_ctrl_destroy( void )
{
  CTRL_PREPARE;
  mas_del_argv( ctrl.commandsv.c, ctrl.commandsv.v, 0 );
  ctrl.commandsv.c = 0;
  ctrl.commandsv.v = NULL;

  mas_del_argv( ctrl.loaded_optsv.c, ctrl.loaded_optsv.v, 0 );
  ctrl.loaded_optsv.c = 0;
  ctrl.loaded_optsv.v = NULL;

  /* if ( ctrl.threads.n.daemon.pid && ctrl.threads.n.daemon.pid == getpid(  ) ) */
  /*   for ( int ifil = 0; ifil < ctrl.pidfilesv.c; ifil++ )                     */
  /*   {                                                                         */
  /*     (* HMSG( "PID FILE %d. %s", ifil, ctrl.pidfilesv.v[ifil] ); *)          */
  /*     unlink( ctrl.pidfilesv.v[ifil] );                                       */
  /*   }                                                                         */
  /*                                                                             */
  /* mas_del_argv( ctrl.pidfilesv.c, ctrl.pidfilesv.v, 0 );                      */
  /* ctrl.pidfilesv.c = 0;                                                       */
  /* ctrl.pidfilesv.v = NULL;                                                    */

  /* for ( int ipd = 0; ipd < ctrl.protos_num; ipd++ ) */
  /*   if ( ctrl.proto_descs[ipd].name )               */
  /*     mas_free( ctrl.proto_descs[ipd].name );       */
  /* if ( ctrl.proto_descs )                           */
  /*   mas_free( ctrl.proto_descs );                   */
  /* ctrl.protos_num = 0;                              */
  /* ctrl.proto_descs = NULL;                          */
  return 0;
}

void
mas_ctrl_add_command( const char *s )
{
  CTRL_PREPARE;
  /* const char *se;              */
  /*                              */
  /* se = mas_find_eq_value( s ); */
  ctrl.commandsv.c = mas_add_argv_arg( ctrl.commandsv.c, &ctrl.commandsv.v, s );
}

static int
mas_control_construct_proc( void )
{
  EVAL_PREPARE;
  int r = 0;

  /* char lexe[256]; */

  /* struct stat sb; */

  /* sprintf( lexe, "/proc/%u/exe", getpid(  ) ); */
  /* if ( lstat( lexe, &sb ) >= 0 ) */
  /* IEVAL( r, lstat( lexe, &sb ) ); */
  if ( !( r < 0 ) && !( ctrl.exepath && ctrl.exename ) )
  {
    char linkname[512];
    ssize_t cmdsize = 0;

    {
      FILE *f = NULL;
      char buf[1024 * 4];

      f = fopen( "/proc/self/cmdline", "r" );
      if ( f )
      {
        cmdsize = fread( buf, 1, sizeof( buf ), f );
        fclose( f );
      }
    }
    /* size_t sz; */

    /* sz = ( sb.st_size ? sb.st_size : 512 ) + 1; */
    /* linkname = mas_malloc( sz ); */
    /* r = readlink( lexe, linkname, sz ); */
    IEVAL( r, readlink( "/proc/self/exe", linkname, sizeof( linkname ) ) );
    if ( !( r < 0 ) )
    {
      linkname[r] = '\0';
      fprintf( stderr, "\n(%d) [%ld] LINKNAME '%s'\n", r, cmdsize, linkname );
    }
    {
      CTRL_PREPARE;
      if ( ctrl.exepath )
        mas_free( ctrl.exepath );
      ctrl.exepath = NULL;
      ctrl.exepath = mas_strdup( linkname );
      if ( ctrl.exename )
        mas_free( ctrl.exename );
      ctrl.exename = NULL;
      if ( ctrl.exepath )
        ctrl.exename = mas_strdup( basename( ctrl.exepath ) );
    }
  }

  return r >= 0 ? 0 : r;
}

__attribute__ ( ( constructor( 10000 ) ) )
     static void master_constructor( void )
{
  CTRL_PREPARE;
  char name_buffer[2048] = "Unknown";
  extern char *program_invocation_short_name;

  if ( !ctrl.stderrfile )
    ctrl.stderrfile = stderr;
  if ( !ctrl.msgfile )
    ctrl.msgfile = ctrl.stderrfile;
  mas_control_construct_proc(  );
  prctl( PR_GET_NAME, ( unsigned long ) name_buffer );
  fprintf( stderr, "******************** CONSTRUCTOR %s : %s : %s : %s\n", __FILE__, name_buffer, program_invocation_name,
           program_invocation_short_name );
  if ( 0 != strcmp( program_invocation_short_name, "ZOCSer" ) )
  {
  }
  else
  {
  }
}

__attribute__ ( ( destructor ) )
     static void master_destructor( void )
{
  CTRL_PREPARE;
  if ( ctrl.exepath )
    mas_free( ctrl.exepath );
  ctrl.exepath = NULL;
  if ( ctrl.exename )
    mas_free( ctrl.exename );
  ctrl.exename = NULL;

  fprintf( stderr, "******************** DESTRUCTOR %s e%d\n", __FILE__, errno );
}
