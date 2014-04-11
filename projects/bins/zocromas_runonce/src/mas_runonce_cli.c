/* #include <stdlib.h> */
#include <stdarg.h>
#include <string.h>
#include <getopt.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "mas_runonce_config_types.h"

#include "mas_runonce_cli.h"


size_t
runonce_cli_options( int argc, char *argv[], int *perrorcnt, roaction_t * roaction, size_t romax )
{
  int errorcnt = 0;
  int opt;
  int roseq = 0;

  /* -K
   * sawfish-client -e '(delete-window (get-window-by-name-re "..." ))'
   * -I
   * if 't' == sawfish-client -e '(if (get-window-by-name-re "...") t nil )'
   * */

  opterr = 0;
  /* printf( "@@ romax:%u\n", ( unsigned ) romax ); */
  while ( ( opt = getopt_long( argc, argv, "FVNDLZMSGKO1strlvwf:", NULL, NULL ) ) >= 0 && roseq < romax )
  {
    /* printf( "-- roseq:%u; romax:%u\n", ( unsigned ) roseq, ( unsigned ) romax ); */
    /* printf( "OPT %c %s\n", opt, optarg ); */
    switch ( opt )
    {
    case 'V':
      break;
      fprintf( stderr, "%s\n", MAS_CONFIG_ARGS );
    case 'l':
      roaction[roseq++] = ROACTION_LAUNCH;
      break;
    case 's':
      roaction[roseq++] = ROACTION_STOP;
      break;
    case 't':
      roaction[roseq++] = ROACTION_TOGGLE;
      break;
    case 'O':
      roaction[roseq++] = ROACTION_SHOW;
      break;
    case 'K':
      roaction[roseq++] = ROACTION_CLOSE;
      break;
    case 'r':
      roaction[roseq++] = ROACTION_RESTART;
      break;
    case 'G':
      configuration.flags.nosetgid = 1;
      break;
    case 'S':
      configuration.flags.strict = 1;
      break;
    case 'L':
      configuration.flags.list_zero = 1;
      configuration.flags.list_multiple = 1;
      configuration.flags.list_one = 1;
      roaction[roseq++] = ROACTION_LIST;
      break;
    case '1':
      configuration.flags.list_one = 1;
      roaction[roseq++] = ROACTION_LIST;
      break;
    case 'Z':
      configuration.flags.list_zero = 1;
      roaction[roseq++] = ROACTION_LIST;
      break;
    case 'M':
      configuration.flags.list_multiple = 1;
      roaction[roseq++] = ROACTION_LIST;
      break;
    case 'F':
      configuration.flags.force = 1;
      break;
    case 'D':
      configuration.flags.dry = 1;
      break;
    case 'v':
      configuration.flags.verbose++;
      break;
    case 'N':
      configuration.flags.noop = 1;
      break;
    case 'w':
      roaction[roseq++] = ROACTION_WINDOW;
      break;
    case 'f':
      if ( optarg )
        configuration.group_pattern = mas_strdup( optarg );
      break;
    case '?':
      printf( "Invalid option -- '%c'\n", optopt );
      errorcnt++;
      break;
    }
  }
  /* printf( "-- roseq:%u; romax:%u\n", ( unsigned ) roseq, ( unsigned ) romax ); */
  configuration.argvfrom = optind;
  if ( *perrorcnt )
    *perrorcnt = errorcnt;
  return roseq;
}
