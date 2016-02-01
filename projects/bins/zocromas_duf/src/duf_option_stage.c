#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"

#include "duf_config.h"

/* ###################################################################### */
#include "duf_option_stage.h"
/* ###################################################################### */

const char *
duf_optstage_name( duf_option_stage_t istage )
{
  static const char *tail[] = {
    [DUF_OPTION_STAGE_NONE] = "none",
    [DUF_OPTION_STAGE_BOOT] = "boot",
    [DUF_OPTION_STAGE_DEBUG] = "debug",
    [DUF_OPTION_STAGE_PRESETUP] = "presetup",
    [DUF_OPTION_STAGE_SETUP] = "setup",
    [DUF_OPTION_STAGE_FIRST] = "first",
    [DUF_OPTION_STAGE_LOOP] = "loop",
    [DUF_OPTION_STAGE_INTERACTIVE] = "interactive",
    NULL
  };
  return ( istage >= DUF_OPTION_STAGE_MIN && istage <= DUF_OPTION_STAGE_MAX )
        /* || istage == DUF_OPTION_STAGE_NONE ? ( tail[istage] ? tail[istage] : "-?-" ) : ( istage == DUF_OPTION_STAGE_ANY ? "any" : "-" ); */
        || istage == DUF_OPTION_STAGE_NONE ? ( tail[istage] ? tail[istage] : "-?-" ) : ( "-" );
}

/*
 * cmd. !use_stage:
 *   ok for > DUF_OPTION_STAGE_DEBUG
 * */
int
duf_optstage_check_old( duf_option_stage_t istage, const duf_longval_extended_t * extended, const duf_longval_extended_table_t * xtable )
{
  int r0 = 0;
  int r1 = 0;
  int r2 = 0;

  DUF_TRACE( options, +150, "checking stage(%s) xuse:%d xminmax:%d/%d", duf_optstage_name( istage ),
             extended->use_stage, extended->stage.min, extended->stage.max );
  DUF_TRACE( options, +150, "checking stage(%s) tuse:%d tminmax:%d/%d", duf_optstage_name( istage ), xtable->use_stage,
             xtable->stage.min, xtable->stage.max );
  r0 = ( istage == DUF_OPTION_STAGE_ANY || istage == DUF_OPTION_STAGE_ALL );
  /* r0 = r0 || ( extended->stage.flag ? 1 : 0 ); (* ???? *) */
  if ( !r0 )
  {
    if ( extended->use_stage )
      r1 = ( extended->stage.min <= istage && extended->stage.max >= istage );
    else
      r1 = ( istage > DUF_OPTION_STAGE_DEBUG );
    assert( xtable );
    if ( xtable->use_stage )
      r2 = ( xtable->stage.min <= istage && xtable->stage.max >= istage );
    else
      r2 = r1;
    r0 = ( r1 || r2 );
  }
  if ( ( extended->use_stage_mask && ( extended->stage_mask & ( 1 << istage ) ) )
       || ( xtable->use_stage_mask && ( xtable->stage_mask & ( 1 << istage ) ) ) )
  {
    r0 = 0;
  }

  DUF_TRACE( options, +150, "checked stage(%s); r0:%d", duf_optstage_name( istage ), r0 );
  return r0;
}

int
duf_optstage_check( duf_option_stage_t istage, const duf_longval_extended_t * extended, const duf_longval_extended_table_t * xtable )
{
  int r0 = 0;
  int r1 = 0;
  int r2 = 0;

  DUF_TRACE( options, +150, "checking stage(%s) xuse:%d xminmax:%d/%d", duf_optstage_name( istage ),
             extended->use_stage, extended->stage.min, extended->stage.max );
  DUF_TRACE( options, +150, "checking stage(%s) tuse:%d tminmax:%d/%d", duf_optstage_name( istage ), xtable->use_stage,
             xtable->stage.min, xtable->stage.max );
  r0 = ( istage == DUF_OPTION_STAGE_ANY || istage == DUF_OPTION_STAGE_ALL ) ? 1 : 0;
  /* if (r0>=0 && !extended->stage.flag ) r0=-1; (* ???? *) */

  if ( extended->use_stage )
    r1 = ( extended->stage.min <= istage && extended->stage.max >= istage ) ? 1 : -1;
  assert( xtable );
  if ( xtable->use_stage )
    r2 = ( xtable->stage.min <= istage && xtable->stage.max >= istage ) ? 1 : -1;
  if ( r1 > 0 )
    r0 = 1;
  else if ( r1 < 0 )
    r0 = -1;
  else if ( r2 < 0 )
    r0 = -1;

  if ( ( extended->use_stage_mask && ( extended->stage_mask & ( 1 << istage ) ) )
       || ( xtable->use_stage_mask && ( xtable->stage_mask & ( 1 << istage ) ) ) )
    r0 = -1;
  if ( r0 == 0 )
  {
    if ( istage > DUF_OPTION_STAGE_DEBUG )
      r0 = 1;
    else
      r0 = -1;
  }
#if 0
  {
    int old;

    old = duf_optstage_check_old( istage, extended, xtable );
    if ( ( old && r0 <= 0 ) || ( !old && r0 >= 0 ) || ( 0 == strcmp( extended->o.name, "help-set" ) ) )
      T( "@@>>>> %s @ %s : (r1:%d; r2:%d) => %d {old:%d}\n", extended->o.name, duf_optstage_name( istage ), r1, r2, r0, old );
  }
#endif
  DUF_TRACE( options, +150, "checked stage(%s); r0:%d", duf_optstage_name( istage ), r0 );
  return r0 > 0;
}

char *
duf_optstages_list( const duf_longval_extended_t * extended, const duf_longval_extended_table_t * xtable )
{
  char *s = NULL;

  for ( duf_option_stage_t istg = DUF_OPTION_STAGE_MIN; istg <= DUF_OPTION_STAGE_MAX; istg++ )
  {
    if ( DUF_OPTION_CHECK_STAGE( istg, extended, xtable ) )
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
