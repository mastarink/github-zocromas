#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <stddef.h>
#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>
#include <mastar/tools/mas_tools.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
/* #include <mastar/log/mas_log.h> */



#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>


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

mas_new_section_func_t __new_section_func = mas_opts_restore_new_section;
mas_at_section_func_t __at_section_func = mas_opts_restore_at_section;
mas_unknown_opt_func_t __unknown_opt_func = mas_opts_restore_flags;


static int
mas_opts_set_argv( int *pargc, char ***pargv, const char *s )
{
  EVAL_PREPARE;
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
mas_opts_add_command( mas_options_t * popts, const char *s )
{
  const char *se;

  se = mas_find_eq_value( s );
  popts->commandsv.c = mas_add_argv_arg( popts->commandsv.c, &popts->commandsv.v, se );
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


/*
  daemon_disable			:: --[no]daemon
  default_port				:: --port
  dir.history  				:: --historydir
  dir.log     				:: --logdir
  dir.mods				:: --modsdir
  dir.pids
  dir.post
  dir.proto
  disconnect_prompt
  env_hostname
  env_optsname
  log					:: --[no]log
  logger				:: --[no]logger
  max_config_backup
  messages  				:: --[no]messages
  msgfilename				:: --redirect-messages
  overwrite_user_opts
  overwrite_user_opts_plus
  read_user_opts			:: --[no]read-home-config
  read_user_opts_plus
  restart_sleep
  save_user_opts
  save_user_opts_plus
  single_child				:! --[no]multi-child
  single_instance			:! --[no]multi-instance
  thname.daemon_bunchi
  thname.daemon_bunchm
  thname.daemon_bunchx
  thname.daemon_masterm
  thname.daemon_masterth
  thname.daemon_masterthx
  thname.daemon_masterx
  thname.listen_close
  thname.listen_exit
  thname.listenin
  thname.listenm
  thname.listent
  thname.listenw
  thname.parent_bunchi
  thname.parent_bunchm
  thname.parent_bunchx
  thname.parent_masterm
  thname.parent_masterth
  thname.parent_masterthx
  thname.parent_masterx
  ticker				:: --[no]ticker
  uuid
  wait_server
  watcher				:: --[no]watcher
*/


mas_option_parse_t opt_table[] = {
  {.name = "env_optsname",.type = MAS_OPT_TYPE_STR,.offset = offsetof( mas_options_t, env_optsname ),.size =
   sizeof( ( ( mas_options_t * ) NULL )->env_optsname )}
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
  {.name = "thname.parent_bunchm",.type = MAS_OPT_TYPE_PSTR,.offset =
   offsetof( mas_options_t, thname ) + offsetof( mas_thnames_t, parent_bunchm ),.dv = "zocParBunch"}
  ,
  {.name = "thname.daemon_bunchm",.type = MAS_OPT_TYPE_PSTR,.offset =
   offsetof( mas_options_t, thname ) + offsetof( mas_thnames_t, daemon_bunchm ),.dv = "zocDaeBunch"}
  ,
  {.name = "thname.parent_bunchx",.type = MAS_OPT_TYPE_PSTR,.offset =
   offsetof( mas_options_t, thname ) + offsetof( mas_thnames_t, parent_bunchx ),.dv = "zocParBunchX"}
  ,
  {.name = "thname.daemon_bunchx",.type = MAS_OPT_TYPE_PSTR,.offset =
   offsetof( mas_options_t, thname ) + offsetof( mas_thnames_t, daemon_bunchx ),.dv = "zocDaeBunchX"}
  ,
  {.name = "thname.parent_bunchi",.type = MAS_OPT_TYPE_PSTR,.offset =
   offsetof( mas_options_t, thname ) + offsetof( mas_thnames_t, parent_bunchi ),.dv = "zocParBunchI"}
  ,
  {.name = "thname.daemon_bunchi",.type = MAS_OPT_TYPE_PSTR,.offset =
   offsetof( mas_options_t, thname ) + offsetof( mas_thnames_t, daemon_bunchi ),.dv = "zocDaeBunchI"}
  ,
  {.name = "thname.parent_masterm",.type = MAS_OPT_TYPE_PSTR,.offset =
   offsetof( mas_options_t, thname ) + offsetof( mas_thnames_t, parent_masterm ),.dv = "zocParMaster"}
  ,
  {.name = "thname.daemon_masterm",.type = MAS_OPT_TYPE_PSTR,.offset =
   offsetof( mas_options_t, thname ) + offsetof( mas_thnames_t, daemon_masterm ),.dv = "zocDaeMaster"}
  ,
  {.name = "thname.parent_masterx",.type = MAS_OPT_TYPE_PSTR,.offset =
   offsetof( mas_options_t, thname ) + offsetof( mas_thnames_t, parent_masterx ),.dv = "zocParMasterX"}
  ,
  {.name = "thname.daemon_masterx",.type = MAS_OPT_TYPE_PSTR,.offset =
   offsetof( mas_options_t, thname ) + offsetof( mas_thnames_t, daemon_masterx ),.dv = "zocDaeMasterX"}
  ,
  {.name = "thname.parent_masterth",.type = MAS_OPT_TYPE_PSTR,.offset =
   offsetof( mas_options_t, thname ) + offsetof( mas_thnames_t, parent_masterth ),.dv = "zocParMasterTH"}
  ,
  {.name = "thname.daemon_masterth",.type = MAS_OPT_TYPE_PSTR,.offset =
   offsetof( mas_options_t, thname ) + offsetof( mas_thnames_t, daemon_masterth ),.dv = "zocDaeMasterTH"}
  ,
  {.name = "thname.parent_masterthx",.type = MAS_OPT_TYPE_PSTR,.offset =
   offsetof( mas_options_t, thname ) + offsetof( mas_thnames_t, parent_masterthx ),.dv = "zocParMasterTHX"}
  ,
  {.name = "thname.daemon_masterthx",.type = MAS_OPT_TYPE_PSTR,.offset =
   offsetof( mas_options_t, thname ) + offsetof( mas_thnames_t, daemon_masterthx ),.dv = "zocDaeMasterTHX"}
  ,
  {.name = "thname.listenw",.type = MAS_OPT_TYPE_PSTR,.offset =
   offsetof( mas_options_t, thname ) + offsetof( mas_thnames_t, listenw ),.dv = "zocListenW"}
  ,
  {.name = "thname.listenin",.type = MAS_OPT_TYPE_PSTR,.offset =
   offsetof( mas_options_t, thname ) + offsetof( mas_thnames_t, listenin ),.dv = "zocListenIn"}
  ,
  {.name = "thname.listent",.type = MAS_OPT_TYPE_PSTR,.offset =
   offsetof( mas_options_t, thname ) + offsetof( mas_thnames_t, listent ),.dv = "zocListenT"}
  ,
  {.name = "thname.listenm",.type = MAS_OPT_TYPE_PSTR,.offset =
   offsetof( mas_options_t, thname ) + offsetof( mas_thnames_t, listenm ),.dv = "zocListenM"}
  ,
  {.name = "thname.listen_close",.type = MAS_OPT_TYPE_PSTR,.offset =
   offsetof( mas_options_t, thname ) + offsetof( mas_thnames_t, listen_close ),.dv = "zocListenClo"}
  ,
  {.name = "thname.listen_exit",.type = MAS_OPT_TYPE_PSTR,.offset =
   offsetof( mas_options_t, thname ) + offsetof( mas_thnames_t, listen_exit ),.dv = "zocListenXit"}
  ,
  {.name = "filename.message",.type = MAS_OPT_TYPE_PSTR,.offset = offsetof( mas_options_t, msgfilename )}
  ,
  {.name = "init-message-string",.type = MAS_OPT_TYPE_PSTR,.offset = offsetof( mas_options_t, init_message )}
  ,
  {.name = "filename.stderr",.type = MAS_OPT_TYPE_PSTR,.offset = offsetof( mas_options_t, stderr_filename )}
  ,
  {.name = "filename.stdout",.type = MAS_OPT_TYPE_PSTR,.offset = offsetof( mas_options_t, stdout_filename )}
  ,
  {.name = "user",.type = MAS_OPT_TYPE_PSTR,.offset = offsetof( mas_options_t, user )}
  ,
  {.name = "group",.type = MAS_OPT_TYPE_PSTR,.offset = offsetof( mas_options_t, group )}
  ,
  {.name = "restart_sleep",.type = MAS_OPT_TYPE_DOUBLE,.offset = offsetof( mas_options_t, restart_sleep )}
  ,
};

int
mas_opts_restore_flags( mas_options_t * popts, const char *s )
{
#define OPT_MATCH(oname,val) ( 0 == mas_strcmp2( val, #oname "=" ) )

/* #define OPT_XFLAGD(oname,fld,val)   else if ( OPT_MATCH( oname,val ) ) popts-> flag.name.fld = ( mas_opts_atou(val)?1:0 ) */
#define OPT_XFLAGD(oname,fld,val)   else if ( OPT_MATCH( oname,val ) ) OPT_SFLAG(popts, fld, mas_opts_atou(val))
#define OPT_XFLAG(oname,val)        OPT_XFLAGD( oname, oname, val )
#define OPT_FLAG(oname,val)         OPT_XFLAG( oname, val )

/* #define OPT_NOXFLAGD(oname,fld,val) else if ( OPT_MATCH( oname,val ) ) popts-> flag.name.fld = ( mas_opts_atou(val)?0:1 ) */
#define OPT_WOXFLAGD(oname,fld,val) else if ( OPT_MATCH( oname,val ) ) OPT_SFLAG(popts, fld, !mas_opts_atou(val))
#define OPT_WOXFLAG(oname,val)  OPT_WOXFLAGD(oname, oname, val)
#define OPT_WOFLAG(oname,val)       OPT_WOXFLAG( wo##oname, val )

#define OPT_XNUMD(oname,fld,val)   else if ( OPT_MATCH( oname,val ) ) popts-> fld = ( mas_opts_atou(val) )
#define OPT_XNUM(oname,val)        OPT_XNUMD( oname, oname, val )
#define OPT_NUM(oname,val)         OPT_XNUM( oname, val )

  /* don't remove this 'if' */
  if ( 0 /* 0 == mas_strcmp2( s, "message=" ) */  )
  {
    HMSG( "RESTORE OPTS: %s", mas_find_eq_value( s ) );
  }

  OPT_XFLAG( log_enable, s );
  OPT_XFLAG( log_run, s );
  OPT_WOFLAG( ticker, s );
  OPT_WOFLAG( watcher, s );
  OPT_NUM( max_config_backup, s );
  OPT_NUM( default_port, s );
  /* OPT_NOFLAG( daemon_disable, s ); */
  OPT_XFLAG( daemon_disable, s );
  OPT_XFLAG( daemon_sys, s );
  OPT_XFLAG( daemon_disable_redirect_std, s );
  OPT_XFLAG( daemon_disable_close_std, s );
  OPT_XFLAG( daemon_disable_setsid, s );
  OPT_XFLAG( daemon_disable_chdir, s );
  OPT_FLAG( read_user_opts, s );
  OPT_FLAG( read_user_opts_plus, s );
  OPT_FLAG( single_instance, s );
  OPT_FLAG( single_child, s );
  OPT_FLAG( messages, s );
  /* OPT_FLAG( has_init_message, s ); */
  OPT_FLAG( save_user_opts, s );
  OPT_FLAG( save_user_opts_plus, s );
  OPT_FLAG( overwrite_user_opts, s );
  OPT_FLAG( overwrite_user_opts_plus, s );
  OPT_FLAG( disconnect_prompt, s );
  OPT_FLAG( wait_server, s );
  return 0;
}

int
mas_opts_restore_new_section( mas_options_t * popts, const char *section )
{
  if ( popts->protosv.v && 0 == strcmp( section, "protos" ) )
  {
    mas_del_argv( popts->protosv.c, popts->protosv.v, 0 );
    popts->protosv.c = 0;
    popts->protosv.v = NULL;
  }
  else if ( popts->commandsv.v && 0 == strcmp( section, "commands" ) )
  {
    mas_del_argv( popts->commandsv.c, popts->commandsv.v, 0 );
    popts->commandsv.c = 0;
    popts->commandsv.v = NULL;
  }
  return 0;
}

int
mas_opts_restore_at_section( mas_options_t * popts, const char *section, const char *s )
{
  EVAL_PREPARE;
  CTRL_PREPARE;
  int r = 0;

  if ( 0 == mas_strcmp2( s, "host=" ) )
  {
    if ( 0 == strcmp( section, ctrl.is_client ? "hosts" : "listen" ) )
    {
      /* mMSG( "%d. +HOST :%s", popts-> hostsv.c, s ); */
      IEVAL( r, mas_opts_set_argv( &popts->hostsv.c, &popts->hostsv.v, s ) );
    }
    else
    {
      mMSG( "%d. HOST :%s @ [%s] %d", popts->hostsv.c, s, section, ctrl.is_client );
    }
  }
  else if ( 0 == mas_strcmp2( s, "proto=" ) )
  {
    if ( 0 == strcmp( section, "protos" ) )
    {
      /* mMSG( "%d. +PROTO :%s", popts-> protosv.c, s ); */
      IEVAL( r, mas_opts_set_argv( &popts->protosv.c, &popts->protosv.v, s ) );
    }
    else
    {
      mMSG( "%d. PROTO :%s @ [%s]", popts->protosv.c, s, section );
    }
  }
  else if ( 0 == mas_strcmp2( s, "command=" ) && 0 == strcmp( section, "commands" ) )
    mas_opts_add_command( popts, s );
  /* for ( int ih = 0; ih < popts-> commandsv.c; ih++ ) */
  /* {                                                */
  /*   mMSG( "command:%s", popts-> commandsv.v[ih] );       */
  /* }                                                */
  return r;
}

int
mas_opts_restore_relative( mas_options_t * popts, const char *filename )
{
  int r = 0;
  char *fpath = NULL;

  fpath = mas_strdup( popts->dir.config );
  fpath = mas_strcat_x( fpath, "/" );
  fpath = mas_strcat_x( fpath, filename );
  r = mas_opts_restore_path( popts, fpath );
  mas_free( fpath );
  return r;
}

int
mas_opts_restore_path( mas_options_t * popts, const char *fpath )
{
  CTRL_PREPARE;
  int r = 0;

  r = _mas_opts_restore_path( popts, fpath, popts, opt_table, sizeof( opt_table ) / sizeof( opt_table[0] ), NULL,
                              __new_section_func, __at_section_func, __unknown_opt_func );
  ctrl.loaded_optsv.c = mas_add_argv_args( ctrl.loaded_optsv.c, &ctrl.loaded_optsv.v, fpath, 0 );
  return r;
}

int
_mas_opts_restore( mas_options_t * popts, const char *dirname, const char *filename )
{
  EVAL_PREPARE;
  int r = 0;

  IEVALM( r, mas_opts_set_configdir( popts, dirname ), "(%d)set config dir: '%s'", dirname );
  IEVALM( r, mas_opts_set_configfilename( popts, filename ), "(%d)opts file:'%s'", filename );
  IEVALM( r, mas_opts_check_dir( popts ), "(%d)config dir: '%s'", popts->dir.config );
  if ( r == 0 )
  {
    mas_opts_restore_relative( popts, popts->configfilename );
    /* HMSG( "RESTORE OPT messages:%d; msg:%d; msg/main:%d;  msg/notice:%d; %lX", popts->flag.name.messages, MAS_CTRL_MESSAGES, */
    /*       MAS_MSG_BIT( msg_main ), MAS_MSG_BIT( msg_notice ), popts->msg_flag.bits );                                            */
    HMSG( "RESTORE OPT messages:%d; msg:%d; msg/main:%d;  msg/notice:%d; %X", OPT_QFLAG( popts, messages ), MAS_CTRL_MESSAGES,
          MAS_MSG_BIT( msg_main ), MAS_MSG_BIT( msg_notice ), popts->flag.name.msg.bits );
  }
  return r;
}

int
mas_opts_restore_user( mas_options_t * popts, const char *dirname, const char *filename )
{
  EVAL_PREPARE;
  int r = 0;

  if ( filename )
  {
    char *fn;

    /* mMSG( "FILENAME: %s", filename ); */
    fn = mas_strdup( filename );
    IEVAL( r, _mas_opts_restore( popts, dirname, fn ) );
    /* mMSG( "BBB: %s - %d", getenv( "MAS_PID_AT_BASHRC" ), r ); */
    mas_free( fn );
  }
  return r;
}

int
_mas_opts_restore_plus( mas_options_t * popts, const char *dirname, const char *filename, va_list args )
{
  EVAL_PREPARE;
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
      IEVAL( r, _mas_opts_restore( popts, dirname, fn ) );
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
mas_opts_restore_user_plus( mas_options_t * popts, const char *dirname, const char *filename, ... )
{
  EVAL_PREPARE;
  int r = 0;
  va_list args;

  va_start( args, filename );
  IEVAL( r, _mas_opts_restore_plus( popts, dirname, filename, args ) );
  va_end( args );
  return r;
}

int
mas_opts_restore_zero( mas_options_t * popts, const char *filename )
{
  CTRL_PREPARE;
  int r = 0;
  char *dir;

#define XSTR(s) STR(s)
#define STR(s) #s
  dir = mas_strdup( XSTR( MAS_SYSCONFDIR ) );
  dir = mas_strcat_x( dir, "/" );
  if ( ctrl.exename )
    dir = mas_strcat_x( dir, ctrl.exename );
  else if ( ctrl.argvname )     /* ? */
    dir = mas_strcat_x( dir, ctrl.argvname );
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
  IEVAL( r, _mas_opts_restore( popts, dir, filename ) );
  mas_free( dir );
  return r;
}

__attribute__ ( ( constructor( 3004 ) ) )
     static void mas_constructor( void )
{
  /* fprintf( stderr, "******************** CONSTRUCTOr %s e%d\n", __FILE__, errno ); */
  mas_common_constructor( IL, 1 );
}
