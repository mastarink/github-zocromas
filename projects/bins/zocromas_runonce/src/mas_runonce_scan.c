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


#include "mas_runonce_window.h"


#include "mas_runonce_scan.h"



int
runonce_section_vscan( config_group_t * grp, config_section_t * sect, int nsec, runonce_flags_t flags, char *msg, va_list args )
{
  int cnt = 0;
  va_list ids;
  int done = 0;

  if ( flags.verbose )
    printf( "function %s\n", __func__ );

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
      case ROACTION_SHOW:
        done = runonce_show( grp, sect, nsec, flags );
        break;
      case ROACTION_CLOSE:
        done = runonce_close( grp, sect, nsec, flags );
        break;
      case ROACTION_LAUNCH:
        done = runonce_launch( grp, sect, nsec, flags );
        break;
      case ROACTION_TERMINATE:
        done = runonce_terminate( grp, sect, nsec, flags );
        break;
      case ROACTION_TOGGLE:
        done = runonce_toggle( grp, sect, nsec, flags );
        break;
      case ROACTION_EXIT:
        done = runonce_exit( grp, sect, nsec, flags );
        break;
        /* case ROACTION_STOP:                                      */
        /* case ROACTION_RESTART:                                   */
        /* case ROACTION_WINDOW:                                    */
        /* case ROACTION_NONE:                                      */
        /*   printf( "NONE %14s : %18s\n", grp->name, sect->name ); */
      default:
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
  return 0;
}

int
runonce_group_vscan( config_group_t * grp, const char *sectpatt, int *pnsecg, runonce_flags_t flags, char *msg, va_list args )
{
  int nsec = 0;

  if ( flags.verbose )
    printf( "function %s\n", __func__ );
  for ( nsec = 0; nsec < grp->num_sections; nsec++ )
  {
    config_section_t *sect = grp->sections + nsec;

    if ( sect->name && sect->name[0] != '@' )
    {
      if ( !sectpatt || ( flags.strict && 0 == strcmp( sectpatt, sect->name ) ) || ( !flags.strict && strstr( sect->name, sectpatt ) ) )
      {
        int secseq = nsec;

        if ( pnsecg )
          secseq = *pnsecg;
        if ( flags.verbose > 4 )
          printf( "(strict:%d) #sect:%d %s ? %s\n", flags.strict, nsec, sectpatt, sect->name );
        runonce_section_vscan( grp, sect, secseq, flags, msg, args );
        if ( pnsecg )
          ( *pnsecg )++;
      }
      else
      {
        /* printf( "didn't match section %s (strict:%d; sectpatt:%s)\n", sect->name, flags.strict, sectpatt ); */
      }
    }
  }

  return 0;
}

int
runonce_vscan( const char *grppatt, const char *sectpatt, runonce_flags_t flags, char *msg, va_list args )
{
  int nsecg = 0;

  if ( flags.verbose )
    printf( "function %s\n", __func__ );
  for ( int ngr = 0; ngr < configuration.numgroups; ngr++ )
  {
    config_group_t *grp = NULL;

    grp = configuration.groups + ngr;
    /* printf( "ngr:%d\n", ngr ); */
    if ( !grppatt || strstr( grp->name, grppatt ) )
    {
      runonce_group_vscan( grp, sectpatt, &nsecg, flags, msg, args );
    }
    else
    {
      /* printf( "didn't match group %s (strict:%d; grppatt:%s)\n", grp->name, flags.strict, grppatt ); */
    }
  }
  return 0;
}

int
runonce_scan( const char *grppatt, const char *sectpatt, runonce_flags_t flags, char *msg, ... )
{
  va_list args;

  if ( flags.verbose )
    printf( "function %s\n", __func__ );
  va_start( args, msg );
  runonce_vscan( grppatt, sectpatt, flags, msg, args );
  va_end( args );
  return 0;
}
