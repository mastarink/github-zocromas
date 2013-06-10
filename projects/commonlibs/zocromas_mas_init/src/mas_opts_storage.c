#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <stddef.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>



#include <mastar/types/mas_opts_types.h>



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
mas_opts_set_configdir( MAS_PASS_OPTS_DECLARE const char *dirname )
{
  MAS_PASS_OPTS_DECL_PREF;
  char *wdirname = NULL;

  if ( MAS_PASS_OPTS_PREF dir.config )
  {
    mas_free( MAS_PASS_OPTS_PREF dir.config );
    MAS_PASS_OPTS_PREF dir.config = NULL;
  }
  wdirname = MAS_PASS_OPTS_PREF dir.config;
  if ( !wdirname )
    wdirname = dirname ? mas_strdup( dirname ) : NULL;
  if ( !wdirname )
    wdirname = MAS_PASS_OPTS_PREF dir.config ? mas_strdup( MAS_PASS_OPTS_PREF dir.config ) : NULL;
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
  if ( MAS_PASS_OPTS_PREF dir.config && *MAS_PASS_OPTS_PREF dir.config
       && *( MAS_PASS_OPTS_PREF dir.config + strlen( MAS_PASS_OPTS_PREF dir.config ) - 1 ) != '/' )
  {
    wdirname = mas_strcat_x( wdirname, "/" );
  }
  MAS_PASS_OPTS_PREF dir.config = wdirname;

  /* FMSG( "setting MAS_PASS_OPTS_PREF dir.config:%s", MAS_PASS_OPTS_PREF dir.config ); */
  return 0;
}

int
mas_opts_set_configfilename( MAS_PASS_OPTS_DECLARE const char *filename )
{
  MAS_PASS_OPTS_DECL_PREF;
  char *wfilename = NULL;

  if ( MAS_PASS_OPTS_PREF configfilename )
    mas_free( MAS_PASS_OPTS_PREF configfilename );
  MAS_PASS_OPTS_PREF configfilename = NULL;

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
  MAS_PASS_OPTS_PREF configfilename = wfilename;

  /* FMSG( "setting MAS_PASS_OPTS_PREF configfilename:%s", MAS_PASS_OPTS_PREF configfilename ); */
  return 0;
}

int
mas_opts_check_dir( MAS_PASS_OPTS_DECLARE1 )
{
  MAS_PASS_OPTS_DECL_PREF;
  int r = 0;
  struct stat dir_stat;

  if ( !MAS_PASS_OPTS_PREF dir.config || !*MAS_PASS_OPTS_PREF dir.config )
    IEVALM( r, -1, "(%d)config dir not set", NULL );
  /* r = stat( MAS_PASS_OPTS_PREF dir.config, &dir_stat ); */
  IEVALM( r, stat( MAS_PASS_OPTS_PREF dir.config, &dir_stat ), "(%d)no configdir: '%s'", MAS_PASS_OPTS_PREF dir.config );
  /* if ( r < 0 )                                     */
  /* {                                                */
  /*   if ( errno == ENOENT )                         */
  /*   {                                              */
  /*     EMSG( "no configdir : %s", MAS_PASS_OPTS_PREF dir.config ); */
  /*   }                                              */
  /*   else                                           */
  /*   {                                              */
  /*     EMSG( "why? : %s", MAS_PASS_OPTS_PREF dir.config );         */
  /*   }                                              */
  /* }                                                */
  /* else                                             */
  /* {                                                */
  /* }                                                */
  return r;
}
