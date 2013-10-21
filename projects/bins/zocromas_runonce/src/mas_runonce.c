/* #include <stdlib.h> */
#include <stdarg.h>
#include <string.h>
#include <getopt.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "mas_runonce_config_types.h"

#include "mas_runonce_config.h"
#include "mas_runonce_test.h"
#include "mas_runonce_action.h"
#include "mas_runonce_pid.h"
#include "mas_runonce_base.h"


#include "mas_runonce.h"


int
runonce_scan( const char *grppatt, const char *sectpatt, runonce_flags_t flags, char *msg, ... )
{
  va_list args;

  /* char *action = NULL; */

  va_start( args, msg );
  for ( int ngr = 0; ngr < configuration.numgroups; ngr++ )
  {
    config_group_t *grp = NULL;

    grp = configuration.groups + ngr;
    /* printf( "ngr:%d\n", ngr ); */
    if ( !grppatt || strstr( grp->name, grppatt ) )
    {
      for ( int nsec = 0; nsec < grp->num_sections; nsec++ )
      {
        config_section_t *sect = grp->sections + nsec;

        if ( sect->name && sect->name[0] != '@' )
        {
          if ( !sectpatt || ( flags.strict && 0 == strcmp( sectpatt, sect->name ) ) || ( !flags.strict && strstr( sect->name, sectpatt ) ) )
          {
            int cnt = 0;
            va_list ids;
            int done = 0;

            if ( flags.verbose > 4 )
              printf( "(strict:%d) #sect:%d %s ? %s\n", flags.strict, nsec, sectpatt, sect->name );

            va_copy( ids, args );
            while ( !done )
            {
              roaction_t id;

              id = va_arg( ids, roaction_t );

              /* printf( "id:%d\n", id ); */

              if ( id == ROACTION_NONE )
                break;
              /* if ( grp->sections && *sect->name != '@' ) */
              {
                switch ( id )
                {
                case ROACTION_LAUNCH:
                  done = runonce_launch( grp, sect, flags );
                  break;
                case ROACTION_TERMINATE:
                  done = runonce_terminate( grp, sect, flags );
                  break;
                case ROACTION_EXIT:
                  done = runonce_exit( grp, sect, flags );
                  break;
                case ROACTION_STOP:
                case ROACTION_RESTART:
                case ROACTION_NONE:
                  printf( "NONE %14s : %18s\n", grp->name, sect->name );
                  break;
                }
              }
              /* printf( "-------------------------------id:%d done:%d; cnt:%d\n", id, done, cnt ); */
              cnt++;
            }
            if ( !done )
            {
              printf( "nothing can be done for %s\n", sect->name );
            }
            va_end( ids );
          }
          else
          {
            /* printf( "didn't match section %s (strict:%d; sectpatt:%s)\n", sect->name, flags.strict, sectpatt ); */
          }
        }
      }
    }
    else
    {
      /* printf( "didn't match group %s (strict:%d; grppatt:%s)\n", grp->name, flags.strict, grppatt ); */
    }
  }
  va_end( args );
  /* return action; */
  return 0;
}


int
runonce_roaction( char *group_patt, char *sect_patt, roaction_t roaction, runonce_flags_t flags )
{
  /* printf( "group_patt:%s; sect_patt:%s (strict:%d)\n", group_patt, sect_patt, flags.strict ); */
  switch ( roaction )
  {
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
  char *group_patt = NULL;
  roaction_t roaction;
  runonce_flags_t flags;

  runonce_create(  );
  runonce_config_load(  );

  {
    int opt;

    /* -K
     * sawfish-client -e '(delete-window (get-window-by-name-re "..." ))'
     * -I
     * if 't' == sawfish-client -e '(if (get-window-by-name-re "...") t nil )'
     * */

    opterr = 0;
    while ( ( opt = getopt_long( argc, argv, "NDLZMSG1srlvf:", NULL, NULL ) ) >= 0 )
    {
      /* printf( "OPT %c %s\n", opt, optarg ); */
      switch ( opt )
      {
      case 'l':
        roaction = ROACTION_LAUNCH;
        break;
      case 's':
        roaction = ROACTION_STOP;
        break;
      case 'r':
        roaction = ROACTION_RESTART;
        break;
      case 'G':
        flags.nosetgid = 1;
        break;
      case 'S':
        flags.strict = 1;
        break;
      case 'L':
        flags.list_zero = 1;
        flags.list_multiple = 1;
        flags.list_one = 1;
        break;
      case '1':
        flags.list_one = 1;
        break;
      case 'Z':
        flags.list_zero = 1;
        break;
      case 'M':
        flags.list_multiple = 1;
        break;
      case 'D':
        flags.dry = 1;
        break;
      case 'v':
        flags.verbose++;
        break;
      case 'N':
        flags.noop = 1;
        break;
      case 'f':
        if ( optarg )
          group_patt = mas_strdup( optarg );
        break;
      case '?':
        printf( "Invalid option -- '%c'\n", optopt );
        errorcnt++;
        break;
      }
    }
    /* printf( "ZERO:%d ONE:%d MULT:%d\n", flags.list_zero, flags.list_one, flags.list_multiple ); */
    if ( !errorcnt )
    {
      /* runonce_test( argc, argv ); */
      /* printf( "[[ %s ]] %d:%d\n", argv[0], argc, optind ); */
      runonce_get_pids( group_patt, NULL, flags );
      if ( optind >= argc )
      {
        runonce_roaction( group_patt, NULL, roaction, flags );
      }
      else
      {
        for ( int ia = optind; ia < argc; ia++ )
        {
          runonce_roaction( group_patt, argv[ia], roaction, flags );
        }
      }
    }
    else
    {
      printf( "ERROR\n" );
    }
  }
  mas_free( group_patt );
  runonce_delete(  );
#ifdef MAS_TRACEMEM
  print_memlist( FL, stderr );
#endif

  /* sleep( 600 ); */
  return 0;
}
