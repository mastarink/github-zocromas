#include "mas_basic_def.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <uuid/uuid.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/tools/mas_tools.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

/* #include "mas_common.h" */
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
  mas_del_argv( opts.hosts_num, opts.hosts, 0 );
  opts.hosts_num = 0;
  opts.hosts = NULL;

  mas_del_argv( opts.protos_num, opts.protos, 0 );
  opts.protos_num = 0;
  opts.protos = NULL;

  mas_del_argv( opts.commands_num, opts.commands, 0 );
  opts.commands_num = 0;
  opts.commands = NULL;

  if ( opts.uuid )
    mas_free( opts.uuid );
  opts.uuid = NULL;

  if ( opts.modsdir )
    mas_free( opts.modsdir );
  opts.modsdir = NULL;

  if ( opts.msgfilename )
    mas_free( opts.msgfilename );
  opts.msgfilename = NULL;

  if ( opts.protodir )
    mas_free( opts.protodir );
  opts.modsdir = NULL;

  if ( opts.logdir )
    mas_free( opts.logdir );
  opts.logdir = NULL;

  if ( opts.configdir )
    mas_free( opts.configdir );
  opts.configdir = NULL;

  if ( opts.configfilename )
    mas_free( opts.configfilename );
  opts.configfilename = NULL;

  mas_del_argv( opts.argc, opts.argv, 0 );
  opts.argc = 0;
  opts.argv = NULL;
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
  {
    mas_free( opts.configfilename );
    opts.configfilename = NULL;
  }

  wfilename = opts.configfilename;
  if ( !wfilename )
    wfilename = filename ? mas_strdup( filename ) : NULL;
  if ( !wfilename )
    wfilename = opts.configfilename ? mas_strdup( opts.configfilename ) : NULL;
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

  r = stat( opts.configdir, &dir_stat );
  if ( r < 0 )
  {
    if ( errno == ENOENT )
    {
      EMSG( "no configdir : %s", opts.configdir );
    }
    else
    {
      EMSG( "why? : %s", opts.configdir );
    }
  }
  else
  {
  }
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
  int rtot = -1;

  mas_opts_set_configdir( dirname );
  mas_opts_set_configfilename( filename );
  if ( mas_opts_check_dir(  ) == 0 )
  {
    char *fpath = NULL;

    fpath = mas_strdup( opts.configdir );
    fpath = mas_strcat_x( fpath, "/" );
    fpath = mas_strcat_x( fpath, opts.configfilename );
    if ( mas_opts_check_old_file( fpath, 0, 0, backup, overwrite ) == 0 )
    {
      FILE *f;

      f = mas_fopen( fpath, "w" );
      if ( f )
      {
        int r;
        char outstr[128] = "#   ";

        rtot = 0;

        {
          strftime( outstr, sizeof( outstr ), "%a, %d %b %Y %T %z", mas_xlocaltime(  ) );
          r = fprintf( f, "# %s\n#\n", outstr );
          if ( r > 0 )
            rtot += r;
        }
        if ( opts.uuid )
        {
          r = fprintf( f, "uuid=%s\n", opts.uuid );
          if ( r > 0 )
            rtot += r;
        }

        {
          r = fprintf( f,
                       "env_optsname=%s\nenv_hostname=%s\nmodsdir=%s\nlogdir=%s\nlogger=%d\nlog=%d\n"
                       "max_config_backup=%u\ndaemon=%u\nmessages=%u\n"
                       "default_port=%u\nsave_opts=%u\nsave_opts_plus=%u\nrestart_sleep=%lg\n"
                       "disconnect_prompt=%u\nwait_server=%u\n", opts.env_optsname, opts.env_hostname, opts.modsdir, opts.logdir,
                       !opts.nologger, !opts.nolog, opts.max_config_backup, !opts.nodaemon, !opts.nomessages, opts.default_port,
                       opts.save_opts, opts.save_opts_plus, opts.restart_sleep, opts.disconnect_prompt, opts.wait_server );
          if ( r > 0 )
            rtot += r;
        }
        {
          {
            r = fprintf( f, "\n[%s %d]\n", ctrl.is_client ? "hosts" : "listen", opts.hosts_num );
            if ( r > 0 )
              rtot += r;
          }

          if ( opts.hosts_num )
          {
            for ( int ih = 0; ih < opts.hosts_num; ih++ )
            {
              int r;

              r = fprintf( f, "host=%s\n", opts.hosts[ih] );
              if ( r > 0 )
                rtot += r;
            }
          }
        }
        {
          {
            r = fprintf( f, "\n[%s %d]\n", "protos", opts.protos_num );
            if ( r > 0 )
              rtot += r;
          }

          if ( opts.protos_num )
          {
            for ( int ih = 0; ih < opts.protos_num; ih++ )
            {
              int r;

              r = fprintf( f, "proto=%s\n", opts.protos[ih] );
              if ( r > 0 )
                rtot += r;
            }
          }
        }
        if ( opts.commands_num )
        {
          r = fprintf( f, "\n[commands %d]\n", opts.commands_num );
          if ( r > 0 )
            rtot += r;
          for ( int ih = 0; ih < opts.commands_num; ih++ )
          {
            int r;

            r = fprintf( f, "command=%s\n", opts.commands[ih] );
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
            r = fprintf( f, "\n[host %s]\n", var->vclass );
            if ( r > 0 )
              rtot += r;
            r = fprintf( f, "%s=%s\n", var->name, var->value );
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
  return rtot;
}

int
mas_opts_save( const char *dirname, const char *filename )
{
  int r = -1;

  if ( opts.save_opts )
  {
    MAS_LOG( "to save opts %s", filename );
    r = _mas_opts_save( dirname, filename, 1, opts.save_opts );
    MAS_LOG( "saved opts : %d", r );
  }
  else
  {
    MAS_LOG( "already saved opts" );
  }
  return r;
}

int
_mas_opts_save_plus( const char *dirname, const char *filename, int backup, int overwrite, va_list args )
{
  int r = -1;
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
    r = _mas_opts_save( dirname, fn, backup, overwrite );
  }
  mas_free( fn );
  return r;
}

int
mas_opts_save_plus( const char *dirname, const char *filename, ... )
{
  int r = -1;
  va_list args;

  va_start( args, filename );
  /* if ( opts.save_opts_plus ) */
  {
    MAS_LOG( "to save opts plus %s", filename );
    r = _mas_opts_save_plus( dirname, filename, 1, opts.save_opts_plus, args );
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

static void
mas_opts_set_unsigned( unsigned *pint, const char *s )
{
  const char *se;

  se = mas_find_eq_value( s );
  if ( se )
  {
    sscanf( se, "%u", pint );
  }
}

static void
mas_opts_set_double( double *pint, const char *s )
{
  const char *se;

  se = mas_find_eq_value( s );
  if ( se )
  {
    sscanf( se, "%lg", pint );
  }
}

void
mas_opts_set_argv( int *pargc, char ***pargv, const char *s )
{
  const char *se;

  se = mas_find_eq_value( s );
  if ( se )
  {
    *pargc = mas_add_argv_arg( *pargc, pargv, se );
  }
}

void
mas_opts_add_command( const char *s )
{
  const char *se;

  se = mas_find_eq_value( s );
  opts.commands_num = mas_add_argv_arg( opts.commands_num, &opts.commands, se );
}

int
mas_opts_restore_nosection( const char *s )
{
  if ( 0 == mas_strcmp2( s, "env_optsname=" ) )
  {
    mas_opts_set_strvalue( opts.env_optsname, sizeof( opts.env_optsname ), s );
  }
  else if ( 0 == mas_strcmp2( s, "env_hostname=" ) )
  {
    mas_opts_set_strvalue( opts.env_hostname, sizeof( opts.env_hostname ), s );
  }
  else if ( 0 == mas_strcmp2( s, "uuid=" ) )
  {
    mas_opts_set_pstrvalue( &opts.uuid, s );
  }
  else if ( 0 == mas_strcmp2( s, "log=" ) )
  {
    unsigned v = 0;

    mas_opts_set_unsigned( &v, s );
    opts.nolog = !v;
  }
  else if ( 0 == mas_strcmp2( s, "logger=" ) )
  {
    unsigned v = 0;

    mas_opts_set_unsigned( &v, s );
    opts.nologger = !v;
    /* mMSG( "READ NOLOGGER :%d", opts.nologger ); */
  }
  else if ( 0 == mas_strcmp2( s, "modsdir=" ) )
  {
    mas_opts_set_pstrvalue( &opts.modsdir, s );
  }
  else if ( 0 == mas_strcmp2( s, "protodir=" ) )
  {
    mas_opts_set_pstrvalue( &opts.protodir, s );
  }
  else if ( 0 == mas_strcmp2( s, "logdir=" ) )
  {
    mas_opts_set_pstrvalue( &opts.logdir, s );
  }
  else if ( 0 == mas_strcmp2( s, "configdir=" ) )
  {
    mas_opts_set_pstrvalue( &opts.configdir, s );
  }
  else if ( 0 == mas_strcmp2( s, "configfilename=" ) )
  {
    mas_opts_set_pstrvalue( &opts.configfilename, s );
  }
  else if ( 0 == mas_strcmp2( s, "max_config_backup=" ) )
  {
    mas_opts_set_unsigned( &opts.max_config_backup, s );
  }
  else if ( 0 == mas_strcmp2( s, "default_port=" ) )
  {
    mas_opts_set_unsigned( &opts.default_port, s );
  }
  else if ( 0 == mas_strcmp2( s, "daemon=" ) )
  {
    unsigned v = 0;

    mas_opts_set_unsigned( &v, s );
    opts.nodaemon = !v;
  }
  else if ( 0 == mas_strcmp2( s, "messages=" ) )
  {
    unsigned v = 0;

    mas_opts_set_unsigned( &v, s );
    opts.nomessages = !v;
  }
  else if ( 0 == mas_strcmp2( s, "save_opts=" ) )
  {
    unsigned v = 0;

    mas_opts_set_unsigned( &v, s );
    opts.save_opts = v;
  }
  else if ( 0 == mas_strcmp2( s, "save_opts_plus=" ) )
  {
    unsigned v = 0;

    mas_opts_set_unsigned( &v, s );
    opts.save_opts_plus = v;
  }
  else if ( 0 == mas_strcmp2( s, "disconnect_prompt=" ) )
  {
    unsigned v = 0;

    mas_opts_set_unsigned( &v, s );
    opts.disconnect_prompt = v;
  }
  else if ( 0 == mas_strcmp2( s, "restart_sleep=" ) )
  {
    double v = 0;

    mas_opts_set_double( &v, s );
    opts.restart_sleep = v;
  }
  else if ( 0 == mas_strcmp2( s, "wait_server=" ) )
  {
    unsigned v = 0;

    mas_opts_set_unsigned( &v, s );
    opts.wait_server = v;
  }
  return 0;
}

int
_mas_opts_restore( const char *dirname, const char *filename )
{
  int r = -1;

  if ( opts.configdir )
  {
    mas_free( opts.configdir );
    opts.configdir = NULL;
  }
  mas_opts_set_configdir( dirname );
  if ( opts.configfilename )
  {
    mas_free( opts.configfilename );
    opts.configfilename = NULL;
  }
  mas_opts_set_configfilename( filename );
  if ( mas_opts_check_dir(  ) == 0 )
  {
    char *fpath = NULL;

    fpath = mas_strdup( opts.configdir );
    fpath = mas_strcat_x( fpath, "/" );
    fpath = mas_strcat_x( fpath, opts.configfilename );
    if ( fpath )
    {
      FILE *f;

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
            /* mMSG( "SECTION :'%s'", section ); */
            continue;
          }
          if ( !section )
            mas_opts_restore_nosection( s );
          else if ( 0 == mas_strcmp2( s, "host=" ) )
          {
            if ( 0 == strcmp( section, ctrl.is_client ? "hosts" : "listen" ) )
            {
              /* mMSG( "%d. +HOST :%s", opts.hosts_num, s ); */
              mas_opts_set_argv( &opts.hosts_num, &opts.hosts, s );
            }
            else
            {
              mMSG( "%d. HOST :%s @ [%s] %d", opts.hosts_num, s, section, ctrl.is_client );
            }
          }
          else if ( 0 == mas_strcmp2( s, "proto=" ) )
          {
            if ( 0 == strcmp( section, "protos" ) )
            {
              /* mMSG( "%d. +PROTO :%s", opts.protos_num, s ); */
              mas_opts_set_argv( &opts.protos_num, &opts.protos, s );
            }
            else
            {
              mMSG( "%d. PROTO :%s @ [%s]", opts.protos_num, s, section );
            }
          }
          else if ( 0 == mas_strcmp2( s, "command=" ) && 0 == strcmp( section, "commands" ) )
            mas_opts_add_command( s );
          else if ( 0 == mas_strcmpv( s, "docroot=", &vp ) && 0 == strcmp( section, "host" ) )
            ctrl.hostvars = mas_variable_create_text( ctrl.hostvars, MAS_THREAD_MASTER, sectvalue, "docroot", vp, 0 );
          /* for ( int ih = 0; ih < opts.commands_num; ih++ ) */
          /* {                                                */
          /*   mMSG( "command:%s", opts.commands[ih] );       */
          /* }                                                */
        }

        /* mMSG( "HOSTS :%d", opts.hosts_num ); */

        /* mas_variables_writef( ctrl.hostvars, "docroot", "'%s' = '%s'\n", STDERR_FILENO ); */

        if ( section )
          mas_free( section );
        if ( sectvalue )
          mas_free( sectvalue );
        mas_fclose( f );
      }
      else
      {
        r = -1;
      }
    }
    mas_free( fpath );
  }
  return r;
}

int
mas_opts_restore( const char *dirname, const char *filename )
{
  int r = 0;

  {
    char *fn;

    /* mMSG( "FILENAME: %s", filename ); */
    fn = mas_strdup( filename );
    r = _mas_opts_restore( dirname, fn );
    /* mMSG( "BBB: %s - %d", getenv( "MAS_PID_AT_BASHRC" ), r ); */
    mas_free( fn );
  }
  if ( r < 0 )
  {
  }
  if ( !opts.uuid )
  {
    uuid_t uuid;
    char buffer[256];

    memset( uuid, 0, sizeof( uuid ) );
    uuid_generate( uuid );
    uuid_unparse_lower( uuid, buffer );
    fprintf( stderr, "UUID %s\n", buffer );
    opts.uuid = mas_strdup( buffer );
  }

  return r;
}

int
_mas_opts_restore_plus( const char *dirname, const char *filename, va_list args )
{
  int r = -1;
  char *s = NULL;
  int x = 0;
  char *fn = NULL;

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
    HMSG( "OPTS:%s", fn );
    r = _mas_opts_restore( dirname, fn );
  }
  mas_free( fn );
  return r;
}

int
mas_opts_restore_plus( const char *dirname, const char *filename, ... )
{
  int r = -1;
  va_list args;

  va_start( args, filename );
  r = _mas_opts_restore_plus( dirname, filename, args );
  va_end( args );
  return r;
}
