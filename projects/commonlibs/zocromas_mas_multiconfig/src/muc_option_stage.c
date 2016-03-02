/* #undef MAS_TRACING */
/* #define MAST_TRACE_CONFIG muc_get_cli_options_trace_config(cli) */
#include "muc_tracen_defs_preset.h"
#include "muc_config_trace_enum.h"


#include <assert.h>                                                  /* assert */
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */

/* #include "muc_tracen_defs.h"                                         (* MAST_TRACE ♠ *) */
#include "muc_printn_defs.h"                                         /* MUC_PRINTF etc. ♠ */

#include "muc_option_config.h"                                       /* muc_get_cli_options_trace_config ♠ */

/* ###################################################################### */
#include "muc_option_stage.h"                                        /* muc_optstage_name ♠ */
/* ###################################################################### */

const char *
muc_optstage_name( const muc_config_cli_t * cli MAS_UNUSED, muc_option_stage_t istage )
{
#if 0
  static const char *tail[] = {
    [MUC_OPTION_STAGE_NONE] = "none",
    [MUC_OPTION_STAGE_BOOT] = "boot",
    [MUC_OPTION_STAGE_DEBUG] = "debug",
    [MUC_OPTION_STAGE_PRESETUP] = "presetup",
    [MUC_OPTION_STAGE_SETUP] = "setup",
    [MUC_OPTION_STAGE_INIT] = "init",
    [MUC_OPTION_STAGE_FIRST] = "first",
    [MUC_OPTION_STAGE_LOOP] = "loop",
    [MUC_OPTION_STAGE_INTERACTIVE] = "interactive",
    NULL
  };
  return ( istage >= MUC_OPTION_STAGE_MIN && istage <= MUC_OPTION_STAGE_MAX )
        /* || istage == MUC_OPTION_STAGE_NONE ? ( tail[istage] ? tail[istage] : "-?-" ) : ( istage == MUC_OPTION_STAGE_ANY ? "any" : "-" ); */
          || istage == MUC_OPTION_STAGE_NONE ? ( tail[istage] ? tail[istage] : "-?-" ) : ( "-" );
#else
  const char *stagename = NULL;

  switch ( istage )
  {
  case MUC_OPTION_STAGE_NONE:
    stagename = "none";
    break;
  case MUC_OPTION_STAGE_BOOT:
    stagename = "boot";
    break;
  case MUC_OPTION_STAGE_DEBUG:
    stagename = "debug";
    break;
  case MUC_OPTION_STAGE_PRESETUP:
    stagename = "presetup";
    break;
  case MUC_OPTION_STAGE_SETUP:
    stagename = "setup";
    break;
  case MUC_OPTION_STAGE_INIT:
    stagename = "init";
    break;
  case MUC_OPTION_STAGE_FIRST:
    stagename = "first";
    break;
  case MUC_OPTION_STAGE_LOOP:
  case MUC_OPTION_STAGE_LOOPE:
    stagename = "loop";
    break;
  case MUC_OPTION_STAGE_INTERACTIVE:
    stagename = "Interactive";
    break;
  case MUC_OPTION_STAGE_ANY:
    stagename = "any";
    break;
  case MUC_OPTION_STAGE_ALL:
    stagename = "all";
    break;
  default:
    if ( istage >= MUC_OPTION_STAGE_LOOP && istage <= MUC_OPTION_STAGE_LOOPE )
      stagename = "loop";
  }
#endif
  return stagename;
}

/*
 * cmd. !use_stage:
 *   ok for > MUC_OPTION_STAGE_DEBUG
 * */
#if 0
int ...
_optstage_check_old( ... _option_stage_t istage, const ... _longval_extended_t * extended, const ... _longval_extended_vtable_t * xvtable )
{
  int r0 = 0;
  int r1 = 0;
  int r2 = 0;

  ... _option_stage_t istage_check;

  istage_check = istage;
  if ( istage > MUC_OPTION_STAGE_LOOP && istage <= MUC_OPTION_STAGE_LOOPE )
    istage_check = MUC_OPTION_STAGE_LOOP;
  else
    istage_check = istage;
  MAST_TRACE( options, +150, "checking stage(%s) xuse:%d xminmax:%d/%d", ... _optstage_name( cli, istage ),
              extended->stage_opts.use_stage, extended->stage_opts.stage.min, extended->stage_opts.stage.max );
  MAST_TRACE( options, +150, "checking stage(%s) tuse:%d tminmax:%d/%d", ... _optstage_name( cli, istage ), xvtable->stage_opts.use_stage,
              xvtable->stage_opts.stage.min, xvtable->stage_opts.stage.max );
  r0 = ( istage_check == MUC_OPTION_STAGE_ANY || istage_check == MUC_OPTION_STAGE_ALL );
/* r0 = r0 || ( extended->stage_opts.stage.flag ? 1 : 0 ); (* ???? *) */
  if ( !r0 )
  {
    if ( extended->stage_opts.use_stage )
      r1 = ( extended->stage_opts.stage.min <= istage_check && extended->stage_opts.stage.max >= istage_check );
    else
      r1 = ( istage_check > MUC_OPTION_STAGE_DEBUG );
    assert( xvtable );
    if ( xvtable->stage_opts.use_stage )
      r2 = ( xvtable->stage_opts.stage.min <= istage_check && xvtable->stage_opts.stage.max >= istage_check );
    else
      r2 = r1;
    r0 = ( r1 || r2 );
  }
  if ( ( extended->stage_opts.use_stage_mask && ( extended->stage_opts.stage_mask & ( 1 << istage_check ) ) )
       || ( xvtable->stage_opts.use_stage_mask && ( xvtable->stage_opts.stage_mask & ( 1 << istage_check ) ) ) )
  {
    r0 = 0;
  }
  MAST_TRACE( options, +150, "checked stage(%s); r0:%d", ... _optstage_name( cli, istage ), r0 );
  return r0;
}
#endif

int
muc_optstage_check( const muc_config_cli_t * cli, muc_option_stage_t istage, const muc_longval_extended_t * extended,
                    const muc_longval_extended_vtable_t * xvtable )
{
  int r0 = 0;
  int r1 = 0;
  int r2 = 0;
  muc_option_stage_t istage_check;

  istage_check = istage;
  if ( istage > MUC_OPTION_STAGE_LOOP && istage <= MUC_OPTION_STAGE_LOOPE )
    istage_check = MUC_OPTION_STAGE_LOOP;
  else
    istage_check = istage;

  MAST_TRACE( options, +150, "checking stage(%s) xuse:%d xminmax:%d/%d", muc_optstage_name( cli, istage ),
              extended->stage_opts.use_stage, extended->stage_opts.stage.min, extended->stage_opts.stage.max );
  MAST_TRACE( options, +150, "checking stage(%s) tuse:%d tminmax:%d/%d", muc_optstage_name( cli, istage ), xvtable->stage_opts.use_stage,
              xvtable->stage_opts.stage.min, xvtable->stage_opts.stage.max );
  r0 = ( istage_check == MUC_OPTION_STAGE_ANY || istage_check == MUC_OPTION_STAGE_ALL ) ? 1 : 0;
/* if (r0>=0 && !extended->stage_opts.stage.flag ) r0=-1; (* ???? *) */

  if ( extended->stage_opts.use_stage )
    r1 = ( extended->stage_opts.stage.min <= istage_check && extended->stage_opts.stage.max >= istage_check ) ? 1 : -1;
  assert( xvtable );
  if ( xvtable->stage_opts.use_stage )
    r2 = ( xvtable->stage_opts.stage.min <= istage_check && xvtable->stage_opts.stage.max >= istage_check ) ? 1 : -1;
  if ( r1 > 0 )
    r0 = 1;
  else if ( r1 < 0 )
    r0 = -1;
  else if ( r2 < 0 )
    r0 = -1;

  if ( ( extended->stage_opts.use_stage_mask && ( extended->stage_opts.stage_mask & ( 1 << istage_check ) ) )
       || ( xvtable->stage_opts.use_stage_mask && ( xvtable->stage_opts.stage_mask & ( 1 << istage_check ) ) ) )
    r0 = -1;
  if ( r0 == 0 )
  {
    if ( istage_check > MUC_OPTION_STAGE_DEBUG )
      r0 = 1;
    else
      r0 = -1;
  }
#if 0
  {
    int old;

    old = ... _optstage_check_old( istage, extended, xvtable );
    if ( ( old && r0 <= 0 ) || ( !old && r0 >= 0 ) || ( 0 == strcmp( extended->o.name, "help-set" ) ) )
      T( "@@>>>> %s @ %s : (r1:%d; r2:%d) => %d {old:%d}\n", extended->o.name, muc_optstage_name( cli, istage ), r1, r2, r0, old );
  }
#endif
  MAST_TRACE( options, +150, "checked stage(%s); r0:%d", muc_optstage_name( cli, istage ), r0 );
  return r0 > 0;
}

char *
muc_optstages_list( const muc_config_cli_t * cli, const muc_longval_extended_t * extended, const muc_longval_extended_vtable_t * xvtable )
{
  char *s = NULL;

  for ( muc_option_stage_t istg = MUC_OPTION_STAGE_MIN; istg <= MUC_OPTION_STAGE_MAX; istg++ )
  {
    if ( istg <= MUC_OPTION_STAGE_LOOP || istg > MUC_OPTION_STAGE_LOOPE )
    {
      if ( MUC_OPTION_CHECK_STAGE( cli, istg, extended, xvtable ) )
        s = mas_strncat_x( s, muc_optstage_name( cli, istg ), 1 );
      else
        s = mas_strcat_x( s, "_" );
    }
  }
  return s;
}

void
muc_optstage_print( const muc_config_cli_t * cli, int use_stage, int use_stage_mask, muc_limits_stage_t stage, unsigned long stage_mask,
                    unsigned eol )
{
  if ( use_stage || use_stage_mask )
  {
    MUC_PRINTF( 0, ".  " );
    if ( use_stage )
    {
      MUC_PRINTF( 0, ".stage(%s(%d),%s(%d)) ", muc_optstage_name( cli, stage.min ), stage.min, muc_optstage_name( cli, stage.max ), stage.max );
    }
    if ( use_stage_mask )
    {
      unsigned long msk = stage_mask;
      unsigned n = 0;

      MUC_PRINTF( 0, ".mask(%lu; { ", stage_mask );
      while ( msk )
      {
        if ( msk & 1 )
        {
          MUC_PRINTF( 0, ".%s; ", muc_optstage_name( cli, n ) );
        }
        n++;
        msk = msk >> 1;
      }
      MUC_PRINTF( 0, ".} )" );
    }
    if ( eol )
      MUC_PUTSL( 0 );
  }
}
