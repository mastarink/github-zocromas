/* #undef MAS_TRACING */
#define MAST_TRACE_CONFIG duf_get_cli_options_trace_config(cli)
#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_z.h"
#include "duf_maintenance_tracen.h"
/* #include "duf_maintenance_errors.h" */
#include "duf_printn_defs.h"

#include "duf_option_config.h"
/* #include "duf_config_util.h" */
#include "duf_config_output_util.h"

/* ###################################################################### */
#include "duf_option_stage.h"
/* ###################################################################### */

const char *
duf_optstage_name( const duf_config_cli_t * cli DUF_UNUSED, duf_option_stage_t istage )
{
#if 0
  static const char *tail[] = {
    [DUF_OPTION_STAGE_NONE] = "none",
    [DUF_OPTION_STAGE_BOOT] = "boot",
    [DUF_OPTION_STAGE_DEBUG] = "debug",
    [DUF_OPTION_STAGE_PRESETUP] = "presetup",
    [DUF_OPTION_STAGE_SETUP] = "setup",
    [DUF_OPTION_STAGE_INIT] = "init",
    [DUF_OPTION_STAGE_FIRST] = "first",
    [DUF_OPTION_STAGE_LOOP] = "loop",
    [DUF_OPTION_STAGE_INTERACTIVE] = "interactive",
    NULL
  };
  return ( istage >= DUF_OPTION_STAGE_MIN && istage <= DUF_OPTION_STAGE_MAX )
        /* || istage == DUF_OPTION_STAGE_NONE ? ( tail[istage] ? tail[istage] : "-?-" ) : ( istage == DUF_OPTION_STAGE_ANY ? "any" : "-" ); */
          || istage == DUF_OPTION_STAGE_NONE ? ( tail[istage] ? tail[istage] : "-?-" ) : ( "-" );
#else
  const char *stagename = NULL;

  switch ( istage )
  {
  case DUF_OPTION_STAGE_NONE:
    stagename = "none";
    break;
  case DUF_OPTION_STAGE_BOOT:
    stagename = "boot";
    break;
  case DUF_OPTION_STAGE_DEBUG:
    stagename = "debug";
    break;
  case DUF_OPTION_STAGE_PRESETUP:
    stagename = "presetup";
    break;
  case DUF_OPTION_STAGE_SETUP:
    stagename = "setup";
    break;
  case DUF_OPTION_STAGE_INIT:
    stagename = "init";
    break;
  case DUF_OPTION_STAGE_FIRST:
    stagename = "first";
    break;
  case DUF_OPTION_STAGE_LOOP:
  case DUF_OPTION_STAGE_LOOPE:
    stagename = "loop";
    break;
  case DUF_OPTION_STAGE_INTERACTIVE:
    stagename = "interactive";
    break;
  case DUF_OPTION_STAGE_ANY:
    stagename = "any";
    break;
  case DUF_OPTION_STAGE_ALL:
    stagename = "all";
    break;
  default:
    if ( istage >= DUF_OPTION_STAGE_LOOP && istage <= DUF_OPTION_STAGE_LOOPE )
      stagename = "loop";
  }
#endif
  return stagename;
}

/*
 * cmd. !use_stage:
 *   ok for > DUF_OPTION_STAGE_DEBUG
 * */
#if 0
int
duf_optstage_check_old( duf_option_stage_t istage, const duf_longval_extended_t * extended, const duf_longval_extended_vtable_t * xvtable )
{
  int r0 = 0;
  int r1 = 0;
  int r2 = 0;
  duf_option_stage_t istage_check;

  istage_check = istage;
  if ( istage > DUF_OPTION_STAGE_LOOP && istage <= DUF_OPTION_STAGE_LOOPE )
    istage_check = DUF_OPTION_STAGE_LOOP;
  else
    istage_check = istage;
  DUF_TRACE( options, +150, "checking stage(%s) xuse:%d xminmax:%d/%d", duf_optstage_name( cli, istage ),
             extended->stage_opts.use_stage, extended->stage_opts.stage.min, extended->stage_opts.stage.max );
  DUF_TRACE( options, +150, "checking stage(%s) tuse:%d tminmax:%d/%d", duf_optstage_name( cli, istage ), xvtable->stage_opts.use_stage,
             xvtable->stage_opts.stage.min, xvtable->stage_opts.stage.max );
  r0 = ( istage_check == DUF_OPTION_STAGE_ANY || istage_check == DUF_OPTION_STAGE_ALL );
/* r0 = r0 || ( extended->stage_opts.stage.flag ? 1 : 0 ); (* ???? *) */
  if ( !r0 )
  {
    if ( extended->stage_opts.use_stage )
      r1 = ( extended->stage_opts.stage.min <= istage_check && extended->stage_opts.stage.max >= istage_check );
    else
      r1 = ( istage_check > DUF_OPTION_STAGE_DEBUG );
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
  DUF_TRACE( options, +150, "checked stage(%s); r0:%d", duf_optstage_name( cli, istage ), r0 );
  return r0;
}
#endif

int
duf_optstage_check( const duf_config_cli_t * cli, duf_option_stage_t istage, const duf_longval_extended_t * extended,
                    const duf_longval_extended_vtable_t * xvtable )
{
  int r0 = 0;
  int r1 = 0;
  int r2 = 0;
  duf_option_stage_t istage_check;

  istage_check = istage;
  if ( istage > DUF_OPTION_STAGE_LOOP && istage <= DUF_OPTION_STAGE_LOOPE )
    istage_check = DUF_OPTION_STAGE_LOOP;
  else
    istage_check = istage;

  DUF_TRACE( options, +150, "checking stage(%s) xuse:%d xminmax:%d/%d", duf_optstage_name( cli, istage ),
             extended->stage_opts.use_stage, extended->stage_opts.stage.min, extended->stage_opts.stage.max );
  DUF_TRACE( options, +150, "checking stage(%s) tuse:%d tminmax:%d/%d", duf_optstage_name( cli, istage ), xvtable->stage_opts.use_stage,
             xvtable->stage_opts.stage.min, xvtable->stage_opts.stage.max );
  r0 = ( istage_check == DUF_OPTION_STAGE_ANY || istage_check == DUF_OPTION_STAGE_ALL ) ? 1 : 0;
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
    if ( istage_check > DUF_OPTION_STAGE_DEBUG )
      r0 = 1;
    else
      r0 = -1;
  }
#if 0
  {
    int old;

    old = duf_optstage_check_old( istage, extended, xvtable );
    if ( ( old && r0 <= 0 ) || ( !old && r0 >= 0 ) || ( 0 == strcmp( extended->o.name, "help-set" ) ) )
      T( "@@>>>> %s @ %s : (r1:%d; r2:%d) => %d {old:%d}\n", extended->o.name, duf_optstage_name( cli, istage ), r1, r2, r0, old );
  }
#endif
  DUF_TRACE( options, +150, "checked stage(%s); r0:%d", duf_optstage_name( cli, istage ), r0 );
  return r0 > 0;
}

char *
duf_optstages_list( const duf_config_cli_t * cli, const duf_longval_extended_t * extended, const duf_longval_extended_vtable_t * xvtable )
{
  char *s = NULL;

  for ( duf_option_stage_t istg = DUF_OPTION_STAGE_MIN; istg <= DUF_OPTION_STAGE_MAX; istg++ )
  {
    if ( DUF_OPTION_CHECK_STAGE( cli, istg, extended, xvtable ) )
      s = mas_strncat_x( s, duf_optstage_name( cli, istg ), 1 );
    else
      s = mas_strcat_x( s, "_" );
  }
  return s;
}

void
duf_optstage_print( const duf_config_cli_t * cli, int use_stage, int use_stage_mask, duf_limits_stage_t stage, unsigned long stage_mask,
                    unsigned eol )
{
  if ( use_stage || use_stage_mask )
  {
    DUF_PRINTF( 0, ".  " );
    if ( use_stage )
    {
      DUF_PRINTF( 0, ".stage(%s(%d),%s(%d)) ", duf_optstage_name( cli, stage.min ), stage.min, duf_optstage_name( cli, stage.max ), stage.max );
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
          DUF_PRINTF( 0, ".%s; ", duf_optstage_name( cli, n ) );
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
