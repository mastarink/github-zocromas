/* #include <stdlib.h> */
#include <stdarg.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "mas_runonce_config_types.h"

#include "mas_runonce_config.h"
#include "mas_runonce_test.h"
#include "mas_runonce_action.h"
#include "mas_runonce_pid.h"
#include "mas_runonce_base.h"


#include "mas_runonce_window.h"
#include "mas_runonce_scan.h"
#include "mas_runonce_cli.h"

#include "mas_runonce.h"


int
runonce_roaction( char *group_patt, char *sect_patt, roaction_t roaction, runonce_flags_t flags )
{
  if ( flags.verbose > 1 )
    printf( "function %s\n", __func__ );
  /* printf( "group_patt:%s; sect_patt:%s (strict:%d)\n", group_patt, sect_patt, !flags.nostrict ); */
  switch ( roaction )
  {
  case ROACTION_LIST:
    runonce_list_pids( group_patt, NULL, flags );
    break;
  case ROACTION_WINDOW:
    {
      unsigned long client_list_size = 0;
      Window *client_list = NULL;

      client_list = get_client_list( &client_list_size );
      for ( int i = 0; i < client_list_size / sizeof( Window ); i++ )
      {
        char *title;
        char *class;
        char *role;
        char *icon_name;
        pid_t wpid = 0;

        wpid = get_window_pid( client_list[i] );
        title = get_window_title( client_list[i] );
        class = get_window_class( client_list[i] );
        role = get_window_role( client_list[i] );
        icon_name = get_window_icon_name( client_list[i] );


        printf( "\n(%d)\n         [%lx]\n-        [%s]\n  class: [%s]\n  role:  [%s]\n  icon:  [%s]\n  w.pid  [%u]\n", i,
                ( unsigned long ) client_list[i], title ? title : "-", class ? class : "-", role ? role : "-", icon_name ? icon_name : "-", wpid );
        mas_free( class );
        mas_free( role );
        mas_free( title );
      }
      mas_free( client_list );
      /* .....
       * client_msg( activate, "_NET_CLOSE_WINDOW", 0, 0, 0, 0, 0 );
       * .....
       * */
    }
    break;
  case ROACTION_STOP:
    runonce_scan( group_patt, sect_patt, flags, NULL, ROACTION_EXIT, ROACTION_TERMINATE, ROACTION_NONE );
    break;
  case ROACTION_RESTART:
    runonce_scan( group_patt, sect_patt, flags, NULL, ROACTION_EXIT, ROACTION_TERMINATE, ROACTION_NONE );
    runonce_pids_reset(  );
    runonce_scan( group_patt, sect_patt, flags, NULL, ROACTION_LAUNCH, ROACTION_NONE );
    break;
  default:
    if ( roaction )
      runonce_scan( group_patt, sect_patt, flags, NULL, roaction, ROACTION_NONE );
    break;
  }
  return 0;
}

int
main( int argc, char *argv[] )
{
  int errorcnt = 0;

  if ( configuration.flags.verbose > 4 )
    printf( "+ to create runonce\n" );
  runonce_create(  );
  if ( configuration.flags.verbose > 4 )
    printf( "+ to load config\n" );
  runonce_config_load(  );
  if ( configuration.flags.verbose > 4 )
    printf( "+ to cload config\n" );

  {
    roaction_t aroaction[512];
    size_t rocnt = 0;

    rocnt = runonce_cli_options( argc, argv, &errorcnt, aroaction, sizeof( aroaction ) / sizeof( aroaction[0] ) );
    if ( !errorcnt )
    {
      if ( configuration.flags.verbose > 1 )
        printf( "+ rocnt:%u\n", ( unsigned ) rocnt );
      for ( int roseq = 0; roseq < rocnt; roseq++ )
      {
        if ( configuration.flags.verbose > 1 )
          printf( "+ roseq:%u\n", ( unsigned ) roseq );
        /* runonce_test( argc, argv ); */
        if ( configuration.flags.verbose > 4 )
          printf( "[[ %s ]] %d:%d\n", argv[0], argc, configuration.argvfrom );
        runonce_get_pids( configuration.group_pattern, NULL, configuration.flags );
        if ( configuration.argvfrom < argc )
          for ( int ia = configuration.argvfrom; ia < argc; ia++ )
            runonce_roaction( configuration.group_pattern, argv[ia], aroaction[roseq], configuration.flags );
        else
          runonce_roaction( configuration.group_pattern, NULL, aroaction[roseq], configuration.flags );
      }
    }
    else
    {
      printf( "ERROR\n" );
    }
  }
  runonce_delete(  );
  return 0;
}
