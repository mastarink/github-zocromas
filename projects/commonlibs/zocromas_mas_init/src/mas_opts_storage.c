#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <stddef.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/types/mas_opts_types.h>
extern mas_options_t opts;

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
/* #include <mastar/log/mas_log.h> */

#include "mas_opts_common.h"
#include "mas_opts_storage.h"


/*
this:
  mas_opts_storage.c
related:
  mas_opts.c
  mas_opts_common.c
  mas_opts_types.h
  mas_opts_data.c
  mas_msg_tools.c
  mas_control.c

*/



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
