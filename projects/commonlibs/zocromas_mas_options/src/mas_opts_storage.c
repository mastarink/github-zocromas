#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>


#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
/* #include <mastar/log/mas_log.h> */

#include <mastar/types/mas_opts_types.h>
#include <mastar/types/mas_control_types.h>

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
mas_opts_set_configdir( mas_options_t * popts, const char *dirname )
{
  char *wdirname = NULL;

  if ( popts->dir.config )
  {
    mas_free( popts->dir.config );
    popts->dir.config = NULL;
  }
  wdirname = popts->dir.config;
  if ( !wdirname )
    wdirname = dirname ? mas_strdup( dirname ) : NULL;
  if ( !wdirname )
    wdirname = popts->dir.config ? mas_strdup( popts->dir.config ) : NULL;
  if ( !wdirname )
  {
    char *t;

    t = getenv( "MAS_ZOCROMAS_CONFIGDIR" );
    if ( t )
      wdirname = mas_strdup( t );
  }
  if ( !wdirname )
  {
    char *h;

    h = getenv( "HOME" );
    if ( h )
    {
      wdirname = mas_strdup( h );
      wdirname = mas_strcat_x( wdirname, "/.zocromas/" );
    }
  }
  if ( wdirname && popts->dir.config && *popts->dir.config && *( popts->dir.config + strlen( popts->dir.config ) - 1 ) != '/' )
  {
    wdirname = mas_strcat_x( wdirname, "/" );
  }
  popts->dir.config = wdirname;

  /* FMSG( "setting popts-> dir.config:%s", popts-> dir.config ); */
  return 0;
}

int
mas_opts_set_configfilename( mas_options_t * popts, const char *filename )
{
  char *wfilename = NULL;

  if ( popts->configfilename )
    mas_free( popts->configfilename );
  popts->configfilename = NULL;

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
  popts->configfilename = wfilename;

  /* FMSG( "setting popts-> configfilename:%s", popts-> configfilename ); */
  return 0;
}

int
mas_opts_check_dir( mas_options_t * popts )
{
  EVAL_PREPARE;
  int r = 0;
  struct stat dir_stat;

  if ( !popts->dir.config || !*popts->dir.config )
    IEVALM( r, -1, "(%d)config dir not set", NULL );
  /* r = stat( popts-> dir.config, &dir_stat ); */
  IEVALM( r, stat( popts->dir.config, &dir_stat ), "(%d)no configdir: '%s'", popts->dir.config );
  /* if ( r < 0 )                                     */
  /* {                                                */
  /*   if ( errno == ENOENT )                         */
  /*   {                                              */
  /*     EMSG( "no configdir : %s", popts-> dir.config ); */
  /*   }                                              */
  /*   else                                           */
  /*   {                                              */
  /*     EMSG( "why? : %s", popts-> dir.config );         */
  /*   }                                              */
  /* }                                                */
  /* else                                             */
  /* {                                                */
  /* }                                                */
  return r;
}

__attribute__ ( ( constructor( 3003 ) ) )
     static void f_constructor( void )
{
  if ( stderr )
    fprintf( stderr, "******************** CONSTRUCTOR %s e%d\n", __FILE__, errno );
}
