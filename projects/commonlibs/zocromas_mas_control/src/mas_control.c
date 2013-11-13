#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include <errno.h>
#include <pthread.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_tools.h>
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

  /* ctrl.messages = !popts->flag.name.nomessages; */
  ctrl.messages = !OPT_QFLAG( popts, nomessages );
  /* ctrl.messages_child = !popts->flag.name.nomessages_child && ctrl.messages; */
  ctrl.messages_child = !OPT_QFLAG( popts, nomessages_child ) && ctrl.messages;
  /* ctrl.messages_parent = !popts->flag.name.nomessages_parent && ctrl.messages; */
  ctrl.messages_parent = !OPT_QFLAG( popts, nomessages_parent ) && ctrl.messages;
  ctrl.messages_set = 1;
  if ( !ctrl.msgfile )
    ctrl.msgfile = ctrl.stderrfile;

  /* ctrl.daemon = !popts->flag.name.daemon.disable; */
  ctrl.daemon = !OPT_QFLAG( popts, daemon.disable );
  ctrl.ticker_mode = popts->ticker_mode;
  /* ctrl.redirect_std = !popts->flag.name.daemon.disable_redirect_std; */
  ctrl.redirect_std = !OPT_QFLAG( popts, daemon.disable_redirect_std );
  /* ctrl.close_std = !popts->flag.name.daemon.disable_close_std; */
  ctrl.close_std = !OPT_QFLAG( popts, daemon.disable_close_std );
  if ( message )
    *message = __func__;

  return 0;
}

int
mas_ctrl_destroy( void )
{
  CTRL_PREPARE;
  if ( ctrl.logpath )
    mas_free( ctrl.logpath );
  ctrl.logpath = NULL;
  {
    if ( ctrl.argvname )
      mas_free( ctrl.argvname );
    ctrl.argvname = NULL;

    if ( ctrl.progname )
      mas_free( ctrl.progname );
    ctrl.progname = NULL;

    if ( ctrl.exepath )
      mas_free( ctrl.exepath );
    ctrl.exepath = NULL;

    if ( ctrl.exename )
      mas_free( ctrl.exename );
    ctrl.exename = NULL;
  }

  {
    if ( ctrl.cmdline )
      mas_free( ctrl.cmdline );
    ctrl.cmdline = NULL;
    if ( ctrl.cmdargv.c && ctrl.cmdargv.v )
    {
      mas_del_argv( ctrl.cmdargv.c, ctrl.cmdargv.v, 0 );
    }
    ctrl.cmdargv.c = 0;
    ctrl.cmdargv.v = NULL;
    if ( ctrl.cmdenv )
      mas_free( ctrl.cmdenv );
  }
  {
    ctrl.cmdenv = NULL;
    if ( ctrl.cmdenvv.c && ctrl.cmdenvv.v )
    {
      mas_del_argv( ctrl.cmdenvv.c, ctrl.cmdenvv.v, 0 );
    }
    ctrl.cmdenvv.c = 0;
    ctrl.cmdenvv.v = NULL;
  }

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

int
mas_add_argv_zs( int pargc, char ***pargv, size_t zsize, const char *zline )
{
  const char *ps, *p, *eps;

  /* char **pargv = NULL; */
  /* int pargc = 0; */

  ps = p = zline;
  eps = ps + zsize;
  while ( ps && *ps && ps < eps )
  {
    while ( ps && *ps && ps < eps )
      ps++;
    if ( ps++ >= eps )
      break;
    pargc = mas_add_argv_arg( pargc, pargv, p );
    p = ps;
  }
  /* for ( int ia = 0; ia < pargc; ia++ )                   */
  /*   fprintf( stderr, "\n%d. '%s'", ia, ( *pargv )[ia] ); */
  /* fprintf( stderr, "\n" );                               */
  return pargc;
}

static int
mas_control_construct_proc( void )
{
  /* EVAL_PREPARE; */
  CTRL_PREPARE;
  int r = 0;

  {
    if ( ctrl.get_cmdline )
    {
      int fd = -1;

      if ( ctrl.cmdline )
        mas_free( ctrl.cmdline );
      ctrl.cmdline = NULL;
      ctrl.cmdline_size = 0;

      fd = open( "/proc/self/cmdline", O_RDONLY );
      if ( fd )
      {
        char buf[1024 * 4];

        ctrl.cmdline_size = read( fd, buf, sizeof( buf ) );
        ctrl.cmdline = mas_malloc( ctrl.cmdline_size + 1 );
        memcpy( ctrl.cmdline, buf, ctrl.cmdline_size );
        close( fd );
      }
    }
    if ( ctrl.get_cmdenv )
    {
      int fd = -1;

      ctrl.cmdenv_size = 0;
      if ( ctrl.cmdenv )
        mas_free( ctrl.cmdenv );
      ctrl.cmdenv = NULL;
      ctrl.cmdenv_size = 0;

      fd = open( "/proc/self/environ", O_RDONLY );
      if ( fd >= 0 )
      {
        size_t rsz = 0;
        size_t bufsz = 1024 * 100;

        ctrl.cmdenv = mas_malloc( bufsz );
        rsz = read( fd, ctrl.cmdenv, bufsz );
        if ( rsz < 0 )
        {
          mas_free( ctrl.cmdenv );
          ctrl.cmdenv = NULL;
        }
        else
        {
          if ( rsz )
          {
            ctrl.cmdenv = mas_realloc( ctrl.cmdenv, rsz );
            ctrl.cmdenv_size = rsz;
          }
          /* if ( ctrl.cmdenv && ctrl.cmdenv_size )                                                            */
          /*   fprintf( stderr, "\n(%ld)EEEE 0: [%s]\n", ctrl.cmdenv_size, ctrl.cmdenv ? ctrl.cmdenv : NULL ); */
        }
        close( fd );
      }
    }
  }
  /* char lexe[256]; */

  /* struct stat sb; */

  /* sprintf( lexe, "/proc/%u/exe", getpid(  ) ); */
  /* if ( lstat( lexe, &sb ) >= 0 ) */
  /* IEVAL( r, lstat( lexe, &sb ) ); */
  if ( !( r < 0 ) && !( ctrl.exepath && ctrl.exename ) )
  {
    char linkname[512];

    /* size_t sz; */

    /* sz = ( sb.st_size ? sb.st_size : 512 ) + 1; */
    /* linkname = mas_malloc( sz ); */
    /* r = readlink( lexe, linkname, sz ); */
    IEVAL( r, readlink( "/proc/self/exe", linkname, sizeof( linkname ) ) );
    if ( !( r < 0 ) )
    {
      linkname[r] = '\0';
      fprintf( stderr, "\n(%d) [%ld] LINKNAME '%s'\n", r, ctrl.cmdline_size, linkname );
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
  {
    if ( ctrl.cmdline_size && ctrl.cmdline )
      ctrl.cmdargv.c = mas_add_argv_zs( ctrl.cmdargv.c, &ctrl.cmdargv.v, ctrl.cmdline_size, ctrl.cmdline );

    {
      const char *t;
      char *s = NULL;

      t = getenv( "HOME" );
      if ( t )
      {
        s = mas_strdup( "HOME=" );
        s = mas_strcat_x( s, t );
        if ( s )
          ctrl.cmdenvv.c = mas_add_argv_arg_nodup( ctrl.cmdenvv.c, &ctrl.cmdenvv.v, s );
      }
      ctrl.cmdenvv.c = mas_add_argv_zs( ctrl.cmdenvv.c, &ctrl.cmdenvv.v, ctrl.cmdenv_size, ctrl.cmdenv );
    }
    if ( ctrl.cmdenv_size && ctrl.cmdenv )
      ctrl.cmdenvv.c = mas_add_argv_zs( ctrl.cmdenvv.c, &ctrl.cmdenvv.v, ctrl.cmdenv_size, ctrl.cmdenv );
  }
  return r >= 0 ? 0 : r;
}

static void
mas_control_possibly_rerun( void )
{
  CTRL_PREPARE;
  if ( !ctrl.tested_rerun && ctrl.rerun_name && *ctrl.rerun_name && ctrl.cmdargv.c && ctrl.cmdargv.v )
  {
    ctrl.already_rerun = ( 0 == strcmp( ctrl.cmdargv.v[0], ctrl.rerun_name ) );
    ctrl.tested_rerun = 1;
  }
  {
    if ( ctrl.rerun && ctrl.rerun_early && ctrl.tested_rerun && !ctrl.already_rerun )
    {
      fprintf( stderr, "\nRERUNz %s as %s", ctrl.exepath, ctrl.rerun_name );
      if ( ctrl.cmdargv.c && ctrl.cmdargv.v )
        ctrl.cmdargv.v[0] = ctrl.rerun_name;
      else
        ctrl.cmdargv.c = mas_add_argv_arg( ctrl.cmdargv.c, &ctrl.cmdargv.v, ctrl.rerun_name );
      execve( ctrl.exepath, ctrl.cmdargv.v, ctrl.cmdenvv.v );
    }
    else
    {
      setenv( "MAS_ZOCROMAS_SERVER", "1", 0 );
    }
  }
  /* mas_del_argv( pargc, ctrl.cmdargv.v, 0 ); */
  /* ctrl.cmdargv.c = 0;                       */
  /* ctrl.cmdargv.v = NULL;                    */
}

__attribute__ ( ( constructor( 10000 ) ) )
     static void mas_constructor( void )
{
  CTRL_PREPARE;
  char name_buffer[2048] = "Unknown";
  extern char *program_invocation_short_name;

  if ( !ctrl.stderrfile )
    ctrl.stderrfile = stderr;
  if ( !ctrl.msgfile )
    ctrl.msgfile = ctrl.stderrfile;
  mas_control_construct_proc(  );
  mas_control_possibly_rerun(  );
  prctl( PR_GET_NAME, ( unsigned long ) name_buffer );
  mas_common_constructor( IL, 0 );
  fprintf( stderr, "+ %s : %s : %s\n", name_buffer, program_invocation_name, program_invocation_short_name );
}

__attribute__ ( ( destructor ) )
     static void mas_destructor( void )
{
  CTRL_PREPARE;
  {
    if ( ctrl.exepath )
      mas_free( ctrl.exepath );
    ctrl.exepath = NULL;
    if ( ctrl.exename )
      mas_free( ctrl.exename );
    ctrl.exename = NULL;
  }
  {
    if ( ctrl.cmdline )
      mas_free( ctrl.cmdline );
    ctrl.cmdline = NULL;
    ctrl.cmdline_size = 0;
    if ( ctrl.cmdargv.c && ctrl.cmdargv.v )
    {
      mas_del_argv( ctrl.cmdargv.c, ctrl.cmdargv.v, 0 );
      ctrl.cmdargv.c = 0;
      ctrl.cmdargv.v = NULL;
    }
  }
  {
    if ( ctrl.cmdenv )
      mas_free( ctrl.cmdenv );
    ctrl.cmdenv = NULL;
    ctrl.cmdenv_size = 0;
    if ( ctrl.cmdenvv.c && ctrl.cmdenvv.v )
    {
      mas_del_argv( ctrl.cmdenvv.c, ctrl.cmdenvv.v, 0 );
      ctrl.cmdenvv.c = 0;
      ctrl.cmdenvv.v = NULL;
    }
  }
  /* fprintf( stderr, "******************** DESTRUCTOr %s e%d\n", __FILE__, errno ); */
  mas_common_destructor( IL, 0 );
}
