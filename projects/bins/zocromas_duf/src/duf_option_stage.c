#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"


/* ###################################################################### */
#include "duf_option_stage.h"
/* ###################################################################### */

const char *
duf_optstage_name( duf_option_stage_t istage )
{
  static const char *tail[] = {
    [DUF_OPTION_STAGE_NONE] = "none",
    [DUF_OPTION_STAGE_PRESETUP] = "presetup",
    [DUF_OPTION_STAGE_SETUP] = "setup",
    [DUF_OPTION_STAGE_FIRST] = "first",
    [DUF_OPTION_STAGE_LOOP] = "loop",
    [DUF_OPTION_STAGE_INTERACTIVE] = "interactive",
    NULL
  };
  return ( istage >= DUF_OPTION_STAGE_MIN && istage <= DUF_OPTION_STAGE_MAX )
        || istage == DUF_OPTION_STAGE_NONE ? ( tail[istage] ? tail[istage] : "-?-" ) : ( istage == DUF_OPTION_STAGE_ANY ? "any" : "-" );
}

int
duf_optstage_check( duf_option_stage_t istage, const duf_longval_extended_t * extended, const duf_longval_extended_table_t * xtable )
{
  int r0 = 0;
  int r1 = 0;
  int r2 = 0;

  DUF_TRACE( options, +150, "checking stage(%s) xflag:%d xuse:%d xminmax:%d/%d", duf_optstage_name( istage ), extended->stage.flag,
             extended->use_stage, extended->stage.min, extended->stage.max );
  DUF_TRACE( options, +150, "checking stage(%s) tflag:%d tuse:%d tminmax:%d/%d", duf_optstage_name( istage ), xtable->stage.flag, xtable->use_stage,
             xtable->stage.min, xtable->stage.max );
  r0 = ( istage == DUF_OPTION_STAGE_ANY );
  r0 = r0 || extended->stage.flag;
  r1 = ( !extended->use_stage || ( extended->stage.min <= istage && extended->stage.max >= istage ) );
  r2 = ( !xtable || ( !xtable->use_stage || ( xtable->stage.min <= istage && xtable->stage.max >= istage ) ) );
  r0 = r0 || ( r1 && r2 );
  if ( ( extended->use_stage_mask && ( extended->stage_mask & ( 1 << istage ) ) )
       || ( xtable->use_stage_mask && ( xtable->stage_mask & ( 1 << istage ) ) ) )
  {
    r0 = 0;
  }

  DUF_TRACE( options, +150, "checked stage(%s); r0:%d", duf_optstage_name( istage ), r0 );
  return r0;
}

char *
duf_optstages_list( const duf_longval_extended_t * extended, const duf_longval_extended_table_t * xtable )
{
  char *s = NULL;

  for ( duf_option_stage_t istg = DUF_OPTION_STAGE_MIN; istg <= DUF_OPTION_STAGE_MAX; istg++ )
  {
    if ( duf_optstage_check( istg, extended, xtable ) )
      s = mas_strncat_x( s, duf_optstage_name( istg ), 1 );
    else
      s = mas_strcat_x( s, "_" );
  }
  return s;
}

 void
duf_optstage_print( int use_stage, int use_stage_mask, duf_limits_stage_t stage, unsigned long stage_mask, unsigned eol )
{
  if ( use_stage || use_stage_mask )
  {
    DUF_PRINTF( 0, ".  " );
    if ( use_stage )
    {
      DUF_PRINTF( 0, ".stage(%s(%d),%s(%d)) ", duf_optstage_name( stage.min ), stage.min, duf_optstage_name( stage.max ), stage.max );
    }
    if ( use_stage_mask )
    {
      unsigned long msk = stage_mask;
      unsigned n = 0;

      DUF_PRINTF( 0, ".mask(%lu; { ", stage_mask );
      while ( msk )
      {
        if ( msk & 1 )
        {
          DUF_PRINTF( 0, ".%s; ", duf_optstage_name( n ) );
        }
        n++;
        msk = msk >> 1;
      }
      DUF_PRINTF( 0, ".} )" );
    }
    if ( eol )
      DUF_PUTSL( 0 );
  }
}
