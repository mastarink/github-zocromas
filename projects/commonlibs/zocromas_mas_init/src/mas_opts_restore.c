#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <stddef.h>
#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
/* #include <mastar/log/mas_log.h> */

#include "mas_opts_common.h"
#include "mas_opts_storage.h"

#include "mas_opts_restore.h"

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

static int
mas_opts_set_argv( int *pargc, char ***pargv, const char *s )
{
  int r = 0;
  const char *se;

  se = mas_find_eq_value( s );
  if ( se )
  {
    *pargc = mas_add_argv_arg( *pargc, pargv, se );
  }
  else
  {
    IEVAL( r, -1 );
  }
  return r;
}

static void
mas_opts_add_command( const char *s )
{
  const char *se;

  se = mas_find_eq_value( s );
  opts.commandsv.c = mas_add_argv_arg( opts.commandsv.c, &opts.commandsv.v, se );
}

static unsigned
mas_opts_atou( const char *s )
{
  unsigned rv = 0;
  const char *pv;

  pv = mas_find_eq_value( s );
  if ( pv )
    rv = atol( pv );
  return rv;
}






mas_option_parse_t opt_table[] = {
  {.name = "env_optsname",.type = MAS_OPT_TYPE_STR,.offset = offsetof( mas_options_t, env_optsname ),.size = sizeof( opts.env_optsname )}
  ,
  {.name = "env_hostname",.type = MAS_OPT_TYPE_STR,.offset = offsetof( mas_options_t, env_hostname )}
  ,
  {.name = "uuid",.type = MAS_OPT_TYPE_PSTR,.offset = offsetof( mas_options_t, uuid )}
  ,
  {.name = "dir.mods",.type = MAS_OPT_TYPE_PSTR,.offset = offsetof( mas_options_t, dir ) + offsetof( mas_dirs_t, mods )}
  ,
  {.name = "dir.pids",.type = MAS_OPT_TYPE_PSTR,.offset = offsetof( mas_options_t, dir ) + offsetof( mas_dirs_t, pids )}
  ,
  {.name = "dir.proto",.type = MAS_OPT_TYPE_PSTR,.offset = offsetof( mas_options_t, dir ) + offsetof( mas_dirs_t, proto )}
  ,
  {.name = "dir.log",.type = MAS_OPT_TYPE_PSTR,.offset = offsetof( mas_options_t, dir ) + offsetof( mas_dirs_t, log )}
  ,
  {.name = "dir.history",.type = MAS_OPT_TYPE_PSTR,.offset = offsetof( mas_options_t, dir ) + offsetof( mas_dirs_t, history )}
  ,
  {.name = "dir.post",.type = MAS_OPT_TYPE_PSTR,.offset = offsetof( mas_options_t, dir ) + offsetof( mas_dirs_t, post )}
  ,
  {.name = "msgfilename",.type = MAS_OPT_TYPE_PSTR,.offset = offsetof( mas_options_t, msgfilename )}
  ,
  {.name = "restart_sleep",.type = MAS_OPT_TYPE_DOUBLE,.offset = offsetof( mas_options_t, restart_sleep )}
  ,
};

int
mas_opts_restore_flags( const char *s )
{
#define OPT_FLAG(name,val) else if ( 0 == mas_strcmp2( val, #name "=" ) ) opts.name = ( mas_opts_atou(val) )
#define OPT_NOFLAG(name,val) else if ( 0 == mas_strcmp2( val, #name "=" ) ) opts.no##name = !( mas_opts_atou(val) )

  if ( 0 == mas_strcmp2( s, "message=" ) )
  {
    HMSG( "RESTORE OPTS: %s", mas_find_eq_value( s ) );
  }
  OPT_NOFLAG( log, s );
  OPT_NOFLAG( logger, s );
  OPT_FLAG( max_config_backup, s );
  OPT_FLAG( default_port, s );
  OPT_NOFLAG( daemon, s );
  OPT_FLAG( read_user_opts, s );
  OPT_FLAG( read_user_opts_plus, s );
  OPT_FLAG( single_instance, s );
  OPT_FLAG( single_child, s );
  OPT_NOFLAG( messages, s );
  OPT_FLAG( save_user_opts, s );
  OPT_FLAG( save_user_opts_plus, s );
  OPT_FLAG( overwrite_user_opts, s );
  OPT_FLAG( overwrite_user_opts_plus, s );
  OPT_FLAG( disconnect_prompt, s );
  OPT_FLAG( wait_server, s );
  return 0;
}


int
mas_opts_restore_new_section( const char *section )
{
  if ( opts.protosv.v && 0 == strcmp( section, "protos" ) )
  {
    mas_del_argv( opts.protosv.c, opts.protosv.v, 0 );
    opts.protosv.c = 0;
    opts.protosv.v = NULL;
  }
  else if ( opts.commandsv.v && 0 == strcmp( section, "commands" ) )
  {
    mas_del_argv( opts.commandsv.c, opts.commandsv.v, 0 );
    opts.commandsv.c = 0;
    opts.commandsv.v = NULL;
  }
  return 0;
}

int
mas_opts_restore_at_section( const char *section, const char *s )
{
  int r = 0;

  if ( 0 == mas_strcmp2( s, "host=" ) )
  {
    if ( 0 == strcmp( section, ctrl.is_client ? "hosts" : "listen" ) )
    {
      /* mMSG( "%d. +HOST :%s", opts.hostsv.c, s ); */
      IEVAL( r, mas_opts_set_argv( &opts.hostsv.c, &opts.hostsv.v, s ) );
    }
    else
    {
      mMSG( "%d. HOST :%s @ [%s] %d", opts.hostsv.c, s, section, ctrl.is_client );
    }
  }
  else if ( 0 == mas_strcmp2( s, "proto=" ) )
  {
    if ( 0 == strcmp( section, "protos" ) )
    {
      /* mMSG( "%d. +PROTO :%s", opts.protosv.c, s ); */
      IEVAL( r, mas_opts_set_argv( &opts.protosv.c, &opts.protosv.v, s ) );
    }
    else
    {
      mMSG( "%d. PROTO :%s @ [%s]", opts.protosv.c, s, section );
    }
  }
  else if ( 0 == mas_strcmp2( s, "command=" ) && 0 == strcmp( section, "commands" ) )
    mas_opts_add_command( s );
  /* for ( int ih = 0; ih < opts.commandsv.c; ih++ ) */
  /* {                                                */
  /*   mMSG( "command:%s", opts.commandsv.v[ih] );       */
  /* }                                                */
  return r;
}

int
mas_opts_restore_relative( const char *filename )
{
  int r = 0;
  char *fpath = NULL;

  fpath = mas_strdup( opts.dir.config );
  fpath = mas_strcat_x( fpath, "/" );
  fpath = mas_strcat_x( fpath, filename );
  r = mas_opts_restore_path( fpath );
  mas_free( fpath );
  return r;
}

int
mas_opts_restore_path( const char *fpath )
{
  int r = 0;

  r = _mas_opts_restore_path( fpath, &opts, opt_table, sizeof( opt_table ) / sizeof( opt_table[0] ), NULL, opts.new_section_func,
                              opts.at_section_func, opts.unknown_opt_func );
  ctrl.loaded_optsv.c = mas_add_argv_args( ctrl.loaded_optsv.c, &ctrl.loaded_optsv.v, fpath, 0 );
  return r;
}

int
_mas_opts_restore( const char *dirname, const char *filename )
{
  int r = 0;

  if ( opts.dir.config )
  {
    mas_free( opts.dir.config );
    opts.dir.config = NULL;
  }
  IEVALM( r, mas_opts_set_configdir( dirname ), "(%d)set config dir: '%s'", dirname );
  IEVALM( r, mas_opts_set_configfilename( filename ), "(%d)opts file:'%s'", filename );
  IEVALM( r, mas_opts_check_dir(  ), "(%d)config dir: '%s'", opts.dir.config );
  if ( r == 0 )
  {
    mas_opts_restore_relative( opts.configfilename );
    HMSG( "RESTORE OPT nomessages: %d", opts.nomessages );
  }
  return r;
}

int
mas_opts_restore_user( const char *dirname, const char *filename )
{
  int r = 0;

  if ( filename )
  {
    char *fn;

    /* mMSG( "FILENAME: %s", filename ); */
    fn = mas_strdup( filename );
    IEVAL( r, _mas_opts_restore( dirname, fn ) );
    /* mMSG( "BBB: %s - %d", getenv( "MAS_PID_AT_BASHRC" ), r ); */
    mas_free( fn );
  }
  return r;
}

int
_mas_opts_restore_plus( const char *dirname, const char *filename, va_list args )
{
  int r = 0;
  char *s = NULL;
  int x = 0;
  char *fn = NULL;

  if ( filename )
  {
    fn = mas_strdup( filename );

    /* FMSG( "FN:%s", fn ); */
    while ( ( s = va_arg( args, char * ) ) )
    {
      if ( s )
      {
        fn = mas_strcat_x( fn, s );
        x++;
      }
    }
    if ( x )
    {
      /* HMSG( "OPTS from:%s", fn ); */
      IEVAL( r, _mas_opts_restore( dirname, fn ) );
    }
    mas_free( fn );
  }
  else
  {
    IEVAL( r, -1 );
  }
  return r;
}

int
mas_opts_restore_user_plus( const char *dirname, const char *filename, ... )
{
  int r = 0;
  va_list args;

  va_start( args, filename );
  IEVAL( r, _mas_opts_restore_plus( dirname, filename, args ) );
  va_end( args );
  return r;
}

#define XSTR(s) STR(s)
#define STR(s) #s
int
mas_opts_restore_zero( const char *filename )
{
  int r = 0;
  char *dir;

  dir = mas_strdup( XSTR( MAS_SYSCONFDIR ) );
  dir = mas_strcat_x( dir, "/" );
  if ( ctrl.binname )
    dir = mas_strcat_x( dir, ctrl.binname );
  else
    dir = mas_strcat_x( dir, "zocromas_uni" );
  /* {                                          */
  /*   char *p;                                 */
  /*   size_t l;                                */
  /*                                            */
  /*   l = strlen( dir );                       */
  /*   while ( ( p = strchr( dir + l, '_' ) ) ) */
  /*     *p = '/';                              */
  /* }                                          */
  IEVAL( r, _mas_opts_restore( dir, filename ) );
  mas_free( dir );
  return r;
}
