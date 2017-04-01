#include <stddef.h>
#include <stdlib.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib.h>

#include <mastar/tools/mas_tools.h>
#include <mastar/tools/mas_time.h>
#include <mastar/tools/mas_arg_tools.h>




#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/log/mas_log.h>

#include "mas_opts_common.h"
#include "mas_opts_storage.h"
/* #include "mas_opts.h" */

#include "mas_opts_save.h"

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
mas_opts_check_old_file( mas_options_t * popts, const char *fpath, int deep, int remove_ext, int backup, int overwrite )
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
        if ( deep < (int)popts->max_config_backup )
        {
          r = mas_opts_check_old_file( popts, fpath_bak, deep, 1, backup, overwrite );
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
_mas_opts_save( mas_options_t * popts, const char *dirname, const char *filename, int backup, int overwrite )
{
  EVAL_PREPARE;
  int r = 0;
  int rtot = -1;

  IEVALM( r, mas_opts_set_configdir( popts, dirname ), "(%d)set config dir: '%s'", dirname );
  IEVALM( r, mas_opts_set_configfilename( popts, filename ), "(%d)opts file:'%s'", filename );
  IEVALM( r, mas_opts_check_dir( popts ), "(%d)config dir: '%s'", popts->dir.config );
  if ( r == 0 )
  {
    char *fpath = NULL;

    fpath = mas_strdup( popts->dir.config );
    fpath = mas_strcat_x( fpath, "/" );
    fpath = mas_strcat_x( fpath, popts->configfilename );
    if ( fpath )
    {
      if ( mas_opts_check_old_file( popts, fpath, 0, 0, backup, overwrite ) == 0 )
      {
        FILE *f;

        f = mas_fopen( fpath, "w" );
        HMSG( "WRITING OPTS %s", fpath );
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
          if ( popts->uuid )
          {
            IEVAL( r, fprintf( f, "uuid=%s\n", popts->uuid ) );
            if ( r > 0 )
              rtot += r;
          }

          {
            IEVAL( r, fprintf( f,
                               "# common\nenv_optsname=%s\nenv_hostname=%s\nhistorydir=%s\npostdir=%s\nlogdir=%s\nlog=%d\n"
                               "max_config_backup=%u\nmessages=%u\n"
                               "default_port=%u\nsave_user_opts=%u\nsave_user_opts_plus=%u\n" "restart_sleep=%lg\n"
                               "# -\n", popts->env_optsname, popts->env_hostname, popts->dir.history,
                               popts->dir.post, popts->dir.log, OPT_QFLAG( popts, log_enable ),
                               popts->max_config_backup, OPT_QFLAG( popts, messages ), popts->default_port,
                               OPT_QFLAG( popts, save_user_opts ), OPT_QFLAG( popts, save_user_opts_plus ), popts->restart_sleep ) );
            if ( r > 0 )
              rtot += r;
          }
          if ( ctrl.is_server )
          {
            IEVAL( r,
                   fprintf( f,
                            "# server\ndaemon=%u\nread_user_opts=%u\nread_user_opts_plus=%u\n"
                            "single_instance=%u\nsingle_child=%u\nlogger=%d\nticker=%d\nwatcher=%d\nmodsdir=%s\n"
                            "pidsdir=%s\nprotodir=%s\n# -\n", ctrl.daemon, OPT_QFLAG( popts, read_user_opts ),
                            OPT_QFLAG( popts, read_user_opts_plus ), OPT_QFLAG( popts, single_instance ), OPT_QFLAG( popts, single_child ),
                            OPT_QFLAG( popts, log_run ), !OPT_QFLAG( popts, woticker ), !OPT_QFLAG( popts, wowatcher ), popts->dir.mods,
                            popts->dir.pids, popts->dir.proto ) );
            if ( r > 0 )
              rtot += r;
          }
          else if ( ctrl.is_client )
          {
            IEVAL( r,
                   fprintf( f, "# client\ndisconnect_prompt=%u\nwait_server=%u\n# -\n", OPT_QFLAG( popts, disconnect_prompt ),
                            OPT_QFLAG( popts, wait_server ) ) );
            if ( r > 0 )
              rtot += r;
          }
          {
            {
              IEVAL( r, fprintf( f, "\n[%s %d]\n", ctrl.is_client ? "hosts" : "listen", popts->hostsv.c ) );
              if ( r > 0 )
                rtot += r;
            }

            if ( popts->hostsv.c )
            {
              for ( int ih = 0; ih < popts->hostsv.c; ih++ )
              {
                IEVAL( r, fprintf( f, "host=%s\n", popts->hostsv.v[ih] ) );
                if ( r > 0 )
                  rtot += r;
              }
            }
          }
          {
            {
              IEVAL( r, fprintf( f, "\n[%s %d]\n", "protos", popts->protosv.c ) );
              if ( r > 0 )
                rtot += r;
            }

            if ( popts->protosv.c )
            {
              for ( int ih = 0; ih < popts->protosv.c; ih++ )
              {
                IEVAL( r, fprintf( f, "proto=%s\n", popts->protosv.v[ih] ) );
                if ( r > 0 )
                  rtot += r;
              }
            }
          }
          if ( popts->commandsv.c )
          {
            IEVAL( r, fprintf( f, "\n[commands %d]\n", popts->commandsv.c ) );
            if ( r > 0 )
              rtot += r;
            for ( int ih = 0; ih < popts->commandsv.c; ih++ )
            {
              IEVAL( r, fprintf( f, "command=%s\n", popts->commandsv.v[ih] ) );
              if ( r > 0 )
                rtot += r;
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
      else
      {
        HMSG( "UNABLE TO WRITE OPTS %s", fpath );
      }
      mas_free( fpath );
    }
  }
  HMSG( "(%d)_SAVE PLUS", rtot );
  return rtot;
}

int
mas_opts_save_user( mas_options_t * popts, const char *dirname, const char *filename )
{
  int r = -__LINE__;

  if ( OPT_QFLAG( popts, save_user_opts ) )
  {
    r = 0;
    EVAL_PREPARE;
    MAS_LOG( "to save opts %s", filename );
    IEVAL( r, _mas_opts_save( popts, dirname, filename, 1, OPT_QFLAG( popts, overwrite_user_opts ) ) );
    MAS_LOG( "saved opts : %d", r );
  }
  else
  {
    r = 0;
    MAS_LOG( "disabled save opts" );
    HMSG( "disabled save opts" );
  }
  HMSG( "(%d)SAVED OPTS", r );
  return r;
}

int
_mas_opts_save_plus( mas_options_t * popts, const char *dirname, const char *filename, int backup, int overwrite, va_list args )
{
  EVAL_PREPARE;
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
    r = 0;
    IEVAL( r, _mas_opts_save( popts, dirname, fn, backup, overwrite ) );
  }
  HMSG( "(%d)_SAVE PLUS", r );
  mas_free( fn );
  return r;
}

int
mas_opts_save_user_plus( mas_options_t * popts, const char *dirname, const char *filename, ... )
{
  EVAL_PREPARE;
  int r = 0;
  va_list args;

  va_start( args, filename );
  if ( OPT_QFLAG( popts, save_user_opts_plus ) )
  {
    MAS_LOG( "to save opts plus %s", filename );
    IEVAL( r, _mas_opts_save_plus( popts, dirname, filename, 1, OPT_QFLAG( popts, overwrite_user_opts_plus ), args ) );
    MAS_LOG( "saved opts plus : %d", r );
  }
  else
  {
    r = 0;
    MAS_LOG( "disabled save opts plus" );
    HMSG( "disabled save opts plus" );
  }
  va_end( args );
  HMSG( "(%d)SAVED OPTS PLUS", r );
  return r;
}

__attribute__ ( ( constructor( 3005 ) ) )
     static void mas_constructor( void )
{
  /* fprintf( stderr, "******************** CONSTRUCTOr %s e%d\n", __FILE__, errno ); */
  mas_common_constructor( IL, 1 );
}
