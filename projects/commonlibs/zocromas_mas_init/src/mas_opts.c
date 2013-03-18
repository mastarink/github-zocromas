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

#include <mastar/variables/mas_variables.h>

#include "mas_opts.h"


/*
this:
  mas_opts.c
related:
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

  if ( opts.modsdir )
    mas_free( opts.modsdir );
  opts.modsdir = NULL;

  if ( opts.pidsdir )
    mas_free( opts.pidsdir );
  opts.pidsdir = NULL;

  if ( opts.msgfilename )
    mas_free( opts.msgfilename );
  opts.msgfilename = NULL;

  if ( opts.protodir )
    mas_free( opts.protodir );
  opts.protodir = NULL;

  if ( opts.logdir )
    mas_free( opts.logdir );
  opts.logdir = NULL;

  if ( opts.historydir )
    mas_free( opts.historydir );
  opts.historydir = NULL;

  if ( opts.postdir )
    mas_free( opts.postdir );
  opts.postdir = NULL;

  if ( opts.configdir )
    mas_free( opts.configdir );
  opts.configdir = NULL;

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

  if ( opts.configdir )
  {
    mas_free( opts.configdir );
    opts.configdir = NULL;
  }
  wdirname = opts.configdir;
  if ( !wdirname )
    wdirname = dirname ? mas_strdup( dirname ) : NULL;
  if ( !wdirname )
    wdirname = opts.configdir ? mas_strdup( opts.configdir ) : NULL;
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
  if ( opts.configdir && *opts.configdir && *( opts.configdir + strlen( opts.configdir ) - 1 ) != '/' )
  {
    wdirname = mas_strcat_x( wdirname, "/" );
  }
  opts.configdir = wdirname;
  /* FMSG( "setting opts.configdir:%s", opts.configdir ); */
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

  if ( !opts.configdir || !*opts.configdir )
    IEVALM( r, -1, "(%d)config dir not set", NULL );
  /* r = stat( opts.configdir, &dir_stat ); */
  IEVALM( r, stat( opts.configdir, &dir_stat ), "(%d)no configdir: '%s'", opts.configdir );
  /* if ( r < 0 )                                     */
  /* {                                                */
  /*   if ( errno == ENOENT )                         */
  /*   {                                              */
  /*     EMSG( "no configdir : %s", opts.configdir ); */
  /*   }                                              */
  /*   else                                           */
  /*   {                                              */
  /*     EMSG( "why? : %s", opts.configdir );         */
  /*   }                                              */
  /* }                                                */
  /* else                                             */
  /* {                                                */
  /* }                                                */
  return r;
}

static int
mas_opts_check_old_file( const char *fpath, int deep, int remove_ext, int backup, int overwrite )
{
  int r = 0;
  FILE *f = NULL;

  f = mas_fopen( fpath, "r" );
  if ( f /* exists old file */  )
  {
    r = -1;
    if ( overwrite )
    {
      r = -1;
      deep++;
      if ( f )
        mas_fclose( f );
      if ( backup )
      {
        char *fpath_bak;

        fpath_bak = mas_strdup( fpath );
        {
          char ext[64];

          if ( remove_ext )
          {
            char *p;

            p = fpath_bak + strlen( fpath_bak );
            while ( p > fpath_bak && *( p - 1 ) >= '0' && *( p - 1 ) <= '9' )
              p--;
            if ( *( p - 1 ) == '.' )
              p--;
            *p = 0;
          }
          sprintf( ext, ".%03d", deep );
          fpath_bak = mas_strcat_x( fpath_bak, ext );
        }
        if ( deep < opts.max_config_backup )
        {
          r = mas_opts_check_old_file( fpath_bak, deep, 1, backup, overwrite );
        }
        if ( r >= 0 )
        {
          r = rename( fpath, fpath_bak );
          if ( r < 0 )
          {
            P_ERR;
          }
        }
        mas_free( fpath_bak );
      }
    }
  }
  else
  {
    r = overwrite;
  }
  return r;
}

int
_mas_opts_save( const char *dirname, const char *filename, int backup, int overwrite )
{
  int r = 0;
  int rtot = -1;

  IEVALM( r, mas_opts_set_configdir( dirname ), "(%d)set config dir: '%s'", dirname );
  IEVALM( r, mas_opts_set_configfilename( filename ), "(%d)opts file:'%s'", filename );
  IEVALM( r, mas_opts_check_dir(  ), "(%d)config dir: '%s'", opts.configdir );
  if ( r == 0 )
  {
    char *fpath = NULL;

    fpath = mas_strdup( opts.configdir );
    fpath = mas_strcat_x( fpath, "/" );
    fpath = mas_strcat_x( fpath, opts.configfilename );
    if ( fpath )
    {
      if ( mas_opts_check_old_file( fpath, 0, 0, backup, overwrite ) == 0 )
      {
        FILE *f;

        f = mas_fopen( fpath, "w" );
        if ( f )
        {
          char outstr[128] = "#   ";

          rtot = 0;
          {
            strftime( outstr, sizeof( outstr ), "%a, %d %b %Y %T %z", mas_xlocaltime(  ) );
            IEVAL( r, fprintf( f, "# %s\n#\n", outstr ) );
            if ( r > 0 )
              rtot += r;
          }
          if ( opts.uuid )
          {
            IEVAL( r, fprintf( f, "uuid=%s\n", opts.uuid ) );
            if ( r > 0 )
              rtot += r;
          }

          {
            IEVAL( r, fprintf( f,
                               "# common\nenv_optsname=%s\nenv_hostname=%s\nhistorydir=%s\npostdir=%s\nlogdir=%s\nlog=%d\n"
                               "max_config_backup=%u\nmessages=%u\n"
                               "default_port=%u\nsave_user_opts=%u\nsave_user_opts_plus=%u\n" "restart_sleep=%lg\n"
                               "# -\n", opts.env_optsname, opts.env_hostname, opts.historydir, opts.postdir, opts.logdir,
                               !opts.nolog, opts.max_config_backup, !opts.nomessages, opts.default_port, opts.save_user_opts,
                               opts.save_user_opts_plus, opts.restart_sleep ) );
            if ( r > 0 )
              rtot += r;
          }
          if ( ctrl.is_server )
          {
            IEVAL( r, fprintf( f,
                               "# server\ndaemon=%u\nread_user_opts=%u\nread_user_opts_plus=%u\n"
                               "single_instance=%u\nsingle_child=%u\nlogger=%d\nmodsdir=%s\n" "pidsdir=%s\nprotodir=%s\n# -\n", ctrl.daemon,
                               opts.read_user_opts, opts.read_user_opts_plus, opts.single_instance, opts.single_child, !opts.nologger,
                               opts.modsdir, opts.pidsdir, opts.protodir ) );
            if ( r > 0 )
              rtot += r;
          }
          else if ( ctrl.is_client )
          {
            IEVAL( r, fprintf( f, "# client\ndisconnect_prompt=%u\nwait_server=%u\n# -\n", opts.disconnect_prompt, opts.wait_server ) );
            if ( r > 0 )
              rtot += r;
          }
          {
            {
              IEVAL( r, fprintf( f, "\n[%s %d]\n", ctrl.is_client ? "hosts" : "listen", opts.hostsv.c ) );
              if ( r > 0 )
                rtot += r;
            }

            if ( opts.hostsv.c )
            {
              for ( int ih = 0; ih < opts.hostsv.c; ih++ )
              {
                IEVAL( r, fprintf( f, "host=%s\n", opts.hostsv.v[ih] ) );
                if ( r > 0 )
                  rtot += r;
              }
            }
          }
          {
            {
              IEVAL( r, fprintf( f, "\n[%s %d]\n", "protos", opts.protosv.c ) );
              if ( r > 0 )
                rtot += r;
            }

            if ( opts.protosv.c )
            {
              for ( int ih = 0; ih < opts.protosv.c; ih++ )
              {
                IEVAL( r, fprintf( f, "proto=%s\n", opts.protosv.v[ih] ) );
                if ( r > 0 )
                  rtot += r;
              }
            }
          }
          if ( opts.commandsv.c )
          {
            IEVAL( r, fprintf( f, "\n[commands %d]\n", opts.commandsv.c ) );
            if ( r > 0 )
              rtot += r;
            for ( int ih = 0; ih < opts.commandsv.c; ih++ )
            {
              IEVAL( r, fprintf( f, "command=%s\n", opts.commandsv.v[ih] ) );
              if ( r > 0 )
                rtot += r;
            }
          }
          {
            mas_variable_t *var = NULL;
            char *vclass = NULL;
            const char *name = "docroot";

            while ( ( var = mas_variable_matching( ctrl.hostvars, var, vclass, name ) ) )
            {
              IEVAL( r, fprintf( f, "\n[host %s]\n", var->vclass ) );
              if ( r > 0 )
                rtot += r;
              IEVAL( r, fprintf( f, "%s=%s\n", var->name, var->value ) );
              if ( r > 0 )
                rtot += r;
              /* vclass = ...; */
              /* name = ...;   */
            }
          }
        }
        if ( f )
        {
          mas_fclose( f );
          f = NULL;
          ctrl.opts_saved = 1;
        }
      }
      mas_free( fpath );
    }
  }
  return rtot;
}

int
mas_opts_save_user( const char *dirname, const char *filename )
{
  int r = -1;

  if ( opts.save_user_opts )
  {
    MAS_LOG( "to save opts %s", filename );
    IEVAL( r, _mas_opts_save( dirname, filename, 1, opts.overwrite_user_opts ) );
    MAS_LOG( "saved opts : %d", r );
  }
  return r;
}

int
_mas_opts_save_plus( const char *dirname, const char *filename, int backup, int overwrite, va_list args )
{
  int r = 0;
  char *s = NULL;
  int x = 0;
  char *fn = NULL;

  fn = mas_strdup( filename );
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
    IEVAL( r, _mas_opts_save( dirname, fn, backup, overwrite ) );
  }
  else
  {
    IEVAL( r, -1 );
  }
  mas_free( fn );
  return r;
}

int
mas_opts_save_user_plus( const char *dirname, const char *filename, ... )
{
  int r = 0;
  va_list args;

  va_start( args, filename );
  if ( opts.save_user_opts_plus )
  {
    MAS_LOG( "to save opts plus %s", filename );
    IEVAL( r, _mas_opts_save_plus( dirname, filename, 1, opts.overwrite_user_opts_plus, args ) );
    MAS_LOG( "saved opts plus : %d", r );
  }
  /* else                                    */
  /* {                                       */
  MAS_LOG( "already saved opts plus" );
  /* }                                       */
  va_end( args );
  return r;
}

static void
mas_opts_set_strvalue( char *pstr, size_t max, const char *s )
{
  const char *se;

  se = mas_find_eq_value( s );
  if ( se && strlen( se ) < max )
  {
    strcpy( pstr, se );
  }
}

static void
mas_opts_set_pstrvalue( char **ppstr, const char *s )
{
  const char *se;

  se = mas_find_eq_value( s );
  if ( se )
  {
    if ( *ppstr )
      mas_free( *ppstr );
    *ppstr = se && ( 0 != strcmp( se, "(null)" ) ) ? mas_strdup( se ) : NULL;
  }
}

/* static void                                            */
/* mas_opts_set_unsigned( unsigned *pint, const char *s ) */
/* {                                                      */
/*   const char *se;                                      */
/*                                                        */
/*   se = mas_find_eq_value( s );                         */
/*   if ( se )                                            */
/*   {                                                    */
/*     sscanf( se, "%u", pint );                          */
/*   }                                                    */
/* }                                                      */

/* static void                                               */
/* mas_opts_set_unsigned_no( unsigned *pint, const char *s ) */
/* {                                                         */
/*   mas_opts_set_unsigned( pint, s );                       */
/*   *pint = !*pint;                                         */
/* }                                                         */

/* static void                                        */
/* mas_opts_set_double( double *pint, const char *s ) */
/* {                                                  */
/*   const char *se;                                  */
/*                                                    */
/*   se = mas_find_eq_value( s );                     */
/*   if ( se )                                        */
/*   {                                                */
/*     sscanf( se, "%lg", pint );                     */
/*   }                                                */
/* }                                                  */

static void
mas_opts_set_double_o( void *data, size_t off, const char *str )
{
  const char *se;
  double d;
  char *cptr;
  double *dptr;

  se = mas_find_eq_value( str );
  if ( se )
  {
    sscanf( se, "%lg", &d );
    cptr = ( ( char * ) data ) + off;
    dptr = ( double * ) cptr;
    *dptr = d;
  }
}

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

/* #define IFOPT1(n) if ( 0 == mas_strcmp2( s, #name "=" ) ) mas_opts_set_pstrvalue( (char*)(&opts) + offsetof(mas_options_t, name), sizeof( opts.name ), s ); */
#define OPT_STR(name,val) else if ( 0 == mas_strcmp2( val, #name "=" ) ) mas_opts_set_strvalue( opts.name, sizeof( opts.name ), val )
#define OPT_PSTR(name,val) else if ( 0 == mas_strcmp2( val, #name "=" ) ) mas_opts_set_pstrvalue( &opts.name, val )
#define OPT_FLAG(name,val) else if ( 0 == mas_strcmp2( val, #name "=" ) ) opts.name = ( mas_opts_atou(val) )
#define OPT_NOFLAG(name,val) else if ( 0 == mas_strcmp2( val, #name "=" ) ) opts.no##name = !( mas_opts_atou(val) )
#define OPT_DOUBLE(name,val) else if ( 0 == mas_strcmp2( val, #name "=" ) ) mas_opts_set_double_o( &opts, offsetof(mas_options_t, name), val )

int
mas_opts_restore_nosection( const char *s )
{
  int r = 0;

  /* unsigned v; */
  /* unsigned nv; */

  /* mas_opts_set_unsigned( &v, s ); */
  /* mas_opts_set_unsigned_no( &nv, s ); */
  /* nv = !v; */
  if ( 0 == mas_strcmp2( s, "message=" ) )
  {
    HMSG( "RESTORE OPTS: %s", mas_find_eq_value( s ) );
  }
  OPT_DOUBLE( restart_sleep, s );
  OPT_STR( env_optsname, s );
  OPT_STR( env_hostname, s );
  OPT_PSTR( uuid, s );
  OPT_PSTR( msgfilename, s );
  OPT_PSTR( modsdir, s );
  OPT_PSTR( pidsdir, s );
  OPT_PSTR( protodir, s );
  OPT_PSTR( logdir, s );
  OPT_PSTR( historydir, s );
  OPT_PSTR( postdir, s );
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
  else
  {
    IEVALM( r, -1, "(%d)opts :'%s'", s );
  }
  /* HMSG( "RESTORE OPT restart_sleep: %lg", opts.restart_sleep ); */
  return r;
}

int
_mas_opts_restore( const char *dirname, const char *filename )
{
  int r = 0;

  if ( opts.configdir )
  {
    mas_free( opts.configdir );
    opts.configdir = NULL;
  }
  IEVALM( r, mas_opts_set_configdir( dirname ), "(%d)set config dir: '%s'", dirname );
  IEVALM( r, mas_opts_set_configfilename( filename ), "(%d)opts file:'%s'", filename );
  IEVALM( r, mas_opts_check_dir(  ), "(%d)config dir: '%s'", opts.configdir );
  if ( r == 0 )
  {
    char *fpath = NULL;

    fpath = mas_strdup( opts.configdir );
    fpath = mas_strcat_x( fpath, "/" );
    fpath = mas_strcat_x( fpath, opts.configfilename );
    if ( fpath )
    {
      FILE *f;

      /* HMSG( "offsetof : %lu", ( unsigned long ) offsetof( mas_options_t, ticker_mode ) ); */
      HMSG( "OPTS from %s", fpath );
      f = mas_fopen( fpath, "r" );
      if ( f )
      {
        r = 0;
        char buf[4096];
        char *section = NULL;
        char *sectvalue = NULL;

        /* section=mas_strdup("DEFAULT"); */
        while ( !feof( f ) )
        {
          char *s, *rbp, *spp;
          size_t len;
          char *vp = NULL;

          s = fgets( buf, sizeof( buf ), f );
          /* mMSG( "read :'%s'", s ); */
          while ( s && *s && *s <= ' ' )
            s++;
          if ( !s || !*s || *s == '#' )
            continue;
          s = mas_chomp( s );
          if ( s && *s )
            r++;
          rbp = strchr( s, ']' );
          spp = strchr( s, ' ' );
          len = strlen( s );

          if ( s[0] == '[' && rbp && ( rbp - s == len - 1 ) )
          {
            if ( section )
              mas_free( section );
            section = NULL;
            if ( sectvalue )
              mas_free( sectvalue );
            sectvalue = NULL;
            if ( spp )
            {
              section = mas_strndup( s + 1, spp - s - 1 );
              sectvalue = mas_strndup( spp + 1, rbp - spp - 1 );
            }
            else if ( rbp )
              section = mas_strndup( s + 1, rbp - s - 1 );
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
            /* mMSG( "SECTION :'%s'", section ); */
            continue;
          }
          if ( !section )
          {
            IEVAL( r, mas_opts_restore_nosection( s ) );
          }
          else if ( 0 == mas_strcmp2( s, "host=" ) )
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
          else if ( 0 == mas_strcmpv( s, "docroot=", &vp ) && 0 == strcmp( section, "host" ) )
            ctrl.hostvars = mas_variable_create_text( ctrl.hostvars, MAS_THREAD_MASTER, sectvalue, "docroot", vp, 0 );
          /* for ( int ih = 0; ih < opts.commandsv.c; ih++ ) */
          /* {                                                */
          /*   mMSG( "command:%s", opts.commandsv.v[ih] );       */
          /* }                                                */
        }

        /* mMSG( "HOSTS :%d", opts.hostsv.c ); */

        /* mas_variables_writef( ctrl.hostvars, "docroot", "'%s' = '%s'\n", STDERR_FILENO ); */

        if ( section )
          mas_free( section );
        if ( sectvalue )
          mas_free( sectvalue );

	ctrl.loaded_optsv.c = mas_add_argv_args( ctrl.loaded_optsv.c, &ctrl.loaded_optsv.v, fpath, 0 );

        mas_fclose( f );
      }
      else
      {
        IEVALM( r, -1, "(%d)opts file:'%s'", fpath );
      }
    }
    else
    {
      IEVAL( r, -1 );
    }
    mas_free( fpath );
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
  if ( !opts.logdir )
  {
    opts.logdir = mas_strdup( MAS_BASE_DIR "/" MAS_LOG_DIR );
    HMSG( "DEF.LOGDIR: %s < '%s'", opts.logdir, MAS_BASE_DIR "/" MAS_LOG_DIR );
  }
#  endif
#endif
#ifdef MAS_PIDS_DIR
  if ( !opts.pidsdir )
  {
    opts.pidsdir = mas_strdup( MAS_BASE_DIR "/" MAS_PIDS_DIR );
    HMSG( "DEF.PIDSDIR: %s", opts.pidsdir );
  }
#endif


#define XSTR(s) STR(s)
#define STR(s) #s
#ifdef MAS_LIBDIR
#  ifdef MAS_MODS_DIR
  if ( !opts.modsdir )
  {
    opts.modsdir = mas_strdup( XSTR( MAS_LIBDIR ) "/" MAS_MODS_DIR );
    HMSG( "DEF.MODSDIR: %s", opts.modsdir );
  }
#  endif
#  ifdef MAS_PROTO_DIR
  if ( !opts.protodir )
  {
    opts.protodir = mas_strdup( XSTR( MAS_LIBDIR ) "/" MAS_PROTO_DIR );
    HMSG( "DEF.PROTODIR: %s", opts.protodir );
  }
#  endif
#endif
  return 0;
}
